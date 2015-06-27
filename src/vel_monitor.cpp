#include <ros/ros.h>
#include <turtlesim/Velocity.h>
#include <iomanip>

#define pi 3.14159265359
enum Direction {right,down,left,up};
enum rule_state{notup,wentup} ;

int i=0;
char events[50];

Direction dir=right;
rule_state rs=notup;


void rule(){

switch(rs){
case notup:

	if (events[i-1]=='u'){
		rs=wentup;
		ROS_INFO_STREAM("went up!");
	}
break;
case wentup:

	if(events[i-1]!='r' && events[i-1]!='d'){
		ROS_INFO_STREAM("WRONG MOVE");
		if(events[i-1]=='u')
			ROS_INFO_STREAM("went up again!");
		else{
			rs=notup;
		}
	}
	if(events[i-1]=='d'){
		rs=notup;
	}
break;
}


}

void velMessageRecieved( const turtlesim::Velocity& msg) {
  ROS_INFO_STREAM(std::setprecision(2) << std::fixed << "angular: " << msg.angular << ", linear:" << msg.linear);



switch (dir){

case right:
	if (msg.angular<0) 
		dir = down;
	if(msg.angular>0)
		dir = up;
	if(msg.linear>0){
		events[i++] = 'r';
		events[i] = '\0';
	ROS_INFO_STREAM(events);
	rule();	
	}
	if(msg.linear<0){
		events[i++] = 'l';
		events[i] = '\0';
		ROS_INFO_STREAM(events);
	rule();	
	}
	ROS_INFO_STREAM("facing_right");
	break;

case down:
	if (msg.angular<0)
		dir = left;
	if(msg.angular>0)
		dir = right;
	if(msg.linear>0){
		events[i++] = 'd';
		events[i] = '\0';
	ROS_INFO_STREAM(events);
	rule();
	}
	if(msg.linear<0){
		events[i++] = 'u';
		events[i] = '\0';
		ROS_INFO_STREAM(events);
	rule();	
	}
	ROS_INFO_STREAM("facing down");
	break;

case left:
	if (msg.angular<0)
		dir = up;
	if(msg.angular>0)
		dir = down;
	if(msg.linear>0){
		events[i++] = 'l';
		events[i] = '\0';
	ROS_INFO_STREAM(events);
	rule();
	}
	if(msg.linear<0){
		events[i++] = 'r';
		events[i] = '\0';
		ROS_INFO_STREAM(events);
	rule();	
	}
	ROS_INFO_STREAM("facing left");
	break;

case up:
	if (msg.angular<0)
		dir = right;
	if(msg.angular>0)
		dir = left;
	if(msg.linear>0){
		events[i++] = 'u';
		events[i] = '\0';
	ROS_INFO_STREAM(events);
	rule();
	}
	if(msg.linear<0){
		events[i++] = 'd';
		events[i] = '\0';
		ROS_INFO_STREAM(events);
	rule();	
	}
	ROS_INFO_STREAM("facing up");
	break;

}

ROS_INFO_STREAM("______");

 




}




int main(int argc, char **argv) {
  ros::init(argc, argv, "subscribe_to_vel");
  ros::NodeHandle nh;





  ros::Subscriber sub = nh.subscribe("turtle1/command_velocity" , 1000, &velMessageRecieved);

  ros::spin();

}
