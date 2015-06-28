#include <ros/ros.h>
#include <turtlesim/Velocity.h>
#include <iomanip>

void velMessageRecieved(const turtlesim::Velocity &msg) {
  ROS_INFO_STREAM(std::setprecision(2) << std::fixed << "angular: " << msg.angular << ", linear:" << msg.linear);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "subscribe_to_vel");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("turtle1/command_velocity", 1000, &velMessageRecieved);
  ros::spin();

  return 0;
}
