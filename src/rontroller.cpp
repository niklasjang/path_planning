#include "niklasjang_path_planning/rontroller.h"
/**
* Settings 
*/
void Rontroller::initialize(void){
	ROS_INFO("Rontroller initialize");
	// 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
	path_pub[0] = nh.advertise<geometry_msgs::Twist>("/r0/cmd_vel", 1000); //dummy
	path_pub[1] = nh.advertise<geometry_msgs::Twist>("/r1/cmd_vel", 1000);
	path_pub[2] = nh.advertise<geometry_msgs::Twist>("/r2/cmd_vel", 1000);
	path_pub[3] = nh.advertise<geometry_msgs::Twist>("/r3/cmd_vel", 1000);
	path_pub[4] = nh.advertise<geometry_msgs::Twist>("/r4/cmd_vel", 1000);
	nextMoveRoombaIndex = 0;

}

void Rontroller::setTwist(double _x, double _z){
	twist.linear.x = _x;
	twist.angular.z = _z;
}

void Rontroller::rollRoll(double spen, int idx){
	ros::Time beginTime = ros::Time::now();
	double begin = beginTime.toSec();
	ros::Duration secondsIWantToSendMessagesFor = ros::Duration(spen); 
	double end = secondsIWantToSendMessagesFor.toSec() + begin;
	double current = 0.0;
	while(current = ros::Time::now().toSec() < end )
	{
		current = ros::Time::now().toSec();
		path_pub[idx].publish(twist);
		// Time between messages, so you don't blast out an thousands of 
		// messages in your 3 secondperiod
		ros::Duration(0.1).sleep();
	}
}

void Rontroller::goStraight(int idx){
	if(idx == 1) ROS_INFO("***R1 GO STRAIGHT *** ");
	else if(idx == 2) ROS_INFO("***R2 GO STRAIGHT *** ");
	else if(idx == 3) ROS_INFO("***R3 GO STRAIGHT *** ");
	else if(idx == 4) ROS_INFO("***R4 GO STRAIGHT *** ");
	else ROS_INFO("***goStraight Invalied index *** ");

	setTwist(GO_STRAIGHT_X, 0.0);
	rollRoll(GO_DURATION, idx);
	stop(idx);
}

void Rontroller::turnLeft(int idx){
	if(idx == 1) ROS_INFO("***R1 TURN LEFT *** ");
	else if(idx == 2) ROS_INFO("***R2 TURN LEFT *** ");
	else if(idx == 3) ROS_INFO("***R3 TURN LEFT *** ");
	else if(idx == 4) ROS_INFO("***R4 TURN LEFT *** ");
	else ROS_INFO("***TURN LEFT Invalied index *** ");

	setTwist(0.0, TURN_LEFT_Z);
	rollRoll(TURN_DURATION, idx);
	stop(idx);
}

void Rontroller::turnRight(int idx){
	if(idx == 1) ROS_INFO("***R1 TURN RIGHT *** ");
	else if(idx == 2) ROS_INFO("***R2 TURN RIGHT *** ");
	else if(idx == 3) ROS_INFO("***R3 TURN RIGHT *** ");
	else if(idx == 4) ROS_INFO("***R4 TURN RIGHT *** ");
	else ROS_INFO("***TURN RIGHT Invalied index *** ");
	ROS_INFO("idx is %d\n", idx);
	setTwist(0.0, TURN_RIGHT_Z);
	rollRoll(TURN_DURATION, idx);
	stop(idx);
}

void Rontroller::stop(int idx){
	setTwist(0.0, 0.0);
	rollRoll(STOP_DURATION, idx);
}

void Rontroller::run(vector <pair<string, string> > data, int idx){
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
			0      //Looking Up : return 1;
		   /|
		  / |
		3 --+-- 1
			|
			2	   //else : return -1;
	*/
	
	//nextMoveRoombaIndex default = 0
	//checkOrientation default = 2
	
	nextMoveRoombaIndex = idx;
	int curr_orientation = roomba.checkOrientation(nextMoveRoombaIndex);
	//ROS_INFO("curr_orientation %d", curr_orientation);
	vector <pair<string, string> > next_move= data;
	for(int i=0; i< next_move.size(); i++){

		if (next_move[i].second == "move-up"){
			if (curr_orientation == 0){
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 2){
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 3){
				roomba.updateOrientation(nextMoveRoombaIndex, 1);
				turnRight(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 1){
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else{
				ROS_INFO("next_move move-up Sth Wrong!");
			}

		}else if (next_move[i].second == "move-down"){
			if (curr_orientation == 0){
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 2){
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 3){
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 1){
				roomba.updateOrientation(nextMoveRoombaIndex, 1);
				turnRight(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else{
				ROS_INFO("next_move move-down Sth Wrong!");
			}

		}else if (next_move[i].second == "move-left"){
			if (curr_orientation == 0){
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 2){
				roomba.updateOrientation(nextMoveRoombaIndex, 1);
				turnRight(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 3){
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 1){
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else{
				ROS_INFO("next_move move-left Sth Wrong!");
			}

		}else if (next_move[i].second == "move-right"){
			if (curr_orientation == 0){
				roomba.updateOrientation(nextMoveRoombaIndex, 1);
				turnRight(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 2){
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 3){
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				roomba.updateOrientation(nextMoveRoombaIndex, -1);
				turnLeft(nextMoveRoombaIndex);
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight(nextMoveRoombaIndex);
			}else if (curr_orientation == 1){
				goStraight(nextMoveRoombaIndex);
			}else{	
				ROS_INFO("next_move move-right Sth Wrong!");
			}
		}else {
			ROS_INFO("next_move Sth Wrong!");
		}
	}

	if(idx == 1){
		ROS_INFO("***R1 CONTROL DONE *** ");
		
	}else if(idx == 2) ROS_INFO("***R2 CONTROL DONE *** ");
	else if(idx == 3) ROS_INFO("***R3 CONTROL DONE *** ");
	else if(idx == 4) ROS_INFO("***R4 CONTROL DONE *** ");
	else ROS_INFO("***CONTROL DONE Invalied index *** ");
}

Rontroller::Rontroller(void){
	ROS_INFO("Rontroller constructor");
	initialize();
}
	

Rontroller::~Rontroller(void){
	ROS_INFO("Rontroller detructor");
}
	