/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#ifndef SRC_TESTEVAL_TESTEVAL_H_
#define SRC_TESTEVAL_TESTEVAL_H_

#include <testDef.h>
#include <testSetup.h>
#include <time.h>
#include <string>

class testEval {
 public:
  testEval(testResultType* testResults, testSetup& TS);
  ~testEval() {};

  void performEvaluation();

 private:
  int maxFaultSeq;
  float minConf;
  float maxDeviation;  // meter
  float detRate;
  std::string resultFilePath;
  std::string reportFilePath;
  std::string referFilePath;
  std::stringstream resultHeader;
  referResultType* testReference;
  testResultType* Results;
  compareResultType* compareResult;

  void writeResultFile();

  void writeReportFile();

  void getReference();

  float calcDistance(float X_1, float Z_1, float X_2 , float Z_2);

  float calcTime(timespec time);

  bool evalMatch(float distance);

  void calcTimeCharac(float& meanET, float& stdDevET, float& minET, float& maxET);
};
#endif  // SRC_TESTEVAL_TESTEVAL_H_
