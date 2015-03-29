/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#ifndef SRC_TESTUTIL_TESTUTIL_H_
#define SRC_TESTUTIL_TESTUTIL_H_

#include <string>

class testUtil {
 public:
  static void writeError(std::string error);
  static void writeInfo(std::string info);
  static std::string timeStamp();

 private:
  testUtil();
  ~testUtil();
};
#endif  // SRC_TESTUTIL_TESTUTIL_H_
