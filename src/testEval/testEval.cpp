/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#include <testEval.h>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <math.h>
#include <testUtil.h>
#include <string>
#include <vector>

testEval::testEval(testResultType* testResults, testSetup& TS) {
  Results = testResults;

  TS.initEvaluation(reportFilePath, referFilePath, resultFilePath, minConf, maxDeviation, resultHeader);

  testEval::getReference();
}

void testEval::writeReportFile() {
  testUtil::writeInfo("Invoke writeReportFile");

  int frame_count = 0;
  fstream fs_result;
  fs_result.open(reportFilePath.c_str(), ios::out);

  fs_result << testUtil::timeStamp() << std::endl;

  for (std::vector<frameResultType>::iterator it = Results->begin(); it != Results->end(); ++it) {
    fs_result << "-- Frame : " << frame_count++ << std::endl;

    fs_result << "---- Time : " <<  testEval::calcTime(it->first) << std::endl;

    int cluster_count = 0;

    for (std::vector<personClusterType>::iterator c_it = it->second->begin(); c_it != it->second->end(); ++c_it) {
      if (c_it->getPersonConfidence() > minConf) {
        fs_result << "---- Cluster : " << cluster_count++  <<std::endl;
        fs_result << "------ B_X   : " <<c_it->getBottom()[0] << std::endl;
        fs_result << "------ B_Y   : " <<c_it->getBottom()[1] << std::endl;
        fs_result << "------ B_Z   : " <<c_it->getBottom()[2] << std::endl;
        fs_result << "------ HOG_C : " <<c_it->getPersonConfidence() << std::endl;
      }
    }
  }

  fs_result.close();

  testUtil::writeInfo("writeReportFile finished");
}

void testEval::getReference() {
  const std::string frameCount_S = "Frame_Count";
  const std::string frameNumber_S = "Frame_Number";

  int frameCount = 0;
  int frameNumber = 0;

  std::fstream fs_refer;
  fs_refer.open(referFilePath.c_str(), std::ios::in);

  std::string refer_line;

  while (!fs_refer.eof()) {
    getline(fs_refer, refer_line);

    if (refer_line.find(frameCount_S) != std::string::npos) {
      refer_line.replace(refer_line.find(frameCount_S), frameCount_S.length(), "");

      boost::trim(refer_line);

      std::istringstream iss(refer_line);

      iss >> frameCount;

      testReference = new referResultType(frameCount);
    } else if (refer_line.find(frameNumber_S) != std::string::npos) {
      refer_line.replace(refer_line.find(frameNumber_S), frameNumber_S.length(), "");

      boost::trim(refer_line);

      std::istringstream iss(refer_line);

      iss >> frameNumber;

    } else if (!refer_line.empty()) {
      std::istringstream iss(refer_line);

      referEntryType entry;

      iss >> entry.first.first;
      iss >> entry.first.second;

      entry.second = false;

      testReference->at(frameNumber - 1).push_back(entry);
    }
  }

  fs_refer.close();
}

float testEval::calcDistance(float X_1, float Z_1, float X_2 , float Z_2) {
  return sqrt(pow((X_2 -X_1), 2) + pow((Z_2 -Z_1), 2));
}

bool testEval::evalMatch(float distance) {
  return (distance <= maxDeviation);
}

float testEval::calcTime(timespec time) {
  float exec_time;

  if (time.tv_nsec > 0) {
    exec_time = float(time.tv_nsec) / 1000000000;
  } else {
    exec_time = float(time.tv_nsec + 1000000000) / 1000000000;
  }

  return exec_time;
}

void testEval::calcTimeCharac(float& meanET, float& stdDevET, float& minET, float& maxET) {
  meanET = 0.0;
  stdDevET = 0.0;
  minET = 100.0;
  maxET = 0.0;

  for (std::vector<frameCompareType>::iterator it = compareResult->begin(); it != compareResult->end(); ++it) {
    meanET += it->execTime;

    if (it->execTime > maxET) {
      maxET = it->execTime;
    }

    if (it->execTime < minET) {
      minET = it->execTime;
    }
  }

  meanET = meanET/compareResult->size();

  for (std::vector<frameCompareType>::iterator it = compareResult->begin(); it != compareResult->end(); ++it) {
    stdDevET += pow((it->execTime - meanET), 2);
  }
  stdDevET = sqrt((stdDevET)/compareResult->size());
}

