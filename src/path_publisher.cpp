#include "ros/ros.h"
#include <time.h>
#include <stdlib.h> 
// ROS 기본 헤더파일
#include <geometry_msgs/Twist.h>
#include "niklasjang_path_planning/MsgTutorial.h"

int main(int argc, char **argv)// 노드 메인 함수
{
	
	ros::init(argc, argv, "path_publisher"); // 노드명 초기화
	ros::NodeHandle nh; // ROS 시스템과 통신을 위한 노드 핸들 선언

	// 퍼블리셔 선언, ros_tutorials_topic 패키지의 MsgTutorial 메시지 파일을 이용한
	// 퍼블리셔 ros_tutorial_pub 를 작성한다. 토픽명은 "ros_tutorial_msg" 이며,
	// 퍼블리셔 큐(queue) 사이즈를 100개로 설정한다는 것이다
	ros::Publisher path_pub;// = nh.advertise<niklasjang_path_planning::MsgTutorial>("/path1/cmd_vel", 1000);
	path_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	// 루프 주기를 설정한다. "10" 이라는 것은 10Hz를 말하는 것으로 0.1초 간격으로 반복된다
	ros::Rate loop_rate(10);
	
	niklasjang_path_planning::MsgTutorial turn_span;

	
	double t_begin = ros::Time::now().toSec();
	double t_current = ros::Time::now().toSec();
	while (ros::ok()){//If this node is not kiiled, return true
		//niklasjang_path_planning::MsgTutorial msg;
		//msg.target_x = 10;
		geometry_msgs::Twist twist;
		twist.linear.x = 0.3;
		twist.angular.z = 0.0;

		//Solution 1. Do not use msg.
		double t_current = ros::Time::now().toSec();
		double duration = t_current - t_begin ;
		ROS_INFO("duration %f = %f - %f \n", duration, t_current, t_begin); 
		if( duration > 3.0){ //ros::Duration(3.0).sleep();
			twist.linear.x = 0.3;
			twist.angular.z = 3.0;
		}




		ROS_INFO("publishing geometry/Twist x: %f, %f", twist.linear.x, twist.angular.z); // stamp.sec 메시지를 표시한다

		//ROS_INFO("publishing geometry/Twist x: %d, %d", msg.target_x, msg.target_y); // stamp.sec 메시지를 표시한다

		path_pub.publish(twist); // 메시지를 발행한다 

		loop_rate.sleep(); // 위에서 정한 루프 주기에 따라 슬립에 들어간다. If  time loop is not filled with this loop_rate
	}
	
	return 0;
}
