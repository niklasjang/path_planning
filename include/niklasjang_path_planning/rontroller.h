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
#define GO_DURATION 1.4

#define TURN_LEFT_Z +1.3
#define TURN_RIGHT_Z -1.3
#define TURN_DURATION 1.0

#define STOP_DURATION 0.5
#define SLEEP_DURATION 0.5


class Rontroller{
private :
	ros::Publisher path_pub;
	geometry_msgs::Twist twist;
	ros::NodeHandle nh;            // ROS 시스템과 통신을 위한 노드 핸들 선언
	int nextMoveRoombaIndex;	   // 1~8
	Roomba roomba;
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
	void run(vector <pair<string, string> > &next_move);
};

#endif
