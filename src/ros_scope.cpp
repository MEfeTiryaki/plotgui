//============================================================================
// Name        : mri_sim.cpp
// Author      : Mehmet Efe Tiryaki
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <Eigen/Core>

#include "ScopeGui.h"
#include "RosScope.hpp"

using namespace std::placeholders;
using namespace std;

int main()
{
  int argc;
  char **argv;
  QApplication app(argc, argv);
  ScopeGui gui;

  gui.show();
  return app.exec()  ;
}
