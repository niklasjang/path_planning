#include "ros/ros.h" // ROS 기본 헤더파일
#include <geometry_msgs/Twist.h>
#include "niklasjang_path_planning/MsgTutorial.h"
// 메시지 콜백 함수로써, 밑에서 설정한 ros_tutorial_msg라는 이름의 토픽
// 메시지를 수신하였을 때 동작하는 함수이다
// 입력 메시지로는 ros_tutorials_topic 패키지의 MsgTutorial 메시지를 받도록 되어있다
void msgCallback(const geometry_msgs::Twist::ConstPtr& path_msg)
{
	ROS_INFO("subscribing geometry/Twist x: %f, %f", path_msg->linear.x, path_msg->angular.z); // stamp.sec 메시지를 표시한다
	}
int main(int argc, char **argv)// 노드 메인 함수
{
	ros::init(argc, argv, "path_subscriber");  // 노드명 초기화
	ros::NodeHandle nh;// ROS 시스템과 통신을 위한 노드 핸들 선언

	// 서브스크라이버 선언, ros_tutorials_topic 패키지의 MsgTutorial 메시지 파일을 이용한
	// 서브스크라이버 ros_tutorial_sub 를 작성한다. 토픽명은 "ros_tutorial_msg" 이며,
	// 서브스크라이버 큐(queue) 사이즈를 100개로 설정한다는 것이다
	ros::Subscriber ros_tutorial_sub = nh.subscribe<geometry_msgs::Twist>("/cmd_vel", 1000, msgCallback);
	// 콜백함수 호출을 위한 함수로써, 메시지가 수신되기를 대기,
	// 수신되었을 경우 콜백함수를 실행한다
	//ros::spin() asks ROS to wait for and execute callbacksuntil the node shuts down.
	/*
	https://cse.sc.edu/~jokane/agitr/agitr-letter-pubsub.pdf

	The question of whether to useros::spinOnce()orros::spin()comes down to this: 
	Does your program have any repetitive work to do, other than responding to callbacks? 
	If theanswer is “No,” then useros::spin(). If the answer is “Yes,” then a reasonable option 
	is towrite a loop that does that other work and callsros::spinOnce()periodically to processcallbacks. 
	*/
	ros::spin();
	return 0;
}
