#include "niklasjang_path_planning/rontroller.h"
/**
* Settings 
*/
void Rontroller::initialize(void){
	ROS_INFO("Rontroller initialize");
	// 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
	path_pub= nh.advertise<geometry_msgs::Twist>("/cmd_vel", 100); //dummy
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
		path_pub.publish(twist);
		// Time between messages, so you don't blast out an thousands of 
		// messages in your 3 secondperiod
		ros::Duration(0.1).sleep();
	}
}

void Rontroller::goStraight(void){
	ROS_INFO("****goStraight****");
	setTwist(GO_STRAIGHT_X, 0.0);
	rollRoll(GO_DURATION);
}

void Rontroller::turnLeft(void){
	ROS_INFO("****turnLeft****");
	setTwist(0.0, TURN_LEFT_Z);
	rollRoll(TURN_DURATION);
}

void Rontroller::turnRight(void){
	ROS_INFO("****turnRight****");
	setTwist(0.0, TURN_RIGHT_Z);
	rollRoll(TURN_DURATION);
}

void Rontroller::stop(void){
	setTwist(0.0, 0.0);
	rollRoll(STOP_DURATION);
}

void Rontroller::run(vector <pair<string, string> > &next_move){
	ROS_INFO("****RUN****");
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
			2(default)//else : return -1;
	*/
	
	int curr_orientation = roomba.checkOrientation();
	for(int i=0; i< next_move.size(); i++){
		if (next_move[i].second == "move-up"){
			if (curr_orientation == 0){
				goStraight();
			}else if (curr_orientation == 2){
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 3){
				roomba.updateOrientation(1);
				turnRight();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 1){
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else{
				ROS_INFO("next_move move-up Sth Wrong!");
			}

		}else if (next_move[i].second == "move-down"){
			if (curr_orientation == 0){
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 2){
				goStraight();
			}else if (curr_orientation == 3){
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 1){
				roomba.updateOrientation(1);
				turnRight();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else{
				ROS_INFO("next_move move-down Sth Wrong!");
			}

		}else if (next_move[i].second == "move-left"){
			if (curr_orientation == 0){
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 2){
				roomba.updateOrientation(1);
				turnRight();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 3){
				goStraight();
			}else if (curr_orientation == 1){
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else{
				ROS_INFO("next_move move-left Sth Wrong!");
			}

		}else if (next_move[i].second == "move-right"){
			if (curr_orientation == 0){
				roomba.updateOrientation(1);
				turnRight();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 2){
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 3){
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				roomba.updateOrientation(-1);
				turnLeft();
				ros::Duration(SLEEP_DURATION).sleep();
				goStraight();
			}else if (curr_orientation == 1){
				goStraight();
			}else{	
				ROS_INFO("next_move move-right Sth Wrong!");
			}
		}else {
			ROS_INFO("next_move Sth Wrong!");
		}
	}
	next_move.clear();
	ROS_INFO("***CONTROL DONE *** ");
}

Rontroller::Rontroller(void){
	ROS_INFO("Rontroller constructor");
	initialize();
}
	

Rontroller::~Rontroller(void){
	ROS_INFO("Rontroller detructor");
}
	