#include <iostream>
#include <string>
#include <ros/ros.h>

#define HOW_MANY_ROOMBAS 4

using namespace std;

#include "niklasjang_path_planning/pddl_result_converter.h"

int main(int argc, char **argv)// 노드 메인 함수
{	
	ros::init(argc, argv, "roomba_pddl_driver"); // 노드명 초기화
	PddlResultConverter converter;	
	ros::Rate loop_rate(1); // subscriber는 메시지가 오는 즉시 callback을 요청하고 바로 그 다음 메시지를 기다립니다. 
	
	while (ros::ok())
	{	
		//ROS_INFO("size %d",rontroller.GetNext().size());
		ROS_INFO("Waiting for PPDL result");
		if(converter.getNextMove(1).size()!= 0 && converter.getNextMove(2).size()!= 0 && 
			converter.getNextMove(3).size()!= 0 && converter.getNextMove(4).size()!= 0){
			ROS_INFO("R1 SIZE is %d\n", converter.getNextMove(1).size());
			ROS_INFO("R2 SIZE is %d\n", converter.getNextMove(2).size());
			ROS_INFO("R3 SIZE is %d\n", converter.getNextMove(3).size());
			ROS_INFO("R4 SIZE is %d\n", converter.getNextMove(4).size());
			converter.setRunCheck();
  		}
	  	ros::spinOnce();  
	  	loop_rate.sleep(); //반복 구독을 수행하고 callback을 지속적으로 요청합니다.
	}
	
	return 0;
}
