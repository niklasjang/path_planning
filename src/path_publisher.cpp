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

#include <string>
#include <vector>

//=========type check
#include <typeinfo>
using namespace std;

class MY_ROBOT{
private :
	geometry_msgs::Twist twist;
	ros::NodeHandle nh; // ROS 시스템과 통신을 위한 노드 핸들 선언
	ros::Publisher path_pub;// = nh.advertise<niklasjang_path_planning::MsgTutorial>("/path1/cmd_vel", 1000);
	ros::Subscriber pddl_sub;
	vector<string> ins;
	vector <pair<string, pair<string, string> > > next;
	string pddl_result;
public:
	void Initialize(void);
	void SetMsg(double _x, double _z){
		twist.linear.x = _x;
		twist.angular.z = _z;
	}
	void RollRoll(double spen);
	void msgCallback(const std_msgs::StringConstPtr& pddl_result);
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
	void split_inst(vector<string> &arr, string& str, const string& delim);
	void split_next(vector<pair<string, pair<string, string> > > &arr, string& str, const string& delim);
	void split(void);
};

void MY_ROBOT::split_inst(vector<string> &arr, string& str, const string& delim) {
	size_t pos = 0;		
	string token;
	while ((pos = str.find(delim)) != string::npos) { //delim ���ڸ� ã������
		token = str.substr(0, pos);  //ó������ pos ���̸�ŭ�� ���ڸ� token����
		//cout << token << endl;
		str.erase(0, pos + delim.length());
		arr.push_back(token);
	}
	//cout << str<< endl;
	arr.push_back(str);
}

void MY_ROBOT::split_next(vector <pair<string, pair<string, string> > > &arr, string& str, const string& delim) {
	size_t pos = 0;
	string token;
	int index = 0;
	string obj;
	string x_pos;
	string y_pos;
	while ((pos = str.find(delim)) != string::npos) { //delim ���ڸ� ã������
		token = str.substr(0, pos);  //ó������ pos ���̸�ŭ�� ���ڸ� token����
		//cout << token <<","<< index << endl;
		if (index == 0) { obj = token; }
		if (index == 1) { x_pos = token;}
		if (index == 2) {
			y_pos = token;
			arr.push_back(make_pair(obj, make_pair(x_pos, y_pos)));
		}
		str.erase(0, pos + delim.length());
		index++;
	}
	
}

void MY_ROBOT::msgCallback(const std_msgs::StringConstPtr& _pddl_result)
{
	pddl_result = _pddl_result->data;
	ROS_INFO(pddl_result.c_str());
	

}

void MY_ROBOT::Initialize(void){
	MY_ROBOT my_robot;
	path_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	// 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
	pddl_sub = nh.subscribe("/result", 1000, &MY_ROBOT::msgCallback, this);
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

void MY_ROBOT::split(void){
	int i = 0;
	//string pddl_result = "[ t7,x2,y2,move-left t6,x3,y2,move-down t3,x3,y1,move-right t7,x2,y1,move-up ]";
	string delimiter = " ";
	
	split_inst(ins, pddl_result, delimiter);
	
	ins.pop_back(); 
	delimiter = ",";
	for (int i = 1; i < ins.size(); i++) { 
		ROS_INFO("string is %s", ins[i].c_str());
		//cout << ins[i] << "\n";
		split_next(next, ins[i] , delimiter);
	}
	ROS_INFO("next size is %d", next.size());
	//cout << next.size() << "\n";
	for (int i = 0; i < next.size(); i++) {
		ROS_INFO("result 1: (%s, %s, %s)\n", next[i].first, next[i].second.first, next[i].second.second);
		//cout << "(" << next[i].first <<"," << next[i].second.first << "," << next[i].second.second << ")\n";
	}
	ROS_INFO("result 2: (%s, %s, %s)\n", next[i].first, next[i].second.first, next[i].second.second);
	//cout << next[0].second.first << "\n";
}

int main(int argc, char **argv)// 노드 메인 함수
{	
	ros::init(argc, argv, "path_publisher"); // 노드명 초기화
	
	MY_ROBOT robot;
	robot.Initialize();
	/*
	ros::Duration(2.0).sleep();
	robot.GoStraight();
	ros::Duration(2.0).sleep();
	robot.TurnLeft();
	ros::Duration(2.0).sleep();
	robot.TurnRight();
	*/

	ros::spin();
}


// TODO : Identificate robot id 
// TODO : Load 6 robots in a world
// TODO : Make reset robot's position with command
// TODO : Pushlish cmd_vel topic for a while
// TODO : Determine current robot's position
// TODO : Make turn function
// TODO : Make go straight function
// TODO : According to plan call function
