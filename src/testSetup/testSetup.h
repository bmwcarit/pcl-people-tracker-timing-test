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
#include <string>

class testSetup {
 public:
  testSetup() {};
  ~testSetup();

  void setInputs(int argc, char *argv[]);

  float getMinConf() const;
  float getMinHeight() const;
  float getMaxHeight() const;
  float getMinWidth() const;
  float getMaxWidth() const;
  float getVoxelSize() const;
  float getSamplingFactor() const;
  float getMaxDeviation() const;
  std::string getInputPath() const;
  std::string getOutputPath() const;
  std::string getTopicName() const;
  Eigen::Matrix3f getIntMatrix() const;
  std::string getSvmFile() const;
  Eigen::VectorXf* getGroundCoeffs() const;
  std::string getReferPath() const;
  std::string getResulPath() const;

 private:
  void readConfig();
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
};
#endif  // SRC_TESTSETUP_TESTSETUP_H_
