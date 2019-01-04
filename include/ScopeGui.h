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

class ScopeGui : public QMainWindow
{
  Q_OBJECT
 public:
  explicit ScopeGui(QWidget *parent = nullptr);

  ~ScopeGui();

 public:
  void setUpdateFunction(std::function<Eigen::VectorXd(double)> f);
  void addGraph();
  void addGraph(QColor color);

  void setColor(Qt::GlobalColor color);
  void setColor(Qt::GlobalColor color, int plotNo);


  void setAxis(double xMin,double xMax,double yMin,double yMax);
  void setXAxis(double xMin,double xMax);
  void setYAxis(double yMin,double yMax);

  void legend(std::vector<string> legendNames);

protected:
  virtual void addData();

private slots:
  void scopeUpdate();
  void mousePress(QMouseEvent* ev);
  void savePlot();
  void zoomButtonPressed();
  void stopButtonPressed();
  void fitButtonPressed();
  void lockButtonPressed();
  void rangeChanged();
 private:

  Ui::PlotGui *ui_;
  QCustomPlot *plot_;
  QTimer dataTimer;
  QAction * saveAct ;
  QAction *zoomAct ;
  QAction *stopAct ;
  QAction *fitAct ;
  QAction *lockAct ;
  QLineEdit *rangeEditLine_;

  std::function<Eigen::VectorXd(double)> updateFunction_;

  int updateRate_;

  double xAxisRange_ ;
  bool constantXAxisRange_;
  double lastDataX_;

  bool autoFitYAxis_;

  bool zoomFlag;
  bool moveFlag;
  bool stopFlag_;

};

#endif /* PLOTGUI_H_ */
