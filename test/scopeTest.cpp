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

#include "plotgui/ScopeGui.h"

using namespace std::placeholders;
using namespace std;


std::vector<Eigen::VectorXd> f(double t){
  std::vector<Eigen::VectorXd> x ;
  x.push_back(Eigen::VectorXd::Zero(2));
  x.back()[0] = sin(t);
  x.back()[1] = t;
  return x;
}
int main()
{

  int argc;
  char **argv;
  QApplication app(argc, argv);
  ScopeGui gui;

  gui.setScopeNumber(1);
  //gui.addTimeSignal(std::bind(&f, -1));
  //gui.addTimeSignal(std::bind(&f, -1));
  //gui.addGraph(0, std::bind(&f, 0));
  gui.show();
  return app.exec()  ;
}
