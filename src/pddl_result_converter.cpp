#include "niklasjang_path_planning/pddl_result_converter.h"

/*
Constructor
*/
PddlResultConverter::PddlResultConverter(){
	ROS_INFO("PddlResultConverter constructor");
	pddl_subscriber[0] = nh.subscribe("/result", 100, &PddlResultConverter::pddlResultCallback, this);
	pddl_publisher = nh.advertise<std_msgs::String>("/received", 100);
	pddl_subscriber[1] = nh.subscribe("/run", 100, &PddlResultConverter::runCallback, this);
}

/*
Callback : Get #Target1 PDDL_Result 
*/
void PddlResultConverter::pddlResultCallback(const std_msgs::StringConstPtr& pddl_result)
{	
	ROS_INFO("CATCH PDDL RESULT ");
	ROS_INFO("Subscribe data : %s", pddl_result->data.c_str());
	chunkPddlResult(pddl_result->data);

	std_msgs::String msg;
	string s = "OK";
	msg.data = s;
	pddl_publisher.publish(msg);
}

/*
Callback : Get #Target1 PDDL_Result 
*/
void PddlResultConverter::runCallback(const std_msgs::StringConstPtr& ok_result)
{	
	ROS_INFO("CATCH OK RESULT ");
	ROS_INFO("Subscribe data : %s", ok_result->data.c_str());
	chunkPddlResult(ok_result->data);

	if(ok_result->data == "OK"){
		ROS_INFO("YES");
		rontroller.run(next_move);
	}else{
		ROS_INFO("NO");
	}
}


/*
chunkPddlResult
*/
void PddlResultConverter::chunkPddlResult(string pddl_result){
	//string pddl_result = "[ t7,x2,y2,move-left t6,x3,y2,move-down t3,x3,y1,move-right t7,x2,y1,move-up ]";
	splitByWhiteSpace(pddl_result);
	chunk_list.pop_back();  //Remove ]
	for (int i = 1; i < chunk_list.size(); i++) { 
		splitByDelimiter(chunk_list[i]);
	}
}

/*	
splitByWhiteSpace
*/
void PddlResultConverter::splitByWhiteSpace(string pddl_result) {
	size_t pos = 0;	
	string white_space = " ";	
	string token;
	while ((pos = pddl_result.find(white_space)) != string::npos) { 
		token = pddl_result.substr(0, pos);
		//ROS_INFO(token.c_str());
		pddl_result.erase(0, pos + white_space.length());
		chunk_list.push_back(token);
	}
	//ROS_INFO(token.c_str());
	chunk_list.push_back(pddl_result);
}

/*
splitByDelimiter
*/
void PddlResultConverter::splitByDelimiter(string& str) {
	size_t pos = 0;
	string delim = ",";
	string token, obj, x_pos, y_pos, move;
	int index = 0;
	while ((pos = str.find(delim)) != string::npos) { 
		token = str.substr(0, pos);  
		//ROS_INFO(token.c_str());
		//ROS_INFO("index is %d", index);
		if (index == 0) { obj = token; }
		str.erase(0, pos + delim.length());   //erase token from str
		if( index == 2) {
			ROS_INFO("next_move.push_back %s, %s", obj.c_str(), str.c_str());
			next_move.push_back(make_pair(obj,str));
		}
		index++;
	}
	
}

vector <pair<string, string> > PddlResultConverter::getNextMove(void){
	return next_move;
}

/*
reset
*/
void PddlResultConverter::reset(void){
	chunk_list.clear();          //instruction string vector
	next_move.clear();
}

PddlResultConverter::~PddlResultConverter(){
	ROS_INFO("PddlResultConverter destructor");
}