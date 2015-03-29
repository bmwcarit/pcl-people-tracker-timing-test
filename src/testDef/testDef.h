/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#ifndef SRC_TESTDEF_TESTDEF_H_
#define SRC_TESTDEF_TESTDEF_H_

#include <pcl/point_types.h>
#include <time.h>
#include <pcl/people/ground_based_people_detection_app.h>
#include <utility>
#include <vector>

typedef pcl::PointXYZRGB SourcePointType;
typedef pcl::PointCloud<SourcePointType> SourcePointCloud;
typedef pcl::people::PersonCluster<SourcePointType> personClusterType;
typedef std::vector<personClusterType> clustersVerctorType;
typedef std::pair <timespec, clustersVerctorType*> frameResultType;
typedef std::vector <frameResultType> testResultType;
typedef pcl::people::GroundBasedPeopleDetectionApp<SourcePointType> peopleDetectorType;

typedef float xPosType;
typedef float zPosType;
typedef std::pair<std::pair<xPosType, zPosType>, bool> referEntryType;
typedef std::vector<referEntryType> referFrameType;
typedef std::vector<referFrameType> referResultType;

typedef struct {
  int frameNumber;
  float execTime;
  int falsePos;
  int falseNeg;
  int match;
  float matchDev;
}frameCompareType;

typedef std::vector<frameCompareType> compareResultType;

#endif  // SRC_TESTDEF_TESTDEF_H_
