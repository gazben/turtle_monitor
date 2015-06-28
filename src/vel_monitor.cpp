#include <ros/ros.h>
#include <turtlesim/Velocity.h>
/* LOCAL INCLUDES */
//#include "monitor/EventHandler.h"
#include "monitor/Property.h"
/* INCLUDES END */

Property* property1;

void velMessageRecieved(const turtlesim::Velocity &msg) {
  SR_regtype tempStateReg = 0;
  if(msg.linear > 0){
    tempStateReg |= EVENT_UP;
    ROS_INFO_STREAM("UP");
  }

  if(msg.linear < 0){
    tempStateReg |= EVENT_DOWN;
    ROS_INFO_STREAM("DOWN");
  }

  if(msg.angular > 0){
    tempStateReg |= EVENT_LEFT;
    ROS_INFO_STREAM("LEFT");
  }

  if(msg.angular < 0){
    tempStateReg |= EVENT_RIGHT;
    ROS_INFO_STREAM("RIGHT");
  }
  StateRegister::stateRegister = tempStateReg;

  if( property1 == nullptr ){
    property1 = new Property();
    property1->constructChildrenNodeFunc = construct_START;
    construct_START(property1);
  }
  property1->Evaluate();

  ROS_INFO_STREAM("---------------");
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "subscribe_to_vel");
  ros::NodeHandle nh;

  StateRegister::clearEvents();
  ros::Subscriber sub = nh.subscribe("turtle1/command_velocity", 1000, &velMessageRecieved);
  ros::spin();

  delete property1;
  return 0;
}
