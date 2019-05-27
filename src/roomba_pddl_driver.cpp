#include <iostream>

#include "roomba.h"
#include "rontroller.h"
#include "pddl_result_converter.h"

int main(int argc, char **argv)// 노드 메인 함수
{	
	ros::init(argc, argv, "roomba_pddl_driver"); // 노드명 초기화
	
	Rontroller rontroller;
	rontroller.Initialize(); //Subscribe pddl_result from server and then split to next instruction vector
	ros::Rate loop_rate(10); // subscriber는 메시지가 오는 즉시 callback을 요청하고 바로 그 다음 메시지를 기다립니다. 
	
	while (ros::ok())
	{	
		//ROS_INFO("size %d",rontroller.GetNext().size());
		//ROS_INFO("Waiting for PPDL result");
		if(rontroller.converter.next_move().size()!= 0){
			ROS_INFO("Next is not empty\n");
			//rontroller.simulation();
			rontroller.TurnLeft();
			rontroller.TurnLeft();
			rontroller.TurnLeft();
			//rontroller.control();
			return 0;
  		}
	  	ros::spinOnce();  
	  	loop_rate.sleep();
	}
	ros::spin(); // ros::spin() 함수를 이용하여 반복 구독을 수행하고 callback을 지속적으로 요청합니다. 
}
