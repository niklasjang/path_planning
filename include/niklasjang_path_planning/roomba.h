#ifndef __ROOMBA_H__
#define __ROOMBA_H__

#include <vector>
#include <ros/ros.h>
#include "gazebo_msgs/ModelStates.h"
#include <tf2_msgs/TFMessage.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2/LinearMath/Quaternion.h>
#define HOW_MANY_ROOMBAS 8    // Upto 8
using namespace std;

/**
* Class Roomba have two utilities.
* First, Get x,y,z,w position of model from REAL WORLD. And then Calculate Yaw. : roombaStateCallback()
* Second, Get x,y,z,w position of model from REAL WORLD. And then Calculate Yaw. : stateCallback()
*/

class Roomba{
private:
	vector< pair<double, double> > curr; //current position
	vector< pair<double, double> > dest; //destination position
	vector< double > yaw;                //current yaw
	ros::Subscriber roomba_state_subscriber;
	ros::NodeHandle state_nh;
	int running_index;					 //represent which roomba is running for this time quantom.
public:
	Roomba(void);
	~Roomba();
	void setRunningIndex(int index);
	void setCurrX(int index, double x);
	void setCurrY(int index, double x);
	void setDestX(int index, double x);
	void setDestY(int index, double x);
	void setYaw(int index, double x);
	double getCurrX(int index);
	double getCurrY(int index);
	double getDestX(int index);
	double getDestY(int index);
	double getYaw(int index);
	void simStateCallback(const gazebo_msgs::ModelStates::ConstPtr& msg);
	void realStateCallback(const tf2_msgs::TFMessage::ConstPtr& msg);
	int checkOrientation(int haveToMoveIndex);
};

#endif