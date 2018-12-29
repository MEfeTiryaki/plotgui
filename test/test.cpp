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

#include "PlotGui.h"

using namespace std;

int main()
{

  int argc;
  char **argv;
  QApplication app(argc, argv);
  PlotGui gui;

  gui.makeTestPlot();
  gui.show();

  return app.exec();
}
