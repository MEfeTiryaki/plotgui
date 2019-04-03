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

  gui.subplot(2,2,0);
  gui.xLabel("x");
  gui.yLabel("y");
  gui.makeTestPlot(1);

  gui.holdOn();

  gui.makeTestPlot(0.5);

  std::vector<std::string> legendNames(1);
  legendNames[0] = "asd";
  gui.legend(legendNames,20);

  gui.subplot(2,2,3);
  gui.xLabel("x");
  gui.yLabel("y");
  gui.setAxis(-2,2,-1,1);
  gui.makeTestPlot(2);
  gui.fillBelow(QColor(0,0,255,125));
  //gui.gridOff();
  legendNames[0] = "rte";
  gui.legend(legendNames);

  gui.show();

  return app.exec();
}
