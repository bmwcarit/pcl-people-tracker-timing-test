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
#include <testUtil.h>
#include <sched.h>


testSetup::testSetup(int argc, char *argv[]) {
  testSetup::setInputs(argc, argv);

  testSetup::setSchedule(std::string(argv[17]));
}

void testSetup::setInputs(int argc, char *argv[]) {
  if ((argc == 18) || (argc == 19)) {
    bool minc_set = false;
    bool minh_set = false;
    bool maxh_set = false;
    bool minw_set = false;
    bool maxw_set = false;
    bool vs_set   = false;
    bool sf_set   = false;

    // read TestConfig.txt
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

    if (argc == 18) {
      outputFile = result_dir + "/" + bag_name + "_Report_" + ".txt";
      resultFile = result_dir + "/" + bag_name + "_Result_" + ".txt";
    } else if (argc == 19) {
      outputFile = result_dir + "/" + bag_name + "_Report_" + std::string(argv[18]) + ".txt";
      resultFile = result_dir + "/" + bag_name + "_Result_" + std::string(argv[18]) + ".txt";
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

void testSetup::getResultHeader(std::stringstream& resultHeader) {
  resultHeader << "# --- Parameterlist ---" << std::endl;
  resultHeader << "# minc  : " << min_confidence << std::endl;
  resultHeader << "# minh  : " << min_height << std::endl;
  resultHeader << "# maxh  : " << max_height << std::endl;
  resultHeader << "# minw  : " << min_width << std::endl;
  resultHeader << "# maxw  : " << max_width << std::endl;
  resultHeader << "# vs    : " << voxel_size << std::endl;
  resultHeader << "# sf    : " << sampling_factor << std::endl;
}

void testSetup::initEvaluation(std::string& outputPath, std::string& referPath, std::string& resultPath, float& minConf, float& maxDeviation, std::stringstream& resultHeader) {
  outputPath = outputFile;
  referPath = referFile;
  resultPath = resultFile;
  minConf = min_confidence;
  maxDeviation = maxDev;
  testSetup::getResultHeader(resultHeader);
}

void testSetup::initPeopleDetector(peopleDetectorType& PD) {
  person_classifier.loadSVMFromFile(svm_file);

  PD.setIntrinsics(rgb_intrinsics_matrix);
  PD.setVoxelSize(voxel_size);
  PD.setClassifier(person_classifier);
  PD.setPersonClusterLimits(min_height, max_height, min_width, max_width);
  PD.setSamplingFactor(sampling_factor);
  PD.setGround(*groundCoeffs);
}

void testSetup::initExecution(peopleDetectorType& PD, std::string& inputPath, std::string& topic) {
  testSetup::initPeopleDetector(PD);
  inputPath = inputFile;
  topic = topicName;
}

void testSetup::setSchedule(std::string policy) {
  struct sched_param schedParam;
  const int sched_priority = 95;
  schedParam.sched_priority = sched_priority;
  int sched_policy = 0;

  if (policy.compare("NO_RT") == 0) {
    testUtil::writeInfo("No RT scheduling selected.");
  } else {
    if (policy.compare("FIFO") == 0) {
      sched_policy = SCHED_FIFO;
    } else if (policy.compare("RR") == 0) {
      sched_policy = SCHED_RR;
    } else {
      testUtil::writeError("Wrong input for scheduling policy. Available options : FIFO | RR | NO_RT");
    }

    if (sched_setscheduler(0, sched_policy, &schedParam) != 0) {
      perror("Failed to set policy");
      testUtil::writeError("Test aborted!");
    } else {
      testUtil::writeInfo(policy + " policy set.");
    }
  }
}
