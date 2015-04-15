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
  testExecution(testSetup& TS);
  ~testExecution();

testResultType* performTest();

 private:
  const int clock_id = CLOCK_MONOTONIC_RAW;
  std::string inputFileName;
  std::string topicName;
  std::vector<std::string> topics;
  rosbag::Bag bag;
  peopleDetectorType peopleDetector;
  rosbag::View* topicView;
  testResultType* testResults;
};
#endif  // SRC_TESTEXECUTION_TESTEXECUTION_H_
