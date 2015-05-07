/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#include <testExecution.h>
#include <boost/foreach.hpp>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <time.h>
#include <testUtil.h>
#include <boost/lexical_cast.hpp>
#include <sys/mman.h>

testExecution::testExecution(testSetup& TS) {
  TS.initExecution(peopleDetector, inputFileName, topicName);

  bag.open(inputFileName, rosbag::bagmode::Read);
  topics.push_back(topicName);
  topicView = new rosbag::View(bag, rosbag::TopicQuery(topics));

  testUtil::writeInfo("Frame Count : " + boost::lexical_cast<std::string>(topicView->size()));
}

testExecution::~testExecution() {
  delete topicView;
}

testResultType* testExecution::performTest() {
  testUtil::writeInfo("Invoke performTest");

  testResults = new testResultType (topicView->size());
  int frame_count = 0;

  BOOST_FOREACH(const rosbag::MessageInstance &msg, *topicView) {
    testUtil::writeInfo("Frame : " + boost::lexical_cast<std::string>(frame_count));

    sensor_msgs::PointCloud2::ConstPtr cloudMsg =
    msg.instantiate<sensor_msgs::PointCloud2>();

    SourcePointCloud cloud;
    pcl::fromROSMsg(*cloudMsg, cloud);

    SourcePointCloud::Ptr pc(new SourcePointCloud(cloud));

    clustersVerctorType* clusters = new clustersVerctorType();

    peopleDetector.setInputCloud(pc);

    timespec begin, end;

    if (mlockall(MCL_CURRENT|MCL_FUTURE) == 0) {
      clock_gettime(clock_id, &begin);
      peopleDetector.compute(*clusters);
      clock_gettime(clock_id, &end);
    } else {
      perror("mlockall failed");
      testUtil::writeError("Test aborted!");
    }
    munlockall();

    testResults->at(frame_count).first.tv_nsec = end.tv_nsec - begin.tv_nsec;
        testResults->at(frame_count).first.tv_sec = end.tv_sec - begin.tv_sec;

    testResults->at(frame_count).second = clusters;

    frame_count++;
  }

  testUtil::writeInfo("performTest finished");

  return testResults;
}
