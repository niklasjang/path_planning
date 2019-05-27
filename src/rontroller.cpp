#include "Rontroller.h"

/**
* Settings 
*
*/
void Rontroller::initialize(void){
	roomba = new Roomba();
	// 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
	path_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	nextMoveRoombaIndex = 0;
	converter = new PddlResultConverter();
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
		SetMsg(0.7, 0.0);
		RollRoll(5);
		Stop();
	}

void Rontroller::turnLeft(void){
	SetMsg(0.0, -2.2);
	RollRoll(1.5);
	Stop();
}

void Rontroller::turnRight(void){
	SetMsg(0.0, +2.2);
	RollRoll(1.5);
	Stop();
}

void Rontroller::stop(void){
	SetMsg(0.0, 0.0);
	RollRoll(0.2);
}

void Rontroller::setPathPub(void){
	for (int i = 0; i < next.size(); i++) {
		//ROS_INFO(next[i].first.c_str());
		//ROS_INFO(next[i].second.first.c_str());
		//ROS_INFO(next[i].second.second.c_str());
		if (next[i].first == "t1"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot1/cmd_vel", 1000);
			nextMoveRoombaIndex = 1;
		}else if (next[i].first == "t2"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot2/cmd_vel", 1000);
			nextMoveRoombaIndex = 2;
		}else if (next[i].first == "t3"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot3/cmd_vel", 1000);
			nextMoveRoombaIndex = 3;
		}else if (next[i].first == "t4"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot4/cmd_vel", 1000);
			nextMoveRoombaIndex = 4;
		}else if (next[i].first == "t5"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot5/cmd_vel", 1000);
			nextMoveRoombaIndex = 5;
		}else if (next[i].first == "t6"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot6/cmd_vel", 1000);
			nextMoveRoombaIndex = 6;
		}else if (next[i].first == "t7"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot7/cmd_vel", 1000);
			nextMoveRoombaIndex = 7;
		}else if (next[i].first == "t8"){
			path_pub = nh.advertise<geometry_msgs::Twist>("/robot8/cmd_vel", 1000);
			nextMoveRoombaIndex = 8;
		}else{
			ROS_INFO("nextMoveRoombaIndex is wrong!");
		}
	}
}

void Rontroller::run(void){
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
	setPathPub();
	int curr_orientation = roomba.checkOrientation(haveToMoveIndex);
	ROS_INFO("curr_orientation %d", curr_orientation);
	ROS_INFO("nextMove[i].second %s", nextMove[i].second.c_str());
	if (nextMove[i].second == "move-up"){
		if (curr_orientation == 1){
			GoStraight();
		}else if (curr_orientation == 2){
			TurnLeft();
			ros::Duration(2.0).sleep();
			TurnLeft();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 3){
			TurnRight();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 4){
			TurnLeft();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else{
			ROS_INFO("nextMove move-up Sth Wrong!");
		}
	}else if (nextMove[i].second == "move-down"){
		if (curr_orientation == 1){
			TurnLeft();
			ros::Duration(2.0).sleep();
			TurnLeft();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 2){
			GoStraight();
		}else if (curr_orientation == 3){
			TurnLeft();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 4){
			TurnRight();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else{
			ROS_INFO("nextMove move-down Sth Wrong!");
		}
	}else if (nextMove[i].second == "move-left"){
		if (curr_orientation == 1){
			TurnLeft();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 2){
			TurnRight();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 3){
			GoStraight();
		}else if (curr_orientation == 4){
			TurnLeft();
			ros::Duration(2.0).sleep();
			TurnLeft();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else{
			ROS_INFO("nextMove move-left Sth Wrong!");
		}
	}else if (nextMove[i].second == "move-right"){
		if (curr_orientation == 1){
			TurnRight();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 2){
			TurnLeft();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 3){
			TurnLeft();
			ros::Duration(2.0).sleep();
			TurnLeft();
			ros::Duration(2.0).sleep();
			GoStraight();
		}else if (curr_orientation == 4){
			GoStraight();
		}else{
			ROS_INFO("nextMove move-right Sth Wrong!");
		}
	}else {
		ROS_INFO("nextMove Sth Wrong!");
	}
	ROS_INFO("control done");
}

void Rontroller::simulation(void);
int Rontroller::CheckOrientation(int);


