/*
 * PlotGui.cpp
 *
 *  Created on: 27 Dec 2018
 *      Author: efe
 */

#include "PlotGui.h"
#include "ui_plotgui.h"

PlotGui::PlotGui(QWidget *parent)
    : QMainWindow(parent),
      ui_(new Ui::PlotGui),
      zoomFlag(false),
      moveFlag(false),
      holdOn_(false)

{
  ui_->setupUi(this);
  plot_ = ui_->customPlot;

  // Add Buttons
  // Save button
  QAction * saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::New);
  saveAct->setStatusTip(tr("Save a new file"));
  connect(saveAct, &QAction::triggered, this, &PlotGui::savePlot);
  ui_->toolbar->addAction(saveAct);

  QAction *zoomAct = new QAction(tr("&Zoom"), this);
  zoomAct->setShortcuts(QKeySequence::New);
  zoomAct->setStatusTip(tr("Zoom"));
  connect(zoomAct, &QAction::triggered, this, &PlotGui::zoomButtonPressed);
  ui_->toolbar->addAction(zoomAct);

  QAction *moveAct = new QAction(tr("&Move"), this);
  moveAct->setShortcuts(QKeySequence::New);
  moveAct->setStatusTip(tr("Move"));
  connect(moveAct, &QAction::triggered, this, &PlotGui::moveButtonPressed);
  ui_->toolbar->addAction(moveAct);

connect(plot_, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
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

int plotNo = 0 ;
if (!holdOn_) {
  plot_->clearGraphs();
  plot_->addGraph();
  plot_->xAxis->setLabel("x");
  plot_->yAxis->setLabel("y");
  plot_->xAxis->setRange(x_eig[0], x_eig[x_eig.size() - 1]);
  plot_->yAxis->setRange(y_eig.minCoeff(), y_eig.maxCoeff());
} else {
  plot_->addGraph();
  plotNo = plot_->graphCount() - 1;
}
plot_->graph(plotNo)->setData(x, y);
plot_->graph(plotNo)->setPen(QPen(color));
plot_->replot();
}

void PlotGui::scatter(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig)
{
plot(x_eig, y_eig, Qt::blue);
}
void PlotGui::scatter(Eigen::VectorXd x_eig, Eigen::VectorXd y_eig, Qt::GlobalColor color)
{
QVector<double> x(x_eig.size());
QVector<double> y(y_eig.size());
for (int i = 0; i < x.size(); ++i) {
  x[i] = x_eig[i];
  y[i] = y_eig[i];
}
int plotNo = 0 ;
if (!holdOn_) {
  plot_->clearGraphs();
  plot_->addGraph();
  plot_->xAxis->setLabel("x");
  plot_->yAxis->setLabel("y");
  plot_->xAxis->setRange(x_eig[0], x_eig[x_eig.size() - 1]);
  plot_->yAxis->setRange(y_eig.minCoeff(), y_eig.maxCoeff());
} else {
  plot_->addGraph();
  plotNo = plot_->graphCount() - 1;
}
plot_->graph(plotNo)->setData(x, y);
plot_->graph(plotNo)->setPen(QPen(color));
plot_->graph(plotNo)->setLineStyle(QCPGraph::lsNone);
plot_->graph(plotNo)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 4));

plot_->replot();
}


void PlotGui::fillBelow(QColor color){
  int plotNo = plot_->graphCount() - 1;
  plot_->graph(plotNo)->setBrush(QBrush(color));
}

void PlotGui::holdOn()
{
holdOn_ = true;
}

void PlotGui::holdOff()
{
holdOn_ = false;
}

void PlotGui::setColor(Qt::GlobalColor color)
{
int plotNo = plot_->graphCount() - 1;
plot_->graph(plotNo)->setPen(QPen(color));
}
void PlotGui::setColor(Qt::GlobalColor color, int plotNo)
{
int plotMax = plot_->graphCount() - 1;
if (plotNo < plotMax) {
  plot_->graph(plotNo)->setPen(QPen(color));
}
}

void PlotGui::setAxis(double xMin,double xMax,double yMin,double yMax)
{
  plot_->xAxis->setRange(xMin, xMax);
  plot_->yAxis->setRange(yMin, yMax);
}

void PlotGui::setXAxis(double xMin,double xMax)
{
  plot_->xAxis->setRange(xMin, xMax);
}

void PlotGui::setYAxis(double yMin,double yMax)
{
  plot_->yAxis->setRange(yMin, yMax);
}

void PlotGui::legend(std::vector<std::string> legendNames)
{
plot_->legend->setVisible(true);
for (int i = 0; i < legendNames.size(); i++) {
  plot_->graph(i)->setName(QString::fromStdString(legendNames[i]));
}
}

void PlotGui::makeTestPlot()
{
// generate some data:
QVector<double> x(101), y(101);  // initialize with entries 0..100
for (int i = 0; i < 101; ++i) {
  x[i] = i / 50.0 - 1;  // x goes from -1 to 1
  y[i] = x[i] * x[i];  // let's plot a quadratic function
}

// create graph and assign data to it:
plot_->addGraph();
plot_->graph(0)->setData(x, y);
// give the axes some labels:
plot_->xAxis->setLabel("x");
plot_->yAxis->setLabel("y");
// set axes ranges, so we see all data:
plot_->xAxis->setRange(-1, 1);
plot_->yAxis->setRange(0, 1);

plot_->replot();
}

void PlotGui::mousePress(QMouseEvent* ev)
{
//QCursor::pos() global position
const QPoint p = ev->pos();
cout << "Mouse pressed at " << p.x() << " , " << p.y() << " !!" << endl;

}

void PlotGui::savePlot()
{
QString fileName = QFileDialog::getSaveFileName(this, tr("Save plot"), "",
                                                tr("png (*.png);jpeg (*.jpg);;All Files (*)"));

plot_->savePng(fileName, plot_->size().width(), plot_->size().height(), 1.0);
}

void PlotGui::zoomButtonPressed()
{
moveFlag = false;
zoomFlag = !zoomFlag;
if (zoomFlag) {
  this->setCursor(Qt::CrossCursor);
} else {
  this->setCursor(Qt::ArrowCursor);
}
}

void PlotGui::moveButtonPressed()
{
zoomFlag = false;
moveFlag = !moveFlag;
if (moveFlag) {
  this->setCursor(Qt::OpenHandCursor);
} else {
  this->setCursor(Qt::ArrowCursor);
}
}
