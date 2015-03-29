/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#ifndef SRC_TESTEXECUTION_TESTEXECUTION_H_
#define SRC_TESTEXECUTION_TESTEXECUTION_H_

#include <string>
#include <vector>
#include "rosbag/bag.h"
#include "rosbag/view.h"
#include <testDef.h>
#include <testSetup.h>

class testExecution {
 public:
  testExecution(const testSetup& TS);
  ~testExecution();
testResultType* performTest();

 private:
  rosbag::Bag bag;
  std::vector<std::string> topics;

  std::string inputFileName;

  peopleDetectorType peopleDetector;
  rosbag::View* topicView;

  testResultType* testResults;

  Eigen::Matrix3f rgb_intrinsics_matrix;

  pcl::people::PersonClassifier<pcl::RGB> person_classifier;
  std::string svm_filename;

  Eigen::VectorXf* groundCoeffs;

  const int clock_id = CLOCK_MONOTONIC_RAW;
};
#endif  // SRC_TESTEXECUTION_TESTEXECUTION_H_
