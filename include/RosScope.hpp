/*File name: RosScope.hpp
 Author: Mehmet Efe Tiryaki
 E-mail: m.efetiryaki@gmail.com
 Date created: 03.03.2019
 Date last modified: 03.03.2019
 */
 #pragma once

#include "ros_node_base/RosExecuterNodeBase.hpp"
 class RosScope : public ros_node_utils::RosExecuterNodeBase
 {
  public:
   RosScope(std::string nodeName);

   ~RosScope(){};

   virtual void create() override;

   virtual void initialize()  override;

   virtual void readParameters()  override;

 };
