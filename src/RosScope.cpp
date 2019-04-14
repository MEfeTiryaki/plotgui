/*File name: RosScope.cpp
 Author: Mehmet Efe Tiryaki
 E-mail: m.efetiryaki@gmail.com
 Date created: 03.03.2019
 Date last modified: 03.03.2019
 */

#include "RosScope.hpp"

RosScope::RosScope(std::string nodeName)
    : RosExecuterNodeBase(nodeName),
      n_(2),
      stop_(false),
      workerThread_(),
      numberOfFigure_(1)
{

}

void RosScope::create()
{
  RosExecuterNodeBase::create();

  CONFIRM("RosScope: create");
}

void RosScope::initialize()
{
  RosExecuterNodeBase::initialize();

  gui_.setScopeNumber(getNumberOfFigures());
  gui_.setXAxisRange(5);
  gui_.setYAxis(-0.5, 0.5);
  gui_.addTimeSignal(boost::bind(&RosScope::getBuffer, this, -1));
  gui_.addStopFunction(boost::bind(&RosScope::setStop, this, _1));
  gui_.addClearFunction(boost::bind(&RosScope::clearBuffer, this));

  for(auto topic : topics_ ){
    gui_.addGraph(0, boost::bind(&RosScope::getBuffer, this, topic.plotNo));
  }

  CONFIRM("RosScope: initialize");
}

void RosScope::readParameters()
{
  paramRead(this->nodeHandle_, "/ros_scope/layout/rows", numberOfFigure_);

  topic dummy = topic();
  int i = 0;
  while (paramRead(this->nodeHandle_, "/ros_scope/topic_" + std::to_string(i) + "/type", dummy.type)) {
    paramRead(this->nodeHandle_, "/ros_scope/topic_" + std::to_string(i) + "/name", dummy.name);
    paramRead(this->nodeHandle_, "/ros_scope/topic_" + std::to_string(i) + "/index", dummy.index);
    paramRead(this->nodeHandle_, "/ros_scope/topic_" + std::to_string(i) + "/plot_no", dummy.plotNo);
    topics_.push_back(dummy);
    data_.push_back(0.0);
    i++;
  }

  CONFIRM("RosScope: readParameters");
}
void RosScope::initializeSubscribers()
{
  RosExecuterNodeBase::initializeSubscribers();

  for (int topicIndex = 0; topicIndex < topics_.size(); topicIndex++) {
    auto topic = topics_[topicIndex];
    if (topic.type == "Float64") {
      subscribers_.push_back(
          this->nodeHandle_->subscribe<std_msgs::Float64MultiArray>(
              topic.name,
              1,
              boost::function<void(std_msgs::Float64)>(
                  boost::bind(&RosScope::Float64SubscriberCallback, this, _1, topicIndex))));
    } else if (topic.type == "Float64MultiArray") {
      subscribers_.push_back(
          this->nodeHandle_->subscribe<std_msgs::Float64MultiArray>(
              topic.name,
              1,
              boost::function<void(std_msgs::Float64MultiArray)>(
                  boost::bind(&RosScope::Float64MultiArraySubscriberCallback, this, _1, topic.index,
                              topicIndex))));
    } else if (topic.type == "Float32") {
      subscribers_.push_back(
          this->nodeHandle_->subscribe<std_msgs::Float64MultiArray>(
              topic.name,
              1,
              boost::function<void(std_msgs::Float32)>(
                  boost::bind(&RosScope::Float32SubscriberCallback, this, _1, topicIndex))));
    } else if (topic.type == "Float32MultiArray") {
      subscribers_.push_back(
          this->nodeHandle_->subscribe<std_msgs::Float64MultiArray>(
              topic.name,
              1,
              boost::function<void(std_msgs::Float32MultiArray)>(
                  boost::bind(&RosScope::Float32MultiArraySubscriberCallback, this, _1, topic.index,
                              topicIndex))));
    } else {
      ERROR("RosScope : Undefined topic type " + topic.type);
    }
  }

  CONFIRM("RosScope: initializeSubscribers");
}

void RosScope::advance()
{
  if (!stop_) {
    boost::lock_guard<boost::mutex> lock(workerMutex_);
    Eigen::VectorXd b = Eigen::VectorXd::Zero(n_ + 1);
    b[n_] = ros::Time::now().toSec();
    for (int i = 0; i < data_.size(); i++) {
      b[i] = data_[i];
    }
    buffer_.push_back(b);
  }
}

void RosScope::execute()
{
  this->rate_ = new ros::Rate(1000);
  while (ros::ok()) {
    advance();
    ros::spinOnce();
    this->rate_->sleep();
  }
}

void RosScope::start()
{
  workerThread_ = new boost::thread(boost::bind(&RosScope::execute, this));
  gui_.show();
  stop_ = false;
}

void RosScope::setStop(bool stop)
{
  //WARNING("RosScope : set - " + std::to_string(stop));
  stop_ = stop;

}

std::vector<double> RosScope::getBuffer(int id)
{
  std::vector<double> b = std::vector<double>(0);

  if (buffer_.size() > 1) {

    boost::lock_guard<boost::mutex> lock(workerMutex_);
    if (id == -1) {

      for (int i = 0; i < buffer_.size() - 1; i++) {
        b.push_back(buffer_[i][n_]);
      }
    } else {
      for (int i = 0; i < buffer_.size() - 1; i++) {
        b.push_back(buffer_[i][id]);
      }
    }
  }
  return b;
}

void RosScope::clearBuffer()
{
  Eigen::VectorXd last = buffer_.back();
  buffer_.clear();
  buffer_.push_back(last);
}

void RosScope::Float32SubscriberCallback(std_msgs::Float32 msg, int index)
{
  boost::lock_guard<boost::mutex> lock(workerMutex_);
  data_[index] = msg.data;
}
void RosScope::Float32MultiArraySubscriberCallback(std_msgs::Float32MultiArray msg, int arrayIndex,
                                                   int index)
{
  boost::lock_guard<boost::mutex> lock(workerMutex_);
  data_[index] = msg.data[arrayIndex];
}
void RosScope::Float64SubscriberCallback(std_msgs::Float64 msg, int index)
{
  boost::lock_guard<boost::mutex> lock(workerMutex_);
  data_[index] = msg.data;
}
void RosScope::Float64MultiArraySubscriberCallback(std_msgs::Float64MultiArray msg, int arrayIndex,
                                                   int index)
{
  std::cout<<" callback " << ros::Time::now().toSec() << std::endl;

  boost::lock_guard<boost::mutex> lock(workerMutex_);
  data_[index] = msg.data[arrayIndex];
}

