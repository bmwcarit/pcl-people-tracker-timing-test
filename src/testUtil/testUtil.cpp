/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#include <testUtil.h>
#include <iostream>
#include <stdexcept>
#include <sstream>

void testUtil::writeError(std::string error) {
  throw std::runtime_error(error);
}

void testUtil::writeInfo(std::string info) {
  std::cout << info << std::endl;
}

std::string testUtil::timeStamp() {
  time_t t;
  struct tm * ts;
  t = time(NULL);
  ts = localtime(&t);
  std::stringstream tss(asctime(ts));
  return tss.str();
}
