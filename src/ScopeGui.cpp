/*
 * ScopeGui.cpp
 *
 *  Created on: 27 Dec 2018
 *      Author: efe
 */

#include "plotgui/ScopeGui.h"
#include "ui_plotgui.h"

ScopeGui::ScopeGui(QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::PlotGui),
      zoomFlag(false),
      moveFlag(false),
      stopFlag_(false),
      updateRate_(0),
      autoFitYAxis_(true),
      xAxisRange_(1.0),
      yAxisMin_(0.0),
      yAxisMax_(1.0),
      constantXAxisRange_(false),
      lastDataX_(0)
{
  ui_->setupUi(this);
  plot_ = ui_->customPlot;
  plot_->setAutoAddPlottableToLegend(false);

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

  cursorPointLabel_ = new QLabel("(0.0 , 0.0)");
  cursorPointLabel_->setFixedWidth(200);
  cursorPointLabel_->setAlignment(Qt::AlignCenter);
  ui_->toolbar->addWidget(cursorPointLabel_);

  connect(plot_, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
  connect(rangeEditLine_, SIGNAL(editingFinished()), this, SLOT(rangeChanged()));
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(scopeUpdate()));
  dataTimer.start(updateRate_);
}

ScopeGui::~ScopeGui()
{
  delete ui_;
}


void ScopeGui::setScopeNumber(int N){

  if (N<0){
    std::cout<<"Error : Scope number can not be greater than 0 !" << std::endl;
    return;
  }
  verticalMarginGroup_ = new QCPMarginGroup(plot_);
  axisRects_.clear();
  axisGraphs_.clear();
  axisLegends_.clear();
  plot_->plotLayout()->clear();
  updateFunctions_ = std::vector<std::vector< std::function<std::vector<double>() >>>(0);
  for(int i = 0; i<N; i++){
    axisRects_.push_back(new QCPAxisRect(plot_));
    axisRects_.back()->setupFullAxesBox(true);
    axisLegends_.push_back(new QCPLegend());
    axisRects_.back()->insetLayout()->addElement(
         axisLegends_.back(), Qt::AlignTop|Qt::AlignRight);
    axisLegends_.back()->setLayer("legend");

    axisGraphs_.push_back(std::vector<QCPGraph*>(0));

    plot_->plotLayout()->addElement(i,0,axisRects_.back());
    axisRects_.back()->setMarginGroup(QCP::msLeft, verticalMarginGroup_);

    updateFunctions_.push_back(std::vector<std::function<std::vector<double>()>>(0));
  }
  for(auto axis: axisRects_)
  {
    for(auto a:axis->axes()){
      a->setLayer("axes");
      a->grid()->setLayer("grid");
    }
  }
}

void ScopeGui::addGraph(int axisNo, std::function<std::vector<double>()> f){
  axisGraphs_[axisNo].push_back(plot_->addGraph(
         axisRects_[axisNo]->axis(QCPAxis::atBottom)
       , axisRects_[axisNo]->axis(QCPAxis::atLeft)));
  updateFunctions_[axisNo].push_back(f);

}

void ScopeGui::addStopFunction(std::function<void(bool)> f){
  stopFunctions_.push_back( f);
}
void ScopeGui::addClearFunction(std::function<void()> f){
  clearFunction_ = f ;
}
void ScopeGui::addTimeSignal(std::function<std::vector<double>()> f){
  timeFunction_ = f ;
}


void ScopeGui::scopeUpdate(){
  if(!stopFlag_){
    addData();
    if (autoFitYAxis_){
      for(auto graphs : axisGraphs_){
        for(auto graph :graphs ){
          graph->rescaleValueAxis();
        }
      }
    }
    else{
      for(auto graphs : axisGraphs_){
        for(auto graph :graphs ){
          graph->valueAxis()->setRange(yAxisMin_, yAxisMax_);
        }
      }
    }
    if(constantXAxisRange_){
      if (lastDataX_<xAxisRange_){
        for(auto graphs : axisGraphs_){
          for(auto graph :graphs ){
            graph->keyAxis()->setRange(xAxisRange_, xAxisRange_, Qt::AlignRight);
          }
        }
      }else{
        for(auto graphs : axisGraphs_){
          for(auto graph :graphs ){
            graph->keyAxis()->setRange(lastDataX_, xAxisRange_, Qt::AlignRight);
          }
        }
      }
    }else{
      for(auto graphs : axisGraphs_){
        for(auto graph :graphs ){
          graph->keyAxis()->setRange(0.0, lastDataX_);
        }
      }
    }
    plot_->replot();
  }
}


void ScopeGui::stopGui(bool stopFlag){
  stopFlag_ = stopFlag ;
  if(stopFlag_){
    stopAct->setIconText(tr("Start"));
    stop(true);
  }else{
    stopAct->setIconText(tr("Stop"));
    stop(false);
  }
}

