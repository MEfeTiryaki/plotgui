/*
 * PlotGui.h
 *
 *  Created on: 27 Dec 2018
 *      Author: efe
 */

#ifndef PLOTGUI_H_
#define PLOTGUI_H_
#pragma once

#include <iostream>
#include <Eigen/Core>
#include <QMainWindow>
#include <QApplication>
#include <qcustomplot.h>

using namespace std;
namespace Ui {
class PlotGui;
}

class PlotGui : public QMainWindow
{
  Q_OBJECT
 public:
  explicit PlotGui(QWidget *parent = nullptr);

  ~PlotGui();

 public:
  void plot(Eigen::VectorXd x, Eigen::VectorXd y);
  void plot(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig, Qt::GlobalColor color);

  void scatter(Eigen::VectorXd x, Eigen::VectorXd y);
  void scatter(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig, Qt::GlobalColor color);

  void fillBelow(QColor color);

  void subplot(int v,int h,int index);

  void holdOn();
  void holdOff();

  void gridOn();
  void gridOff();

  void gridMinorOn();
  void gridMinorOff();

  void xLogScaleOn();
  void xLogScaleOff();

  void yLogScaleOn();
  void yLogScaleOff();

  void xLabel(std::string l);
  void xLabel(std::string l,int fontSize);
  void yLabel(std::string l);
  void yLabel(std::string l,int fontSize);
  void legend(std::vector<string> legendNames);
  void legend(std::vector<string> legendNames, int fontSize);
  void xTickSize(int fontSize);
  void yTickSize(int fontSize);

  void setColor(Qt::GlobalColor color);
  void setColor(Qt::GlobalColor color, int plotNo);

  void setAxis(double xMin,double xMax,double yMin,double yMax);
  void setXAxis(double xMin,double xMax);
  void setYAxis(double yMin,double yMax);


  void makeTestPlot(double k );
private slots:

  void mousePress(QMouseEvent* ev);
  void mouseDoubleClick(QMouseEvent* ev);
  void savePlot();
  void zoomButtonPressed();
  void moveButtonPressed();
 private:

  Ui::PlotGui *ui_;
  QCustomPlot *plot_;
  QLabel *cursorPointLabel_;
  QAction *saveAct_;
  QAction *zoomAct_;
  QAction *moveAct_;

  int lastEditedAxis_;
  std::vector<QCPMarginGroup*> verticalMarginGroups_;
  std::vector<QCPMarginGroup*> horizontalMarginGroups_;
  std::vector<QCPAxisRect*> axisRects_;
  std::vector<QCPLegend*> axisLegends_;
  std::vector<std::vector<QCPGraph*>> axisGraphs_;
  std::vector<int> lastEditedGraphes_;
  std::vector<bool> holdOns_;
  int verticalAxesNumber_;
  int horizontalAxesNumber_;

  bool zoomFlag_;
  bool moveFlag_;
  bool holdOn_;

};

#endif /* PLOTGUI_H_ */
