#include <ros/ros.h>
#include <turtlesim/Velocity.h>
#include <iomanip>

#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4
enum Direction {right,down,left,up}; 
enum rule_state{notup,wentup} ;	

Direction dir=right;
rule_state rs=notup;


void rule(char event){

	switch(rs){ //rule checking state machine
		
		case notup:		//in case there was no up event yet

			if (event==UP){	//ha felfele event erkezett
				rs=wentup;			//allapotot valtunk
			}
			break;

		case wentup:	//in case there was an up event (rule checking starts)

			if(event!=RIGHT && event!=DOWN){ // if the current event is not allowed
				ROS_ERROR_STREAM("WRONG MOVE");
				rs=notup;		//error, rule checking stops	
			}

			if(event==DOWN){
				rs=notup;		//all events were allowed, rule checking stops
			}
			break;
		default:
			break;
	}


}

void velMessageRecieved( const turtlesim::Velocity& msg)
{
	/*ROS_INFO_STREAM(std::setprecision(2) << std::fixed << "angular: " 	
		<< msg.angular << ", linear:" << msg.linear);
	*/
	int event = 0;

	switch (dir) //event generating state machine
	{

		case right:			//in case the turtle faces now right
			if (msg.angular<0) //turned right -> now turtle faces down
				dir = down;
			if(msg.angular>0) //turned left -> now turtle faces up
				dir = up;
			if(msg.linear>0) //went straight forward
			{
				event = RIGHT;
				rule(event);	
			}

			if(msg.linear<0)
			{
				event = LEFT;
				rule(event);	
			}
			//ROS_INFO_STREAM("facing_right");
			break;

		case down:				//in case the turtle faces now down 							
			if (msg.angular<0)	//almost the same as the previous state
				dir = left;
			if(msg.angular>0)
				dir = right;
			if(msg.linear>0)
			{
				event = DOWN;
				rule(event);
			}

			if(msg.linear<0)
			{
				event = UP;
				rule(event);	
			}
			//ROS_INFO_STREAM("facing down");
			break;

		case left:
			if (msg.angular<0)
				dir = up;
			if(msg.angular>0)
				dir = down;
			if(msg.linear>0)
			{
				event = LEFT;
				rule(event);
			}

			if(msg.linear<0)
			{
				event = RIGHT;
				rule(event);	
			}
			//ROS_INFO_STREAM("facing left");
			break;

		case up:
			if (msg.angular<0)
				dir = right;
			if(msg.angular>0)
				dir = left;
			if(msg.linear>0)
			{
				event = UP;
				rule(event);
			}

			if(msg.linear<0)
			{
				event = DOWN;
				rule(event);	
			}
			//ROS_INFO_STREAM("facing up");
			break;
		default: 
			break;

	}

ROS_INFO_STREAM("______");

}


int main(int argc, char **argv) 
{
  ros::init(argc, argv, "subscribe_to_vel");
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("turtle1/command_velocity" , 1000, 
  	&velMessageRecieved);

  ros::spin();

}
