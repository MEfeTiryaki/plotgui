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
#include <boost/thread/thread.hpp>

#include <QMainWindow>
#include <QApplication>
#include <plotgui/qcustomplot.h>

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
  void setScopeNumber(int N);
  void addTimeSignal(std::function<std::vector<double>()> f);
  void addGraph(int axisNo, std::function<std::vector<double>()> f);
  void addStopFunction(std::function<void(bool)> f);
  void addClearFunction(std::function<void()> f);

  void stopGui(bool stop);

  void setColor(Qt::GlobalColor color);
  void setColor(Qt::GlobalColor color, int plotNo);

  void setXAxisRange(double range);

  void setAxis(double xMin,double xMax,double yMin,double yMax);
  void setXAxis(double xMin,double xMax);
  void setYAxis(double yMin,double yMax);

  void legend(int axisNo ,std::vector<std::string> legendNames, int fontSize);
  void legend(int axisNo ,std::vector<std::string> legendNames);

protected:
  virtual void addData();

private slots:
  void scopeUpdate();
  void stop(bool stop);

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
  QLabel *cursorPointLabel_;

  std::function<std::vector<double>()> timeFunction_;
  std::vector<std::vector<std::function<std::vector<double>()> > > updateFunctions_;
  std::vector<std::function<void(bool)>> stopFunctions_;
  std::function<void()> clearFunction_;
  int updateRate_;
  boost::mutex addDataMutex_;

  double xAxisRange_ ;
  double yAxisMin_  ;
  double yAxisMax_  ;
  bool constantXAxisRange_;
  double lastDataX_;

  std::vector<QCPAxisRect*> axisRects_;
  std::vector<QCPLegend*> axisLegends_;
  std::vector<std::vector<QCPGraph*>> axisGraphs_;
  QCPMarginGroup* verticalMarginGroup_;

  bool autoFitYAxis_;

  bool zoomFlag;
  bool moveFlag;
  bool stopFlag_;

};

#endif /* PLOTGUI_H_ */
