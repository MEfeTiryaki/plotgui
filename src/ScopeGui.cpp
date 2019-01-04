/*
 * ScopeGui.cpp
 *
 *  Created on: 27 Dec 2018
 *      Author: efe
 */

#include "ScopeGui.h"
#include "ui_plotgui.h"

ScopeGui::ScopeGui(QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::PlotGui),
      zoomFlag(false),
      moveFlag(false),
      stopFlag_(false),
      updateRate_(10),
      autoFitYAxis_(true),
      xAxisRange_(10),
      constantXAxisRange_(true),
      lastDataX_(0)

{
  ui_->setupUi(this);
  plot_ = ui_->customPlot;

  // Add Buttons
  // Save button
  saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::New);
  saveAct->setStatusTip(tr("Save a new file"));
  connect(saveAct, &QAction::triggered, this, &ScopeGui::savePlot);
  ui_->toolbar->addAction(saveAct);

  zoomAct = new QAction(tr("&Zoom"), this);
  zoomAct->setShortcuts(QKeySequence::New);
  zoomAct->setStatusTip(tr("Zoom"));
  connect(zoomAct, &QAction::triggered, this, &ScopeGui::zoomButtonPressed);
  ui_->toolbar->addAction(zoomAct);

  stopAct = new QAction(tr("&Stop"), this);
  stopAct->setShortcuts(QKeySequence::New);
  stopAct->setStatusTip(tr("Stop"));
  connect(stopAct, &QAction::triggered, this, &ScopeGui::stopButtonPressed);
  ui_->toolbar->addAction(stopAct);

  fitAct = new QAction(tr("&Fit"), this);
  fitAct->setShortcuts(QKeySequence::New);
  fitAct->setStatusTip(tr("Fit"));
  connect(fitAct, &QAction::triggered, this, &ScopeGui::fitButtonPressed);
  ui_->toolbar->addAction(fitAct);

  lockAct = new QAction(tr("&Lock"), this);
  lockAct->setShortcuts(QKeySequence::New);
  lockAct->setStatusTip(tr("Lock"));
  connect(lockAct, &QAction::triggered, this, &ScopeGui::lockButtonPressed);
  ui_->toolbar->addAction(lockAct);

  rangeEditLine_ = new QLineEdit(QString::number(xAxisRange_));
  rangeEditLine_->setFixedWidth(60);
  rangeEditLine_->setAlignment(Qt::AlignRight);
  ui_->toolbar->addWidget(rangeEditLine_);

  connect(plot_, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
  connect(rangeEditLine_, SIGNAL(editingFinished()), this, SLOT(rangeChanged()));
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(scopeUpdate()));
  dataTimer.start(updateRate_);
}

ScopeGui::~ScopeGui()
{
  delete ui_;
}

void ScopeGui::addGraph(){
  plot_->addGraph(); // blue line
  plot_->graph(plot_->graphCount()-1)->setPen(QPen(Qt::blue));
}

void ScopeGui::addGraph(QColor color){
  plot_->addGraph(); // blue line
  plot_->graph(plot_->graphCount()-1)->setPen(color);
}


void ScopeGui::scopeUpdate(){
  if(!stopFlag_){

    addData();

    if (autoFitYAxis_){
      for(int i=0; i<plot_->graphCount();i++){
        plot_->graph(i)->rescaleValueAxis();
      }
    }
    if(constantXAxisRange_){
      plot_->xAxis->setRange(lastDataX_, xAxisRange_, Qt::AlignRight);
    }else{
      plot_->xAxis->setRange(0.0, lastDataX_);
    }

    plot_->replot();
  }
}
void ScopeGui::setUpdateFunction(std::function<Eigen::VectorXd(double)> f){
  updateFunction_ = f;
}
void ScopeGui::addData(){
  lastDataX_ += 0.002;

  Eigen::VectorXd newData = updateFunction_(lastDataX_);
  for( int i = 0 ; i < plot_->graphCount() ; i++ ){
    plot_->graph(i)->addData(lastDataX_,newData(i));
    // qSin(lastDataX_) + qrand()/(double)RAND_MAX*1*qSin(lastDataX_/0.3843
  }
}


void ScopeGui::setColor(Qt::GlobalColor color)
{
  int plotNo = plot_->graphCount() - 1;
  plot_->graph(plotNo)->setPen(QPen(color));
}

void ScopeGui::setColor(Qt::GlobalColor color, int plotNo)
{
  int plotMax = plot_->graphCount() - 1;
  if (plotNo < plotMax) {
    plot_->graph(plotNo)->setPen(QPen(color));
  }
}

void ScopeGui::setAxis(double xMin,double xMax,double yMin,double yMax)
{
  plot_->xAxis->setRange(xMin, xMax);
  plot_->yAxis->setRange(yMin, yMax);
}

void ScopeGui::setXAxis(double xMin,double xMax)
{
  plot_->xAxis->setRange(xMin, xMax);
}

void ScopeGui::setYAxis(double yMin,double yMax)
{
  plot_->yAxis->setRange(yMin, yMax);
}

void ScopeGui::legend(std::vector<std::string> legendNames)
{
  plot_->legend->setVisible(true);
  for (int i = 0; i < legendNames.size(); i++) {
    plot_->graph(i)->setName(QString::fromStdString(legendNames[i]));
  }
}

void ScopeGui::mousePress(QMouseEvent* ev)
{
  //QCursor::pos() global position
  const QPoint p = ev->pos();
  cout << "Mouse pressed at " << p.x() << " , " << p.y() << " !!" << endl;

}

void ScopeGui::savePlot()
{
  dataTimer.stop();

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save plot"), "",
                                                  tr("png (*.png);jpeg (*.jpg);;All Files (*)"));

  plot_->savePng(fileName, plot_->size().width(), plot_->size().height(), 1.0);
  dataTimer.start();

}

void ScopeGui::zoomButtonPressed()
{
  moveFlag = false;
  zoomFlag = !zoomFlag;
  if (zoomFlag) {
    this->setCursor(Qt::CrossCursor);
  } else {
    this->setCursor(Qt::ArrowCursor);
  }
}

void ScopeGui::stopButtonPressed()
{
  stopFlag_ = !stopFlag_;
  if(stopFlag_){
    dataTimer.stop();
  }else{
    dataTimer.start();
  }
  if(stopFlag_){
      stopAct->setIconText(tr("Start"));
  }else{
      stopAct->setIconText(tr("Stop"));
  }
}

void ScopeGui::fitButtonPressed()
{
  autoFitYAxis_ = !autoFitYAxis_;
  if(autoFitYAxis_){
      fitAct->setIconText(tr("Not Fit"));
  }else{
      fitAct->setIconText(tr("Fit"));
  }
}

void ScopeGui::lockButtonPressed(){
  constantXAxisRange_ = !constantXAxisRange_;
  if(constantXAxisRange_){
      lockAct->setIconText(tr("Unlock"));
  }else{
      lockAct->setIconText(tr("Lock"));
  }
}

void ScopeGui::rangeChanged(){
  if (!rangeEditLine_->text().isEmpty()){
    bool isNumber;
    double newRange = rangeEditLine_->text().toDouble(&isNumber);
    if (isNumber && newRange > 0.0 ){
      xAxisRange_ = newRange ;
    }
    return;
  }
  rangeEditLine_->setText(QString::number(xAxisRange_));
}