void testEval::writeResultFile() {
  testUtil::writeInfo("Invoke writeResultFile");

  fstream fs_result;
  fs_result.open(resultFilePath.c_str(), ios::out);

  fs_result << "# " << testUtil::timeStamp();

  fs_result << resultHeader.str();

  float minTime;
  float maxTime;
  float meanTime;
  float stdDevT;

  testEval::calcTimeCharac(meanTime, stdDevT, minTime, maxTime);

  fs_result << "# -- Characteristics --" << std::endl;
  fs_result << "# meanTime : " << meanTime << std::endl;
  fs_result << "# stdDevT  : " << stdDevT << std::endl;
  fs_result << "# minTime  : " << minTime << std::endl;
  fs_result << "# maxTime  : " << maxTime << std::endl;

  fs_result << "# ---- Detection  ----" << std::endl;
  fs_result << "# detRate : " << std::setprecision(3) << detRate << std::endl;
  fs_result << "# maxFaSe : " << maxFaultSeq << std::endl;
  fs_result << "# ---------------------" << std::endl;

  fs_result << setw(6) << "#FRAME";
  fs_result << setw(11) << "EXE_TIME";
  fs_result << setw(5) << "FP";
  fs_result << setw(5) << "FN";
  fs_result << setw(5) << "MA";
  fs_result << setw(9) << "MA_DEV" << std::endl;

  for (std::vector<frameCompareType>::iterator it = compareResult->begin(); it != compareResult->end(); ++it) {
    fs_result << setw(6) << it->frameNumber;
    fs_result << setw(11) << std::setprecision(5) << it->execTime;
    fs_result << setw(5) << it->falsePos;
    fs_result << setw(5) << it->falseNeg;
    fs_result << setw(5) << it->match;
    fs_result << setw(9) << std::setprecision(2) << it->matchDev << std::endl;
  }

  fs_result.close();

  testUtil::writeInfo("writeResultFile finished");
}

void testEval::performEvaluation() {
  testUtil::writeInfo("Invoke performEvaluation");

  compareResult = new compareResultType(testReference->size());

  detRate = 0;
  maxFaultSeq = 0;
  int tempFaultSeq = 0;

  for (int frame = 0; frame < testReference->size(); frame++) {
    int match_count = 0;
    float match_dev = 0;
    int detection_count = 0;
    frameCompareType frameResult;

    for (std::vector<personClusterType>::iterator res_it = Results->at(frame).second->begin(); res_it != Results->at(frame).second->end(); ++res_it) {
      if (res_it->getPersonConfidence() >= minConf) {
        detection_count++;

        for (std::vector<referEntryType>::iterator ref_it = testReference->at(frame).begin(); ref_it != testReference->at(frame).end(); ++ref_it) {
          if (!(ref_it->second)) {  // no match yet{
            float deviation = testEval::calcDistance(ref_it->first.first, ref_it->first.second, res_it->getBottom()[0], res_it->getBottom()[2]);

            if (testEval::evalMatch(deviation)) {
              ref_it->second = true;
              match_count++;
              match_dev += deviation;
              break;
            }
          }
        }
      }
    }

    frameResult.frameNumber = frame;
    frameResult.execTime = testEval::calcTime(Results->at(frame).first);
    // falsePos = clusters per frame (HOG_conf > minConf) minus matches per frame
    frameResult.falsePos = detection_count - match_count;
    // falseNeg = real objects per frame minus matches per frame
    frameResult.falseNeg = testReference->at(frame).size() - match_count;
    frameResult.match = match_count;
    // matchDev = average deviation of all matches per frame
    if (match_count > 0) {
      frameResult.matchDev = (match_dev / match_count) / maxDeviation;
    } else {
      frameResult.matchDev = -1.0;
    }

    if (match_count == testReference->at(frame).size()) {
      detRate++;

      if (tempFaultSeq > maxFaultSeq) {
        maxFaultSeq = tempFaultSeq;
      }

      tempFaultSeq = 0;

    } else {
      tempFaultSeq++;
    }

    compareResult->at(frame) = frameResult;
  }

  detRate = detRate/testReference->size();

  testUtil::writeInfo("performEvaluation finished");

  testEval::writeResultFile();
  testEval::writeReportFile();
}
