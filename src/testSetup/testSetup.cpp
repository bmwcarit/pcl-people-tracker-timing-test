/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#include <testSetup.h>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include <testDef.h>
#include <testUtil.h>


void testSetup::setInputs(int argc, char *argv[]) {
  if ((argc == 17) || (argc == 18)) {
    bool minc_set = false;
    bool minh_set = false;
    bool maxh_set = false;
    bool minw_set = false;
    bool maxw_set = false;
    bool vs_set   = false;
    bool sf_set   = false;

    // Read TestConfig.txt
    testSetup::readConfig();

    // path to input file (XXX.bag)
    if (boost::filesystem::exists(boost::filesystem::current_path().string() + "/" + std::string(argv[1]))) {
      inputFile = boost::filesystem::current_path().string() + "/" + std::string(argv[1]);
    } else {
      testUtil::writeError("InputFile not found.");
    }

    // path to result file
    std::string result_dir = boost::filesystem::current_path().string() + "/Results";

    if (!boost::filesystem::exists(result_dir)) {
      boost::filesystem::create_directory(result_dir);
    }

    std::string bag_name = std::string(argv[1]);
    bag_name.replace(bag_name.find(".bag"), 4, "");

    if (argc == 17) {
      outputFile = result_dir + "/" + bag_name + "_Report_" + ".txt";
      resultFile = result_dir + "/" + bag_name + "_Result_" + ".txt";
    } else if (argc == 18) {
      outputFile = result_dir + "/" + bag_name + "_Report_" + std::string(argv[17]) + ".txt";
      resultFile = result_dir + "/" + bag_name + "_Result_" + std::string(argv[17]) + ".txt";
    }

    // path to refer file
    if (boost::filesystem::exists(boost::filesystem::current_path().string() + "/ReferFiles/" + std::string(argv[2]))) {
      referFile = boost::filesystem::current_path().string() + "/ReferFiles/" + std::string(argv[2]);
    } else {
      testUtil::writeError("No reference file found.");
    }

    // setting of test parameters
    for (int i = 3; i < argc; i++) {
      std::string arg(argv[i]);

      if (arg.compare("-minc") == 0) {
        min_confidence = atof(argv[i+1]);
        i++;
        minc_set = true;
      } else if (arg.compare("-minh") == 0) {
        min_height = atof(argv[i+1]);
        i++;
        minh_set = true;
      } else if (arg.compare("-maxh") == 0) {
        max_height = atof(argv[i+1]);
        i++;
        maxh_set = true;
      } else if (arg.compare("-minw") == 0) {
        min_width = atof(argv[i+1]);
        i++;
        minw_set = true;
      } else if (arg.compare("-maxw") == 0) {
        max_width = atof(argv[i+1]);
        i++;
        maxw_set = true;
      } else if (arg.compare("-vs") == 0) {
        voxel_size = atof(argv[i+1]);
        i++;
        vs_set = true;
      } else if (arg.compare("-sf") == 0) {
        sampling_factor = atof(argv[i+1]);
        i++;
        sf_set = true;
      }
    }

    if (!(minc_set && minh_set && maxh_set && minw_set && maxw_set && vs_set && sf_set)) {
      testUtil::writeError("Test parameter not set.");
    }
  } else {
    testUtil::writeError("Incorrect inputs.");
  }
}