void ScopeGui::stop(bool stop){
  for(auto f :stopFunctions_){
    f(stop);
  }
}

void ScopeGui::addData(){
  boost::lock_guard<boost::mutex> lock(addDataMutex_);
  stop(true);
  std::vector<double> t = timeFunction_();
  if( t.size() != 0 ){
    lastDataX_ = t.back();
    std::vector<double> newData;
    for(int axisNo = 0 ; axisNo<axisRects_.size() ; axisNo++ ){
      for(int graphNo = 0 ; graphNo<axisGraphs_[axisNo].size(); graphNo++ ){
        newData = updateFunctions_[axisNo][graphNo]();
        for( int i = 0 ; i < t.size() ; i++){
          axisGraphs_[axisNo][graphNo]->addData(t[i],newData[i]);
        }
      }
    }
    clearFunction_();
  }
  stop(false);
}


void ScopeGui::setColor(Qt::GlobalColor color)
{
  // TODO : Correct this
  //int plotNo = plot_->graphCount() - 1;
  //plot_->graph(plotNo)->setPen(QPen(color));
}

void ScopeGui::setColor(Qt::GlobalColor color, int plotNo)
{
  // TODO : Correct this
  //int plotMax = plot_->graphCount() - 1;
  //if (plotNo < plotMax) {
  //  plot_->graph(plotNo)->setPen(QPen(color));
  //}
}


void ScopeGui::setXAxisRange(double range){
    xAxisRange_ = range ;
    rangeEditLine_->setText(QString::number(xAxisRange_));
}

void ScopeGui::setAxis(double xMin,double xMax,double yMin,double yMax)
{
  // TODO : Correct this
  //plot_->xAxis->setRange(xMin, xMax);
  //plot_->yAxis->setRange(yMin, yMax);
}

void ScopeGui::setXAxis(double xMin,double xMax)
{
  // TODO : Correct this
  //plot_->xAxis->setRange(xMin, xMax);
}

void ScopeGui::setYAxis(double yMin,double yMax)
{
  yAxisMin_ = yMin ;
  yAxisMax_ = yMax ;
}

void ScopeGui::legend(int axisNo, std::vector<std::string> legendNames)
{
    legend(axisNo,legendNames,15);
}

void ScopeGui::legend(int axisNo ,std::vector<std::string> legendNames, int fontSize)
{
  axisLegends_[axisNo]->clearItems();
  for(int i = 0; i < legendNames.size(); i++){
    axisLegends_[axisNo]->addItem(
            new QCPPlottableLegendItem(axisLegends_[axisNo]
          , axisGraphs_[axisNo][i] ));
  }
  axisLegends_[axisNo]->setVisible(true);
  for (int i = 0; i < legendNames.size(); i++) {
    axisGraphs_[axisNo][i]->setName(QString::fromStdString(legendNames[i]));
  }
  QFont font;
  font.setPixelSize(fontSize);
  axisLegends_[axisNo]->setFont(font);
}


void ScopeGui::mousePress(QMouseEvent* ev)
{
  //QCursor::pos() global position
  const QPoint p = ev->pos();
  double xValue = -1.0;
  double yValue = -1.0;
  int rectNo = -1 ;
  for(int i=0; i<axisRects_.size();i++ ){
    auto rect = axisRects_[i];
    if( rect->left()<p.x() && rect->right()>p.x()
      &&rect->bottom()>p.y() && rect->top()<p.y()){
      rectNo = i;
      break;
    }
  }
  if(ev->button() == Qt::LeftButton){
    if(rectNo!=-1){
      double xLowerValue = axisGraphs_[rectNo].back()->keyAxis()->range().lower;
      double xUpperValue = axisGraphs_[rectNo].back()->keyAxis()->range().upper;
      double yLowerValue = axisGraphs_[rectNo].back()->valueAxis()->range().lower;
      double yUpperValue = axisGraphs_[rectNo].back()->valueAxis()->range().upper;
      double xRatio = (double)(p.x()-axisRects_[rectNo]->left())
                    / (axisRects_[rectNo]->right()-axisRects_[rectNo]->left());
      double yRatio = (double)(-p.y()+axisRects_[rectNo]->bottom())
                    / (axisRects_[rectNo]->bottom()-axisRects_[rectNo]->top());

      xValue = xLowerValue + xRatio*(xUpperValue-xLowerValue);
      yValue = yLowerValue + yRatio*(yUpperValue-yLowerValue);
      QString pointText = QString("");
      pointText.append( QString::fromStdString("(" ));
      pointText.append(QString::number(xValue));
      pointText.append( QString::fromStdString(" , " ));
      pointText.append(QString::number(yValue));
      pointText.append( QString::fromStdString(")" ));
      cursorPointLabel_->setText(pointText);
    }
  }

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
    stopAct->setIconText(tr("Start"));
    stop(true);
  }else{
    dataTimer.start();
    stopAct->setIconText(tr("Stop"));
    stop(false);
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
