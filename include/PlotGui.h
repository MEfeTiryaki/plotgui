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

  void holdOn();
  void holdOff();

  void setColor(Qt::GlobalColor color);
  void setColor(Qt::GlobalColor color, int plotNo);


  void setAxis(double xMin,double xMax,double yMin,double yMax);
  void setXAxis(double xMin,double xMax);
  void setYAxis(double yMin,double yMax);

  void legend(std::vector<string> legendNames);

  void makeTestPlot();
private slots:

  void mousePress(QMouseEvent* ev);
  void savePlot();
  void zoomButtonPressed();
  void moveButtonPressed();
 private:
  Ui::PlotGui *ui_;
  QCustomPlot *plot_;

  bool zoomFlag;
  bool moveFlag;
  bool holdOn_;

};

#endif /* PLOTGUI_H_ */
