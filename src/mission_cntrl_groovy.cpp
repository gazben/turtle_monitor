#include <ros/ros.h>
#include <turtlesim/Velocity.h>
#include <signal.h>
#include <termios.h>
#include "std_msgs/String.h"

#define pi 3.14159265359

class TeleopTurtle {
public:
  TeleopTurtle();

  void keyLoop();

private:
  ros::NodeHandle nh_;
  double linear_, angular_, l_scale_, a_scale_;
  ros::Publisher vel_pub_;
  ros::Publisher cmd_pub_;
};

TeleopTurtle::TeleopTurtle() :
    linear_(0),
    angular_(0),
    l_scale_(1.0),
    a_scale_(pi / 2) {
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);

  cmd_pub_ = nh_.advertise<std_msgs::String>("turtle1/cmd", 1000);
  vel_pub_ = nh_.advertise<turtlesim::Velocity>("turtle1/command_velocity", 1);
}

int kfd = 0;

struct termios cooked, raw;

//command string; l=left, u=up, r=right, d=down, s=stop, w=wait, x=up+right
//std::string commands = "uuluruuluud";
std::string commands = "xrd";

void quit(int sig) {
  tcsetattr(kfd, TCSANOW, &cooked);
  ros::shutdown();
  exit(0);
}


int main(int argc, char **argv) {
  ros::init(argc, argv, "teleop");
  TeleopTurtle teleop;
  signal(SIGINT, quit);
  teleop.keyLoop();
  return 0;
}


void TeleopTurtle::keyLoop() {
  int i = 0;
  char c = commands[i];
  bool dirty = false;

  std_msgs::String cmd;

  // get the console in raw mode
  tcgetattr(kfd, &cooked);
  memcpy(&raw, &cooked, sizeof(struct termios));
  raw.c_lflag &= ~(ICANON | ECHO);
  // Setting a new line, then end of file                        
  raw.c_cc[VEOL] = 1;
  raw.c_cc[VEOF] = 2;
  tcsetattr(kfd, TCSANOW, &raw);

  //wait for the monitor to init
  sleep(2);

  while (c != '\0') {
    linear_ = angular_ = 0;
    ROS_DEBUG("value: 0x%02X\n", c);
    std::stringstream ss;
    switch (c) {
      case 'x':
        ROS_INFO_STREAM("UP+RIGHT");
        angular_ = -1.0;
        linear_ = 1.0;
        dirty = true;
        ss << "key UP\n\r";
        ss << "key RIGHT\n\r";
        cmd.data = ss.str();
        break;
      case 'l':
        ROS_INFO_STREAM("LEFT");
        angular_ = 1.0;
        dirty = true;
        ss << "key LEFT\n\r";
        cmd.data = ss.str();
        break;
      case 'r':
        ROS_INFO_STREAM("RIGHT");
        angular_ = -1.0;
        dirty = true;
        ss << "key RIGHT\n\r";
        cmd.data = ss.str();
        break;
      case 'u':
        ROS_INFO_STREAM("UP");
        linear_ = 1.0;
        dirty = true;
        ss << "key UP\n\r";
        cmd.data = ss.str();
        break;
      case 'd':
        ROS_INFO_STREAM("DOWN");
        linear_ = -1.0;
        dirty = true;
        ss << "key DOWN\n\r";
        cmd.data = ss.str();
        break;
      case 's':
        ROS_INFO_STREAM("STOP");
        angular_ = 0.0;
        linear_ = 0.0;
        dirty = true;
        ss << "key STOP\n\r";
        cmd.data = ss.str();
        break;
      case 'w':
        usleep(1000000);
    }
    c = commands[++i];

    turtlesim::Velocity vel;
    vel.angular = a_scale_ * angular_;
    vel.linear = l_scale_ * linear_;

    usleep(500000);
    if (dirty) {
      vel_pub_.publish(vel);
      cmd_pub_.publish(cmd);
      dirty = false;
    }
    usleep(1000000);
  }
  return;
}