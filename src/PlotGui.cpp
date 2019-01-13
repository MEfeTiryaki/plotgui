/*
 * PlotGui.cpp
 *
 *  Created on: 27 Dec 2018
 *      Author: efe
 */

#include "plotgui/PlotGui.h"
#include "ui_plotgui.h"

PlotGui::PlotGui(QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::PlotGui),
      zoomFlag_(false),
      moveFlag_(false),
      holdOn_(false),
      verticalAxesNumber_(0),
      horizontalAxesNumber_(0),
      axisRects_(0),
      lastEditedGraphes_(0),
      holdOns_(0)

{
  ui_->setupUi(this);
  plot_ = ui_->customPlot;
  plot_->setAutoAddPlottableToLegend(false);
  subplot(1,1,0);

  // Add Buttons
  // Save button
  saveAct_ = new QAction(tr("&Save"), this);
  saveAct_->setShortcuts(QKeySequence::New);
  saveAct_->setStatusTip(tr("Save a new file"));
  connect(saveAct_, &QAction::triggered, this, &PlotGui::savePlot);
  ui_->toolbar->addAction(saveAct_);

  zoomAct_ = new QAction(tr("&Zoom"), this);
  zoomAct_->setShortcuts(QKeySequence::New);
  zoomAct_->setStatusTip(tr("Zoom"));
  connect(zoomAct_, &QAction::triggered, this, &PlotGui::zoomButtonPressed);
  ui_->toolbar->addAction(zoomAct_);

  moveAct_ = new QAction(tr("&Move"), this);
  moveAct_->setShortcuts(QKeySequence::New);
  moveAct_->setStatusTip(tr("Move"));
  connect(moveAct_, &QAction::triggered, this, &PlotGui::moveButtonPressed);
  ui_->toolbar->addAction(moveAct_);


  cursorPointLabel_ = new QLabel("(0.0 , 0.0)");
  cursorPointLabel_->setFixedWidth(200);
  cursorPointLabel_->setAlignment(Qt::AlignCenter);
  ui_->toolbar->addWidget(cursorPointLabel_);

  connect(plot_, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
  connect(plot_, SIGNAL(mouseDoubleClick(QMouseEvent*)), this, SLOT(mouseDoubleClick(QMouseEvent*)));
  //connect(plot_, SIGNAL(mouseMove(QWheelEvent*)), this, SLOT(wheelEvent(QWheelEvent*)));


}

PlotGui::~PlotGui()
{
  delete ui_;
}


void PlotGui::plot(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig)
{
  plot(x_eig, y_eig, Qt::blue);
}

void PlotGui::plot(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig, Qt::GlobalColor color)
{
  QVector<double> x(x_eig.size());
  QVector<double> y(y_eig.size());
  for (int i = 0; i < x.size(); ++i) {
    x[i] = x_eig[i];
    y[i] = y_eig[i];
  }


  int graphIndex = 0 ;
  if (!holdOns_[lastEditedAxis_]) {
    // Remove all graph from plot
    for(auto g : axisGraphs_[lastEditedAxis_]){
      plot_->removeGraph(g);
    }
    axisGraphs_[lastEditedAxis_].clear();
    axisGraphs_[lastEditedAxis_].push_back(
        plot_->addGraph( axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)
                      , axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)));
    axisGraphs_[lastEditedAxis_].back()->keyAxis()->setRange(x_eig.minCoeff()
                                                       ,x_eig.maxCoeff());
    axisGraphs_[lastEditedAxis_].back()->valueAxis()->setRange(y_eig.minCoeff()
                                                       ,y_eig.maxCoeff());
  } else {
    graphIndex = axisGraphs_[lastEditedAxis_].size() - 1;
    axisGraphs_[lastEditedAxis_].push_back(
        plot_->addGraph( axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)
                      , axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)));
  }

  lastEditedGraphes_[lastEditedAxis_] = graphIndex;
  axisGraphs_[lastEditedAxis_].back()->setData(x,y);
  axisGraphs_[lastEditedAxis_].back()->setPen(QPen(color));
  plot_->replot();

}

void PlotGui::plot(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig, int lineWidth){
  plot(x_eig, y_eig);
  QPen pen = axisGraphs_[lastEditedAxis_].back()->pen();
  pen.setWidth(lineWidth);
  axisGraphs_[lastEditedAxis_].back()->setPen(pen);
}
void PlotGui::plot(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig, Qt::GlobalColor color ,int lineWidth){
  plot(x_eig, y_eig, color);
  QPen pen = QPen(color);
  pen.setWidth(lineWidth);
  axisGraphs_[lastEditedAxis_].back()->setPen(pen);
}


void PlotGui::scatter(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig)
{
  scatter(x_eig, y_eig, Qt::blue);
}

