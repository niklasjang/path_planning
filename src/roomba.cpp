#include "niklasjang_path_planning/roomba.h"

/**
* Settings for subscribe model state
*/
Roomba::Roomba(void){
	ROS_INFO("Roomba constructor");
	//roomba_state_subscriber = state_nh.subscribe("/gazebo/model_states", 1000, &Roomba::simStateCallback, this);
	//roomba_state_subscriber = state_nh.subscribe("/tf", 1000, &Roomba::realStateCallback, this);
	
	//Set 6 roomba's initial positions as (0.0, 0.0)
	/*for(int i=0; i<=HOW_MANY_ROOMBAS; i++){                    
		curr.push_back(make_pair(0.0,0.0));
		dest.push_back(make_pair(0.0,0.0));
		orientations[i] = 0;
	}*/
	orientation = 0;
}

Roomba::~Roomba(void){
	ROS_INFO("roomba destructor");
}


int Roomba::checkOrientation(void){
	//Check current orientation
	/*
	return value
			0      //Looking Up : return 1;
		   /|
		  / |
		3 --+-- 1
			|
			2	   //else : return -1;
	*/
	return orientation;
}

void Roomba::updateOrientation(int value){
	int temp = orientation;
	temp += value+4;
	temp %= 4;
	orientation = temp;
	ROS_INFO("orientation is %d\n", orientation);
}

/**
* Get current position of models from /gazebo/model_states messeges in REAL WORLD.
* From x,y,z,w calculate yaw.
*/
/*
void Roomba::realStateCallback(const tf2_msgs::TFMessage::ConstPtr& msg){
	double roll, pitch, yaw;
	//Left wheel : msg[0].transforms.transform.rotation.x
	//Right wheel : msg[1].transforms.transform.rotation.x
	geometry_msgs::Quaternion rot = msg->transforms[0].transform.rotation;
	tf2::Quaternion q(rot.x, rot.y, rot.z, rot.w);
	tf2::Matrix3x3 m(q);
  	m.getRPY(roll, pitch, yaw);
  	//ROS_INFO("Left wheel yaw is %lf\n", yaw);
}
*/



/**
* Without Gazebo simulator, this method will not be run.
* Get current position of models from /gazebo/model_states messeges in GAZEBO SIMULATOR.
* From x,y,z,w calculate yaw.
*/
/*
void Roomba::simStateCallback(const gazebo_msgs::ModelStates::ConstPtr& msg)
{	
	for(int i=1; i<9; i++){
		//ROS_INFO(msg->name[i].c_str());
		if(msg->name[i] == "create1"){
			setCurrX(0, msg->pose[i].position.x);
			setCurrX(0, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			setYaw(0, yaw);
			//ROS_INFO("yaw : %f", yaw);
		}else if(msg->name[i] == "create2"){
			setCurrX(1, msg->pose[i].position.x);
			setCurrX(1, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			setYaw(1, yaw);
		}else if(msg->name[i] == "create3"){
			setCurrX(2, msg->pose[i].position.x);
			setCurrX(2, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			setYaw(2, yaw);
		}else if(msg->name[i] == "create4"){
			setCurrX(3, msg->pose[i].position.x);
			setCurrX(3, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			setYaw(3, yaw);
		}else if(msg->name[i] == "create5"){
			setCurrX(4, msg->pose[i].position.x);
			setCurrX(4, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			setYaw(4, yaw);
		}else if(msg->name[i] == "create6"){
			setCurrX(5, msg->pose[i].position.x);
			setCurrX(5, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			setYaw(5, yaw);
		}else if(msg->name[i] == "create7"){
			setCurrX(6, msg->pose[i].position.x);
			setCurrX(6, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			setYaw(6, yaw);
		}else if(msg->name[i] == "create8"){
			setCurrX(7, msg->pose[i].position.x);
			setCurrX(7, msg->pose[i].position.y);
			tf2::Quaternion q(msg->pose[i].orientation.x, msg->pose[i].orientation.y,
				msg->pose[i].orientation.z, msg->pose[i].orientation.w);
			tf2::Matrix3x3 m(q);
			double roll, pitch, yaw;
  			m.getRPY(roll, pitch, yaw);
			setYaw(7, yaw);
		}else{
			ROS_INFO("i is %d", i);
			ROS_INFO("Sth wrong!");
		}
	}
	//ROS_INFO("stateCallback done!");
}
*/
/*
void Roomba::setCurrX(int index, double x){
	curr[index].first= x;
}
void Roomba::setCurrY(int index, double x){
	curr[index].second= x;
}
void Roomba::setDestX(int index, double x){
	dest[index].first= x;
}
void Roomba::setDestY(int index, double x){
	dest[index].second= x;
}
double Roomba::getCurrX(int index){
	return curr[index].first;
}
double Roomba::getCurrY(int index){
	return curr[index].second;
}
double Roomba::getDestX(int index){
	return dest[index].first;
}
double Roomba::getDestY(int index){
	return dest[index].second;
}
*/
