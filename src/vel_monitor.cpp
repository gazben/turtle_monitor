#include <ros/ros.h>
#include <turtlesim/Velocity.h>
/* LOCAL INCLUDES */
//#include "monitor/EventHandler.h"
#include "monitor/Property.h"
/* INCLUDES END */

Property property1;

void velMessageRecieved(const turtlesim::Velocity &msg) {
  ROS_INFO_STREAM(std::setprecision(2) << std::fixed << "angular: " << msg.angular << ", linear:" << msg.linear);

  SR_regtype tempStateReg;
  if(msg.linear > 0 && msg.angular == 0){
    tempStateReg |= EVENT_UP;
    ROS_INFO_STREAM("UP");
  }

  if(msg.linear < 0 && msg.angular == 0){
    tempStateReg |= EVENT_DOWN;
    ROS_INFO_STREAM("DOWN");
  }

  if(msg.linear == 0 && msg.angular > 0){
    tempStateReg |= EVENT_RIGHT;
    ROS_INFO_STREAM("LEFT");
  }

  if(msg.linear== 0 && msg.angular < 0){
    tempStateReg |= EVENT_LEFT;
    ROS_INFO_STREAM("RIGHT");
  }

  EventInterfaceHandler::getinstance()->insertEvent(tempStateReg);
  Property::Evaluate(&property1);

  ROS_INFO_STREAM("________________");
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "subscribe_to_vel");
  ros::NodeHandle nh;

  Eventhandler::clearEvents();
  property1.constructChildrenNodeFunc = construct_START;
  construct_START(&property1);  //Bruteforce for the first element

  ros::Subscriber sub = nh.subscribe("turtle1/command_velocity", 1000, &velMessageRecieved);
  ros::spin();

  return 0;
}
