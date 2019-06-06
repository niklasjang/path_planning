#include "niklasjang_path_planning/rontroller.h"
/**
* Settings 
*/
void Rontroller::initialize(void){
	ROS_INFO("Rontroller constructor");
	// 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
	path_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	nextMoveRoombaIndex = 0;

}
void Rontroller::setTwist(double _x, double _z){
	twist.linear.x = _x;
	twist.angular.z = _z;
}

void Rontroller::rollRoll(double spen){
	ros::Time beginTime = ros::Time::now();
	double begin = beginTime.toSec();
	ros::Duration secondsIWantToSendMessagesFor = ros::Duration(spen); 
	double end = secondsIWantToSendMessagesFor.toSec() + begin;
	double current = 0.0;
	while(current = ros::Time::now().toSec() < end )
	{
		current = ros::Time::now().toSec();
		//ROS_INFO("x, z : (%f, %f) \n", twist.linear.x, twist.angular.z);
		//ROS_INFO("current : %f \n", current);
		//ROS_INFO("end : %f \n", end);
		path_pub.publish(twist);

		// Time between messages, so you don't blast out an thousands of 
		// messages in your 3 secondperiod
		ros::Duration(0.1).sleep();
	}
}

void Rontroller::goStraight(void){
	ROS_INFO("*** GO STRAIGHT *** ");
	setTwist(GO_STRAIGHT_X, 0.0);
	rollRoll(GO_DURATION);
	stop();
}

void Rontroller::turnLeft(void){
	ROS_INFO(" *** TURN LEFT *** ");
	setTwist(0.0, TURN_LEFT_Z);
	rollRoll(TURN_DURATION);
	stop();
}

void Rontroller::turnRight(void){
	ROS_INFO("*** TURN RIGHT *** ");
	setTwist(0.0, TURN_RIGHT_Z);
	rollRoll(TURN_DURATION);
	stop();
}

void Rontroller::stop(void){
	setTwist(0.0, 0.0);
	rollRoll(STOP_DURATION);
}


void Rontroller::setPathPub(string target){
	path_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	/*if (target == "t1"){
		path_pub = nh.advertise<geometry_msgs::Twist>("/robot1/cmd_vel", 1000);
		nextMoveRoombaIndex = 1;
	}else if (target == "t2"){
		path_pub = nh.advertise<geometry_msgs::Twist>("/robot2/cmd_vel", 1000);
		nextMoveRoombaIndex = 2;
	}else if (target == "t3"){
		path_pub = nh.advertise<geometry_msgs::Twist>("/robot3/cmd_vel", 1000);
		nextMoveRoombaIndex = 3;
	}else if (target == "t4"){
		path_pub = nh.advertise<geometry_msgs::Twist>("/robot4/cmd_vel", 1000);
		nextMoveRoombaIndex = 4;
	}else if (target == "t5"){
		path_pub = nh.advertise<geometry_msgs::Twist>("/robot5/cmd_vel", 1000);
		nextMoveRoombaIndex = 5;
	}else if (target == "t6"){
		path_pub = nh.advertise<geometry_msgs::Twist>("/robot6/cmd_vel", 1000);
		nextMoveRoombaIndex = 6;
	}else if (target == "t7"){
		path_pub = nh.advertise<geometry_msgs::Twist>("/robot7/cmd_vel", 1000);
		nextMoveRoombaIndex = 7;
	}else if (target == "t8"){
		path_pub = nh.advertise<geometry_msgs::Twist>("/robot8/cmd_vel", 1000);
		nextMoveRoombaIndex = 8;
	}else{
		ROS_INFO("nextMoveRoombaIndex is wrong!");
	}*/
	path_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
}

void Rontroller::run(void){
	ROS_INFO("Rontroller run");
	/*
	Gazebo :
	(-1, -1) (-1, 0) (-1, 1)
	(0 , -1) (0 , 0) (0 , 1)
	(1 , -1) (1 , 0) (1 , 1)

	PDDL Result:
	(x1 y1) (x2 y1) (x3 y1)
	(x1 y2) (x2 y2) (x3 y2)
	(x1 y3) (x2 y3) (x3 y3)

	Roomba::checkOrientation return value
			1      //Looking Up : return 1;
		   /|
		  / |
		3 --+-- 4
			|
			2	   //else : return -1;
	*/
	

	//nextMoveRoombaIndex default = 0
	//checkOrientation default = 2
	int curr_orientation = roomba.checkOrientation(nextMoveRoombaIndex);

	ROS_INFO("curr_orientation %d", curr_orientation);
	vector <pair<string, string> > next_move= converter.getNextMove();
	for(int i=0; i< next_move.size(); i++){

		setPathPub(next_move[i].first);
		
		if (next_move[i].second == "move-up"){
			if (curr_orientation == 1){
				goStraight();
			}else if (curr_orientation == 2){
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 3){
				turnRight();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 4){
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else{
				ROS_INFO("next_move move-up Sth Wrong!");
			}

		}else if (next_move[i].second == "move-down"){
			if (curr_orientation == 1){
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 2){
				goStraight();
			}else if (curr_orientation == 3){
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 4){
				turnRight();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else{
				ROS_INFO("next_move move-down Sth Wrong!");
			}

		}else if (next_move[i].second == "move-left"){
			if (curr_orientation == 1){
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 2){
				turnRight();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 3){
				goStraight();
			}else if (curr_orientation == 4){
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else{
				ROS_INFO("next_move move-left Sth Wrong!");
			}

		}else if (next_move[i].second == "move-right"){
			if (curr_orientation == 1){
				turnRight();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 2){
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 3){
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 4){
				goStraight();
			}else{
				ROS_INFO("next_move move-right Sth Wrong!");
			}
		}else {
			ROS_INFO("next_move Sth Wrong!");
		}
	}
	
	

	ROS_INFO("control done");
	
}

void Rontroller::reset(void){
	converter.reset();
}

int Rontroller::checkOrientation(int haveToMoveIndex){
	//Check current orientation
	/*
	return value
			1      //Looking Up : return 1;
		   /|
		  / |
		3 --+-- 4
			|
			2	   //else : return -1;
	*/;
	double yaw = 0;//roomba.getYaw(haveToMoveIndex);
	if (-0.5 <= yaw && yaw <= 0.5 ){
		//Looking Down
		return 2;
	}else if (1.0 <= yaw && yaw <= 2.0 ){
		//Looking Right
		return 4;
	}else if (-2.0 <= yaw && yaw  <= -1.0 ){
		//Looking Left
		return 3;
	}else if (-2.5 >= yaw || yaw >= 2.5 ){
		//Looking Up
		return 1;
	}else{
		ROS_INFO("Really Bad orientation. %f", yaw);
		return -1;
	}
}


PddlResultConverter Rontroller::getConverter(void){
	return converter;
}

Rontroller::Rontroller(void){
	ROS_INFO("Rontroller constructor");
}
	

Rontroller::~Rontroller(void){
	ROS_INFO("Rontroller detructor");
}
	