/**
* Copyright (c) 2015, BMW Car IT GmbH
* Author: Michael Mühlbauer-Prassek
*
* This software is licensed under BSD 3-clause License
* (see http://spdx.org/licenses/BSD-3-Clause).
**/

#include <testSetup.h>
#include <testExecution.h>
#include <testEval.h>

int main(int argc, char *argv[]) {
  testSetup TS;
  TS.setInputs(argc, argv);

  testExecution TE(TS);

  testResultType* tr = TE.performTest();

  testEval TEV(tr, TS);

  TEV.performEvaluation();

  delete tr;
}
