#ifndef __RONTROLLER_H__
#define __RONTROLLER_H__

#include <iostream>
#include <ros/ros.h>
#include <time.h>
#include <geometry_msgs/Twist.h>

#include "niklasjang_path_planning/roomba.h"
#include "niklasjang_path_planning/pddl_result_converter.h"

class Rontroller{
private :
	ros::Publisher path_pub;       // = nh.advertise<niklasjang_path_planning::MsgTutorial>("/path1/cmd_vel", 1000);
	geometry_msgs::Twist twist;
	ros::NodeHandle nh;            // ROS 시스템과 통신을 위한 노드 핸들 선언
	Roomba roomba;
	PddlResultConverter converter;
	int nextMoveRoombaIndex;	   // 1~8
public:
	Rontroller();
	~Rontroller();
	void initialize(void);
	void setTwist(double _x, double _z);
	void rollRoll(double spen);
	void goStraight(void);
	void turnLeft(void);
	void turnRight(void);
	void stop(void);
	void simulation(void);
	int checkOrientation(int);
	void setPathPub(void);
	void run(void);
	PddlResultConverter getConverter(void);
};

#endif