void testSetup::readConfig() {
  std::string testConfigPath = boost::filesystem::current_path().string() + "/TestConfig.txt";

  // Configuration Parameters
  const std::string topic_S = "topic";
  const std::string rgb_intrinsics_matrix_S = "rgb_intrinsics_matrix";
  const std::string svm_file_S = "svm_filename";
  const std::string groundCoeffs_S = "groundCoeffs";
  const std::string maxDeviation_S = "maxDeviation";
  bool topic_set = false;
  bool rgb_intrinsics_matrix_set = false;
  bool svm_file_set = false;
  bool groundCoeffs_set = false;
  bool maxDeviation_set = false;

  if (boost::filesystem::exists(testConfigPath )) {
    std::fstream fs_config;
    fs_config.open(testConfigPath.c_str(), std::ios::in);
    std::string config_line;

    while (!fs_config.eof()) {
      getline(fs_config, config_line);

      if (config_line.find(topic_S) != std::string::npos) {
        config_line.replace(config_line.find(topic_S), topic_S.length(), "");

        boost::trim(config_line);

        topicName = config_line;

        topic_set = true;
      }

      if (config_line.find(svm_file_S) != std::string::npos) {
        config_line.replace(config_line.find(svm_file_S), svm_file_S.length(), "");

        boost::trim(config_line);

        svm_file = boost::filesystem::current_path().string() + "/" + config_line;

        svm_file_set = true;
      }

      if (config_line.find(rgb_intrinsics_matrix_S) != std::string::npos) {
        config_line.replace(config_line.find(rgb_intrinsics_matrix_S), rgb_intrinsics_matrix_S.length(), "");

        boost::trim(config_line);

        std::istringstream iss(config_line);

        std::vector<float> matrix(9);

        for (int i=0; i <= 8; i++) {
          iss >> matrix[i];
        }

        rgb_intrinsics_matrix << matrix[0], matrix[1], matrix[2], matrix[3], matrix[4], matrix[5], matrix[6], matrix[7], matrix[8];

        rgb_intrinsics_matrix_set = true;
      }

      if (config_line.find(groundCoeffs_S) != std::string::npos) {
        config_line.replace(config_line.find(groundCoeffs_S), groundCoeffs_S.length(), "");

        boost::trim(config_line);

        std::istringstream iss(config_line);

        std::vector<float> vec(4);

        for (int i=0; i <= 3; i++) {
          iss >> vec[i];
        }

        groundCoeffs = new Eigen::VectorXf(4);

        *groundCoeffs << vec[0], vec[1], vec[2], vec[3];

        groundCoeffs_set = true;
      }

      if (config_line.find(maxDeviation_S) != std::string::npos) {
        config_line.replace(config_line.find(maxDeviation_S), maxDeviation_S.length(), "");

        boost::trim(config_line);

        std::istringstream iss(config_line);

        iss >> maxDev;

        maxDeviation_set = true;
      }
    }
    
    fs_config.close();

    if (!(topic_set && rgb_intrinsics_matrix_set && svm_file_set && groundCoeffs_set && maxDeviation_set)) {
      testUtil::writeError("Settings in TestConfig.txt missing.");
    }
  } else {
    testUtil::writeError("No TestConfig.txt found in " + boost::filesystem::current_path().string());
  }
}

testSetup::~testSetup() {
  delete groundCoeffs;
}

float testSetup::getMinConf() const {
  return min_confidence;
}

float testSetup::getMinHeight() const {
  return min_height;
}

float testSetup::getMaxHeight() const {
  return max_height;
}

float testSetup::getMinWidth() const {
  return min_width;
}

float testSetup::getMaxWidth() const {
  return max_width;
}

float testSetup::getVoxelSize() const {
  return voxel_size;
}

float testSetup::getSamplingFactor() const {
  return sampling_factor;
}

std::string testSetup::getInputPath() const {
  return inputFile;
}

std::string testSetup::getOutputPath() const {
  return outputFile;
}

std::string testSetup::getTopicName() const {
  return topicName;
}

Eigen::Matrix3f testSetup::getIntMatrix() const {
  return rgb_intrinsics_matrix;
}

std::string testSetup::getSvmFile() const {
  return svm_file;
}

Eigen::VectorXf* testSetup::getGroundCoeffs() const {
  return groundCoeffs;
}

std::string testSetup::getReferPath() const {
  return referFile;
}

std::string testSetup::getResulPath() const {
  return resultFile;
}

float testSetup::getMaxDeviation() const {
  return maxDev;
}
