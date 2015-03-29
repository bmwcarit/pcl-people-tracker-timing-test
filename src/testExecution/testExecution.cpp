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

testExecution::testExecution(const testSetup& TS) {
  inputFileName = TS.getInputPath();
  bag.open(inputFileName, rosbag::bagmode::Read);

  topics.push_back(TS.getTopicName());
  topicView = new rosbag::View(bag, rosbag::TopicQuery(topics));

  testUtil::writeInfo("Frame Count : " + boost::lexical_cast<std::string>(topicView->size()));

  rgb_intrinsics_matrix = TS.getIntMatrix();
  svm_filename = TS.getSvmFile();
  person_classifier.loadSVMFromFile(svm_filename);
  groundCoeffs = TS.getGroundCoeffs();

  peopleDetector.setIntrinsics(rgb_intrinsics_matrix);
  peopleDetector.setVoxelSize(TS.getVoxelSize());
  peopleDetector.setClassifier(person_classifier);
  peopleDetector.setPersonClusterLimits(TS.getMinHeight(), TS.getMaxHeight(), TS.getMinWidth(), TS.getMaxWidth());
  peopleDetector.setSamplingFactor(TS.getSamplingFactor());
  peopleDetector.setGround(*groundCoeffs);
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

    clock_gettime(clock_id, &begin);
    peopleDetector.compute(*clusters);
    clock_gettime(clock_id, &end);

    testResults->at(frame_count).first.tv_nsec = end.tv_nsec - begin.tv_nsec;

    testResults->at(frame_count).second = clusters;

    frame_count++;
  }

  testUtil::writeInfo("performTest finished");

  return testResults;
}
