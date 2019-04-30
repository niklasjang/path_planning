#include "ros/ros.h"
#include <time.h>
#include <stdlib.h> 
// ROS 기본 헤더파일
#include <geometry_msgs/Twist.h>
#include "niklasjang_path_planning/MsgTutorial.h"

class MY_ROBOT{
private :
	geometry_msgs::Twist twist;
	ros::NodeHandle nh; // ROS 시스템과 통신을 위한 노드 핸들 선언
	ros::Publisher path_pub;// = nh.advertise<niklasjang_path_planning::MsgTutorial>("/path1/cmd_vel", 1000);
public:
	void Initialize(void);
	void SetMsg(double _x, double _z){
		twist.linear.x = _x;
		twist.angular.z = _z;
	}
	void RollRoll(double spen);
	
	void GoStraight(void){
		SetMsg(0.3, 0.0);
		RollRoll(5);
		Stop();
	}

	void TurnLeft(void){
		SetMsg(0.0, +1.4);
		RollRoll(1.0);
		Stop();
	}

	void TurnRight(void){
		SetMsg(0.0, -1.4);
		RollRoll(1.0);
		Stop();
	}

	void Stop(void){
		SetMsg(0.0, 0.0);
		RollRoll(0.2);
	}
};

void MY_ROBOT::Initialize(void){
	path_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	// 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
	ros::Rate loop_rate(10);
}

void MY_ROBOT::RollRoll(double spen){
	ros::Time beginTime = ros::Time::now();
	double begin = beginTime.toSec();
	ros::Duration secondsIWantToSendMessagesFor = ros::Duration(spen); 
	double end = secondsIWantToSendMessagesFor.toSec() + begin;
	double current = 0.0;
	while(current = ros::Time::now().toSec() < end )
	{
		current = ros::Time::now().toSec();
		ROS_INFO("x, z : (%f, %f) \n", twist.linear.x, twist.angular.z);
		//ROS_INFO("current : %f \n", current);
		//ROS_INFO("end : %f \n", end);
		path_pub.publish(twist);

		// Time between messages, so you don't blast out an thousands of 
		// messages in your 3 secondperiod
		ros::Duration(0.1).sleep();
	}
}



int main(int argc, char **argv)// 노드 메인 함수
{	
	ros::init(argc, argv, "path_publisher"); // 노드명 초기화
	MY_ROBOT robot;
	robot.Initialize();
	ros::Duration(2.0).sleep();
	robot.GoStraight();
	ros::Duration(2.0).sleep();
	robot.TurnLeft();
	ros::Duration(2.0).sleep();
	robot.TurnRight();
	
	return 0;
}


// TODO : Identificate robot id 
// TODO : Load 6 robots in a world
// TODO : Make reset robot's position with command
// TODO : Pushlish cmd_vel topic for a while
// TODO : Determine current robot's position
// TODO : Make turn function
// TODO : Make go straight function
// TODO : According to plan call function
