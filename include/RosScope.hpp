/*File name: RosScope.hpp
 Author: Mehmet Efe Tiryaki
 E-mail: m.efetiryaki@gmail.com
 Date created: 03.03.2019
 Date last modified: 03.03.2019
 */
#pragma once

#include <Eigen/Core>

#include <iostream>
#include <boost/thread/thread.hpp>
#include "ros_node_base/RosExecuterNodeBase.hpp"
#include "ros_node_base/ros_node_utils.hpp"
#include <vector>
#include "std_msgs/Float32.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Float32MultiArray.h"
#include "std_msgs/Float64MultiArray.h"

#include "plotgui/ScopeGui.h"
using namespace ros_node_utils;

struct topic{
  std::string type;
  std::string name;
  int index;
  int plotNo;
};

class RosScope : public RosExecuterNodeBase
{
 public:
  RosScope(std::string nodeName);

  ~RosScope()
  {
  }
  ;

  virtual void create() override;

  virtual void initialize() override;

  virtual void readParameters() override;

  virtual void initializeSubscribers() override;

  virtual void advance();

  virtual void start();

  virtual void execute() override;

  /** \~turkish
   @param[id]

   \return
   \~english
   @param[initialValue]

   \return
   */
  std::vector<double> getBuffer(int id);

  /** \~turkish
   @param[stop]

   \~english
   @param[stop]
   */
  void setStop(bool stop);

  /** \~turkish

   \~english
   */
  void clearBuffer();

  int getNumberOfFigures()
  {
    return numberOfFigure_;
  }

  void Float32SubscriberCallback(std_msgs::Float32 msg,int index);

  void Float32MultiArraySubscriberCallback(std_msgs::Float32MultiArray msg,int arrayIndex,int index);

  void Float64SubscriberCallback(std_msgs::Float64 msg,int index);

  void Float64MultiArraySubscriberCallback(std_msgs::Float64MultiArray msg,int arrayIndex,int index);

 protected:

  ScopeGui gui_;

  bool stop_;

  int n_;

  // solution matrix to be filled during calculation
  std::vector<Eigen::VectorXd> buffer_;

  std::vector<ros::Subscriber> subscribers_;

  std::vector<double> data_;

  int numberOfFigure_;

  std::vector<topic> topics_;

  boost::mutex workerMutex_;
  boost::thread* workerThread_;
};
