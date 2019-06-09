#include "niklasjang_path_planning/pddl_result_converter.h"

/*
Constructor
*/
PddlResultConverter::PddlResultConverter(){
	ROS_INFO("PddlResultConverter constructor");
	pddl_subscriber[1] = nh.subscribe("/r1_result", 1000, &PddlResultConverter::r1pddlResultCallback, this);
	pddl_subscriber[2] = nh.subscribe("/r2_result", 1000, &PddlResultConverter::r2pddlResultCallback, this);
	pddl_subscriber[3] = nh.subscribe("/r3_result", 1000, &PddlResultConverter::r3pddlResultCallback, this);
	pddl_subscriber[4] = nh.subscribe("/r4_result", 1000, &PddlResultConverter::r4pddlResultCallback, this);
	run_check = false;
}


/*
chunkPddlResult
*/
void PddlResultConverter::chunkPddlResult(string pddl_result,int idx){
	//string pddl_result = "[ t7,x2,y2,move-left t6,x3,y2,move-down t3,x3,y1,move-right t7,x2,y1,move-up ]";
	splitByWhiteSpace(pddl_result, idx);
	
	chunk_list[idx].pop_back();  //Remove ]
	
	for (int i = 1; i < chunk_list[idx].size(); i++) { 
		splitByDelimiter(chunk_list[idx][i], idx);
	}
}

/*
splitByWhiteSpace
*/
void PddlResultConverter::splitByWhiteSpace(string pddl_result, int idx) {
	size_t pos = 0;	
	string white_space = " ";	
	string token;
	while ((pos = pddl_result.find(white_space)) != string::npos) { 
		token = pddl_result.substr(0, pos);
		//ROS_INFO(token.c_str());
		pddl_result.erase(0, pos + white_space.length());
		chunk_list[idx].push_back(token);
	}
	//ROS_INFO(token.c_str());
	chunk_list[idx].push_back(pddl_result);
}


	

/*
splitByDelimiter
*/
void PddlResultConverter::splitByDelimiter(string& str, int idx) {
	//ROS_INFO("splitNextInstruction");
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
			next_move[idx].push_back(make_pair(obj,str));
		}
		index++;
	}
	
}


/*
reset
*/
void PddlResultConverter::resetAll(){
	for(int i=0; i<HOW_MANY_ROOMBAS+1; i++){
		chunk_list[i].clear();          //instruction string vector
		next_move[i].clear();
	}
}

void PddlResultConverter::reset(int index){
	chunk_list[index].clear();          //instruction string vector
	next_move[index].clear();
}


/*
Callback : Get #Target1 PDDL_Result 
*/
void PddlResultConverter::r1pddlResultCallback(const std_msgs::StringConstPtr& pddl_result)
{	
	ROS_INFO("R1 CATCH PDDL RESULT ");
	ROS_INFO("Subscribe data : %s", pddl_result->data.c_str());
	chunkPddlResult(pddl_result->data, 1);
	run(next_move[1], 1);
	next_move[1].clear();
}

/*
Callback : Get #Target1 PDDL_Result 
*/
void PddlResultConverter::r2pddlResultCallback(const std_msgs::StringConstPtr& pddl_result)
{	
	ROS_INFO("R2 CATCH PDDL RESULT ");
	ROS_INFO("Subscribe data : %s", pddl_result->data.c_str());
	chunkPddlResult(pddl_result->data, 2);
	run(next_move[2], 2);
	next_move[2].clear();
}

/*
Callback : Get #Target1 PDDL_Result 
*/
void PddlResultConverter::r3pddlResultCallback(const std_msgs::StringConstPtr& pddl_result)
{	
	ROS_INFO("R3 CATCH PDDL RESULT ");
	ROS_INFO("Subscribe data : %s", pddl_result->data.c_str());
	chunkPddlResult(pddl_result->data, 3);
	run(next_move[3], 3);
	next_move[3].clear();
}

/*
Callback : Get #Target1 PDDL_Result 
*/
void PddlResultConverter::r4pddlResultCallback(const std_msgs::StringConstPtr& pddl_result)
{	
	ROS_INFO("R4 CATCH PDDL RESULT ");
	ROS_INFO("Subscribe data : %s", pddl_result->data.c_str());
	chunkPddlResult(pddl_result->data, 4);
	run(next_move[4], 4);
	next_move[4].clear();
}


void PddlResultConverter::run(vector <pair<string, string> > data, int idx){
	rontroller.run(data, idx);
}

vector <pair<string, string> > PddlResultConverter::getNextMove(int index){
	return next_move[index];
}

bool PddlResultConverter::getRunCheck(void){
	return run_check;
}

void PddlResultConverter::setRunCheck(void){
	run_check = true;
}

PddlResultConverter::~PddlResultConverter(){
	ROS_INFO("PddlResultConverter destructor");
}