void PlotGui::scatter(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig, Qt::GlobalColor color)
{
  plot(x_eig, y_eig, color);
  axisGraphs_[lastEditedAxis_].back()->setLineStyle(QCPGraph::lsNone);
  axisGraphs_[lastEditedAxis_].back()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));
}


void PlotGui::makeTestPlot(double k )
{
  // generate some data:
  int size = 101;
  Eigen::VectorXd x = Eigen::VectorXd::Zero(size);
  Eigen::VectorXd y = Eigen::VectorXd::Zero(size);
  for (int i = 0; i < size; ++i) {
    x[i] = i / 50.0 - 1;  // x goes from -1 to 1
    y[i] = k * x[i] * x[i];  // let's plot a quadratic function
    //std::cout<<k <<","<<x[i] <<","<<y[i] <<","<<std::endl;
  }
  plot(x,y,5);

}

void PlotGui::fillBelow(QColor color){
  axisGraphs_[lastEditedAxis_].back()->setBrush(QBrush(color));
}

void PlotGui::subplot(int v,int h,int index){

  if (index>v*h-1){
    std::cout<<"Error : Subplot index can not be greater than " << v*h << "!"<< std::endl;
    return;
  }
  if (index<0){
    std::cout<<"Error : Subplot index can not be greater than 0 !" << std::endl;
    return;
  }
  if(!(verticalAxesNumber_== v && horizontalAxesNumber_==h)){
    verticalAxesNumber_ = v;
    horizontalAxesNumber_ = h;
    verticalMarginGroups_.clear();
    axisRects_.clear();
    axisGraphs_.clear();
    axisLegends_.clear();
    lastEditedGraphes_.clear();
    holdOns_.clear();
    plot_->plotLayout()->clear();

    for(int i = 0; i<verticalAxesNumber_; i++){
      horizontalMarginGroups_.push_back(new QCPMarginGroup(plot_));
    }
    for(int j = 0; j<horizontalAxesNumber_; j++){
      verticalMarginGroups_.push_back(new QCPMarginGroup(plot_));
    }

    lastEditedAxis_ = 0 ;
    for(int i = 0; i<verticalAxesNumber_; i++){
      for(int j = 0; j<horizontalAxesNumber_; j++){
        axisRects_.push_back(new QCPAxisRect(plot_));
        axisRects_.back()->setupFullAxesBox(true);
        axisRects_.back()->setRangeDrag(Qt::Vertical | Qt::Horizontal);
        axisLegends_.push_back(new QCPLegend());
        axisRects_.back()->insetLayout()->addElement(
             axisLegends_.back(), Qt::AlignTop|Qt::AlignRight);
        axisLegends_.back()->setLayer("legend");

        axisGraphs_.push_back(std::vector<QCPGraph*>(0));
        lastEditedGraphes_.push_back(0);
        axisGraphs_[lastEditedAxis_].push_back(plot_->addGraph(
                  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)
                , axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)));
        holdOns_.push_back(false);
        plot_->plotLayout()->addElement(i,j,axisRects_.back());
        axisRects_.back()->setMarginGroup(QCP::msLeft, verticalMarginGroups_[j]);
        axisRects_.back()->setMarginGroup(QCP::msBottom, horizontalMarginGroups_[i]);

        axisRects_.back()->setRangeZoomAxes(axisGraphs_[lastEditedAxis_].back()->keyAxis(),
                                        axisGraphs_[lastEditedAxis_].back()->valueAxis());
        lastEditedAxis_++;
      }
    }
    for(auto axis: axisRects_)
    {
      for(auto a:axis->axes()){
        a->setLayer("axes");
        a->grid()->setLayer("grid");
      }
    }
  }

  lastEditedAxis_ = index ;

}


void PlotGui::holdOn()
{
  holdOns_[lastEditedAxis_] = true;
}

void PlotGui::holdOff()
{
  holdOns_[lastEditedAxis_] = false;
}

void PlotGui::gridOn()
{
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->setVisible(true);
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->setVisible(true);
}

void PlotGui::gridOff()
{
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->setVisible(false);
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->setVisible(false);
}


void PlotGui::gridMinorOn()
{
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->grid()->setSubGridVisible(true);
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->grid()->setSubGridVisible(true);
}


void PlotGui::gridMinorOff()
{
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->grid()->setSubGridVisible(false);
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->grid()->setSubGridVisible(false);
}

void PlotGui::xLogScaleOn(){
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->setScaleType(QCPAxis::stLogarithmic);
}
void PlotGui::xLogScaleOff(){
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->setScaleType(QCPAxis::stLinear);
}

void PlotGui::yLogScaleOn(){
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->setScaleType(QCPAxis::stLogarithmic);
}
void PlotGui::yLogScaleOff(){
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->setScaleType(QCPAxis::stLinear);
}


