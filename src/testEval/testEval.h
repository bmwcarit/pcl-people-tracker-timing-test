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
  testEval(testResultType* testResults, const testSetup& TS);
  ~testEval() {};

  void performEvaluation();

 private:
  void writeResultFile();

  void writeReportFile();

  void getResultHeader(const testSetup& TS);

  void getReference();

  float calcDistance(float X_1, float Z_1, float X_2 , float Z_2);

  float calcTime(timespec time);

  bool evalMatch(float distance);

  void calcTimeCharac(float& meanET, float& stdDevET, float& minET, float& maxET);

  referResultType* testReference;
  std::string referFilePath;
  float minConf;
  testResultType* Results;

  float maxDeviation;  // meter

  compareResultType* compareResult;

  std::string resultFilePath;

  std::string reportFilePath;

  std::stringstream resultHeader;

  float detRate;

  int maxFaultSeq;
};
#endif  // SRC_TESTEVAL_TESTEVAL_H_
