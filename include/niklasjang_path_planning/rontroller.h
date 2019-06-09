#ifndef __RONTROLLER_H__
#define __RONTROLLER_H__

#include <iostream>
#include <ros/ros.h>
#include <time.h>
#include <geometry_msgs/Twist.h>
#include <vector>

#include "niklasjang_path_planning/roomba.h"
using namespace std;


#define GO_STRAIGHT_X 0.3
#define GO_DURATION 2.0

#define TURN_LEFT_Z +2.2
#define TURN_RIGHT_Z -2.2
#define TURN_DURATION 0.7

#define STOP_DURATION 0.2
#define SLEEP_DURATION 2.0


class Rontroller{
private :
	ros::Publisher path_pub[HOW_MANY_ROOMBAS+1];       // = nh.advertise<niklasjang_path_planning::MsgTutorial>("/path1/cmd_vel", 1000);
	geometry_msgs::Twist twist;
	ros::NodeHandle nh;            // ROS 시스템과 통신을 위한 노드 핸들 선언
	int nextMoveRoombaIndex;	   // 1~8
	Roomba roomba;
public:
	Rontroller();
	~Rontroller();
	void initialize(void);
	void setTwist(double _x, double _z);
	void rollRoll(double spen,int idx);
	void goStraight(int idx);
	void turnLeft(int idx);
	void turnRight(int idx);
	void stop(int idx);
	void run(vector <pair<string, string> > data, int idx);
};


/*
run execute vector operations. run method get vector as param
converter publish eah

*/
#endif
