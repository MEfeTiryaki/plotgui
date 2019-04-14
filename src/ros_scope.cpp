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

#include "RosScope.hpp"

using namespace std;

int main()
{
  int argc;
  char **argv;
  QApplication app(argc, argv);


  RosScope scope("ros_scope");
  scope.create();
  scope.initialize();
  scope.start();


  return app.exec()  ;
}
