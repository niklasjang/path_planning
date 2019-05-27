#ifndef __RONTROLLER_H__
#define __RONTROLLER_H__

#include <iostream>
#include "ros/ros.h"
#include <time.h>
#include <stdlib.h> 
// ROS 기본 헤더파일
#include <geometry_msgs/Twist.h>
#include "std_msgs/String.h"
#include "niklasjang_path_planning/MsgTutorial.h"
//===================================
//Spliter header
using namespace std;
#include <string>
#include <vector>

//=========state check
#include "gazebo_msgs/ModelStates.h"
// tf2 matrix
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Quaternion.h>

#include "roomba.h"
#include "rddl_result_converter.h"

class Rontroller{

private :
	ros::Publisher path_pub;       // = nh.advertise<niklasjang_path_planning::MsgTutorial>("/path1/cmd_vel", 1000);
	geometry_msgs::Twist twist;
	ros::NodeHandle nh;            // ROS 시스템과 통신을 위한 노드 핸들 선언
	Roomba roomba;
	int nextMoveRoombaIndex;	   // 1~8
public:
	Rontroller();
	void initialize(void);
	void setTwist(double _x, double _z);
	void rollRoll(double spen);
	void goStraight(void);
	void turnLeft(void);
	void turnRight(void);
	void stop(void);

	void run(void);
	void simulation(void);
	int checkOrientation(int);
	~Rontroller();
};

#endif