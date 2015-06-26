#include <ros/ros.h>
#include <turtlesim/Velocity.h>
#include <stdio.h>
#include "std_msgs/String.h"
#include <unistd.h>
#include <cstring>

#define pi 3.14159265359
 
class TeleopTurtle
{
public:
  TeleopTurtle();
  void execute();

private:
  ros::NodeHandle nh_;
  double linear_, angular_, l_scale_, a_scale_;
  ros::Publisher vel_pub_; 
};

TeleopTurtle::TeleopTurtle():
  linear_(0),
  angular_(0),
  l_scale_(1.0),
  a_scale_(pi/2)
{
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);

  vel_pub_ = nh_.advertise<turtlesim::Velocity>("turtle1/command_velocity", 1);
}
//write here the route of the turtle.
//l=left, r=right, u=up, d=down, s=stop
char* commands = "uuluruvuluud";

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop");
  TeleopTurtle teleop;

  teleop.execute();
 
  return(0);
}


void TeleopTurtle::execute()
{
  int i=0;
  char c = commands[i];
  bool dirty=false;
 
  for(;;)
  {
    
    while(c != '\0')
    {    
    
      linear_=angular_=0;

      //checks the current character of the command string, and
      //executes the necessary steps
      switch(c)
      {
        
        case 'l':
          ROS_WARN("LEFT");
          angular_ = 1.0;       
          dirty = true;
          break;

        case 'r':
          ROS_WARN("RIGHT");
          angular_ = -1.0;        
          dirty = true;
          break;

        case 'u':
          ROS_WARN("UP");
          linear_ = 1.0;
          dirty = true;
          break;

        case 'd':
          ROS_WARN("DOWN");
          linear_ = -1.0;
          dirty = true;    
          break;

        case 's':
          ROS_WARN("STOP");
          angular_ = 0.0;
          linear_ = 0.0;
          dirty = true;
          break;

        default : 
          ROS_WARN("WRONG CHARACTER");
          break;
      }

      c = commands[++i];

      //sets the publishable value
      turtlesim::Velocity vel;
      vel.angular = a_scale_*angular_;
      vel.linear = l_scale_*linear_;

      usleep(500000);
      
      //publishes the data 
      if(dirty==true)
      {
        vel_pub_.publish(vel);    
        dirty=false;
      }
      usleep(1000000);
    }
  }
  return;
}

