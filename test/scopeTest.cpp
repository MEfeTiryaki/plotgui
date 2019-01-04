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

using namespace std::placeholders;
using namespace std;


Eigen::VectorXd f(double t){
  Eigen::VectorXd x = Eigen::VectorXd::Zero(1);
  x[0] = sin(t);
  return x;
}
int main()
{

  int argc;
  char **argv;
  QApplication app(argc, argv);
  ScopeGui gui;

  gui.setUpdateFunction(std::bind(&f, _1));
  gui.addGraph();
  gui.show();
  return app.exec()  ;
}
