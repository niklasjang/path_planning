#include "ros/ros.h" // ROS 기본 헤더파일
#include "std_msgs/String.h"
#include <string>
using namespace std;
/*
Callback : Get #Target1 PDDL_Result 
*/

bool check[5];

void r1ReceiveChecker(const std_msgs::StringConstPtr& received)
{	
	ROS_INFO("CATCH OK R1 RESULT ");
	ROS_INFO("Subscribe data : %s", received->data.c_str());
	
	if(received->data == "OK"){
		check[1] = true;
	}
}

void r2ReceiveChecker(const std_msgs::StringConstPtr& received)
{	
	ROS_INFO("CATCH OK R2 RESULT ");
	ROS_INFO("Subscribe data : %s", received->data.c_str());
	
	if(received->data == "OK"){
		check[2] = true;
	}
}

void r3ReceiveChecker(const std_msgs::StringConstPtr& received)
{	
	ROS_INFO("CATCH OK R3 RESULT ");
	ROS_INFO("Subscribe data : %s", received->data.c_str());
	
	if(received->data == "OK"){
		check[3] = true;
	}
}

void r4ReceiveChecker(const std_msgs::StringConstPtr& received)
{	
	ROS_INFO("CATCH OK R4 RESULT ");
	ROS_INFO("Subscribe data : %s", received->data.c_str());
	
	if(received->data == "OK"){
		check[4] = true;
	}
}


int main(int argc, char **argv)// 노드 메인 함수
{
	ros::init(argc, argv, "result_receive_checker");  // 노드명 초기화
	
	ros::NodeHandle nh;// ROS 시스템과 통신을 위한 노드 핸들 선언
	ros::Rate loop_rate(1);
	ros::Publisher run_pub;
	ros::Subscriber receive_sub[5];

	receive_sub[1] = nh.subscribe("/r1/received", 100, r1ReceiveChecker);
	receive_sub[2] = nh.subscribe("/r2/received", 100, r2ReceiveChecker);
	receive_sub[3] = nh.subscribe("/r3/received", 100, r3ReceiveChecker);
	receive_sub[4] = nh.subscribe("/r4/received", 100, r4ReceiveChecker);
	run_pub = nh.advertise<std_msgs::String>("/run", 100);

	bool flag = true;
	while (ros::ok())
	{	
		//ROS_INFO("size %d",rontroller.GetNext().size());
		ROS_INFO("Waiting for All Result Reived");
		for( int i=1; i<5; i++){
			flag = flag && check[i];
		}
		if(flag){
			std_msgs::String msg;
			string s = "RUN";
			msg.data = s;
			run_pub.publish(msg);
			for(int i=1; i<5; i++){
				check[i] = false;
			}
		}
	  	ros::spinOnce();  
	  	loop_rate.sleep(); //반복 구독을 수행하고 callback을 지속적으로 요청합니다.
	}
	return 0;
}