void PlotGui::xLabel(std::string l){
  xLabel(l, 15);
}
void PlotGui::xLabel(std::string l, int fontSize){
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->setLabel(QString::fromStdString(l));
  QFont font;
  font.setPixelSize(fontSize);
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->setLabelFont(font);
}
void PlotGui::yLabel(std::string l){
  yLabel(l, 15);
}
void PlotGui::yLabel(std::string l, int fontSize){
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->setLabel(QString::fromStdString(l));
  QFont font;
  font.setPixelSize(fontSize);
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->setLabelFont(font);
}

void PlotGui::legend(std::vector<std::string> legendNames)
{
    legend(legendNames,15);
}

void PlotGui::legend(std::vector<std::string> legendNames, int fontSize)
{
  axisLegends_[lastEditedAxis_]->clearItems();

  for(int i = 0; i < legendNames.size(); i++){
    axisLegends_[lastEditedAxis_]->addItem(
        new QCPPlottableLegendItem(axisLegends_[lastEditedAxis_]
          ,axisGraphs_[lastEditedAxis_][i] ));
  }
  axisLegends_[lastEditedAxis_]->setVisible(true);
  for (int i = 0; i < legendNames.size(); i++) {
    axisGraphs_[lastEditedAxis_][i]->setName(QString::fromStdString(legendNames[i]));
  }
  QFont font;
  font.setPixelSize(fontSize);
  axisLegends_[lastEditedAxis_]->setFont(font);
}

void PlotGui::xTickSize(int fontSize){
  QFont font;
  font.setPixelSize(fontSize);
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atBottom)->setTickLabelFont(font);
}
void PlotGui::yTickSize(int fontSize){
  QFont font;
  font.setPixelSize(fontSize);
  axisRects_[lastEditedAxis_]->axis(QCPAxis::atLeft)->setTickLabelFont(font);
}

void PlotGui::setColor(Qt::GlobalColor color)
{
  axisGraphs_[lastEditedAxis_].back()->setPen(QPen(color));
}
void PlotGui::setColor(Qt::GlobalColor color, int plotNo)
{
  int plotMax = plot_->graphCount() - 1;
  if (plotNo < plotMax) {
    axisGraphs_[lastEditedAxis_][plotNo]->setPen(QPen(color));
  }
}

void PlotGui::setAxis(double xMin,double xMax,double yMin,double yMax)
{
  axisGraphs_[lastEditedAxis_].back()->keyAxis()->setRange(xMin,xMax);
  axisGraphs_[lastEditedAxis_].back()->valueAxis()->setRange(yMin,yMax);
}

void PlotGui::setXAxis(double xMin,double xMax)
{
  axisGraphs_[lastEditedAxis_].back()->keyAxis()->setRange(xMin,xMax);
}

void PlotGui::setYAxis(double yMin,double yMax)
{
  axisGraphs_[lastEditedAxis_].back()->valueAxis()->setRange(yMin,yMax);
}



void PlotGui::mousePress(QMouseEvent* ev)
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
    if(moveFlag_){
      return ;
    }
    if(zoomFlag_){
      return ;
    }
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

void PlotGui::mouseDoubleClick(QMouseEvent* ev)
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
  if(rectNo!=-1){
    if(ev->button() == Qt::RightButton){
      std::cout << "double click!!" << rectNo << std::endl;
    }
  }
}

void PlotGui::savePlot()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save plot"), "",
                                                  tr("png (*.png);jpeg (*.jpg);;All Files (*)"));

  plot_->savePng(fileName, plot_->size().width(), plot_->size().height(), 1.0);
}

void PlotGui::zoomButtonPressed()
{
  moveFlag_ = false;
  zoomFlag_ = !zoomFlag_;
  if (zoomFlag_) {
    this->setCursor(Qt::CrossCursor);

    plot_->setInteraction(QCP::iRangeZoom,true);
    zoomAct_->setIconText(tr("&Zoom"));
  } else {
    this->setCursor(Qt::ArrowCursor);
    plot_->setInteraction(QCP::iRangeZoom,true);
    zoomAct_->setIconText(tr("Zoom"));
  }
}

void PlotGui::moveButtonPressed()
{
  zoomFlag_ = false;
  moveFlag_ = !moveFlag_;
  if (moveFlag_) {
    this->setCursor(Qt::OpenHandCursor);
    plot_->setInteraction(QCP::iRangeDrag,true);
    moveAct_->setIconText(tr("&Move"));
  } else {
    this->setCursor(Qt::ArrowCursor);
    plot_->setInteraction(QCP::iRangeDrag,false);
    moveAct_->setIconText(tr("Move"));
  }
}
