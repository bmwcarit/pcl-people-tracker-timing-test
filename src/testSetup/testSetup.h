/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#ifndef SRC_TESTSETUP_TESTSETUP_H_
#define SRC_TESTSETUP_TESTSETUP_H_

#include <Eigen/Dense>
#include <testDef.h>

class testSetup {
 public:
  testSetup(int argc, char *argv[]);
  ~testSetup();

  void initEvaluation(std::string& outputPath, std::string& referPath, std::string& resultPath, float& minConf, float& maxDeviation, std::stringstream& resultHeader);
  void initExecution(peopleDetectorType& PD, std::string& inputPath, std::string& topic);

 private:
  float min_confidence;
  float min_height;
  float max_height;
  float min_width;
  float max_width;
  float voxel_size;
  float sampling_factor;
  float maxDev;  // meter
  std::string inputFile;
  std::string outputFile;
  std::string referFile;
  std::string resultFile;
  std::string svm_file;
  std::string topicName;
  Eigen::Matrix3f rgb_intrinsics_matrix;
  Eigen::VectorXf* groundCoeffs;
  personClassifierType person_classifier;

  void setInputs(int argc, char *argv[]);

  void readConfig();

  void getResultHeader(std::stringstream& resultHeader);

  void initPeopleDetector(peopleDetectorType& PD);
};
#endif  // SRC_TESTSETUP_TESTSETUP_H_
