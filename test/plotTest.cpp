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

#include "plotgui/PlotGui.h"

using namespace std;

int main()
{

  int argc;
  char **argv;
  QApplication app(argc, argv);
  PlotGui gui;

  gui.makeTestPlot(1);
  gui.holdOn();
  gui.makeTestPlot(0.5);

  std::vector<std::string> legendNames(1);
  legendNames[0] = "asd";
  gui.legend(legendNames);
  gui.show();

  return app.exec();
}
