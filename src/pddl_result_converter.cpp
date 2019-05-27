#include "niklasjang_path_planning/pddl_result_converter.h"

PddlResultConverter::PddlResultConverter(){
	pddl_subscriber = nh.subscribe("/result", 1000, &PddlResultConverter::pddlResultCallback, this);
}

void PddlResultConverter::pddlResultCallback(const std_msgs::StringConstPtr& _pddl_result)
{	
	//ROS_INFO("MSGCALLBACK");
	pddl_result = _pddl_result->data;
	//ROS_INFO("Subscribe data : %s", pddl_result.c_str());
	chunkPddlResult();
}

void PddlResultConverter::chunkPddlResult(void){
	//string pddl_result = "[ t7,x2,y2,move-left t6,x3,y2,move-down t3,x3,y1,move-right t7,x2,y1,move-up ]";
	splitByWhiteSpace();
	
	chunk_list.pop_back();  //Remove ]
	
	for (int i = 1; i < chunk_list.size(); i++) { 
		//ROS_INFO("string is %s", chunk_list[i].c_str());
		//cout << ins[i] << "\n";
		splitByDelimiter(chunk_list[i]);
	}
	//ROS_INFO("next size is %d", next.size());
	//cout << next.size() << "\n";
	/*
	for (int i = 0; i < next.size(); i++) {
		ROS_INFO("result 1: %s, %s, %s",  next[i].first.c_str(),  next[i].second.first.c_str(), next[i].second.second.c_str());
		cout << "(" << next[i].first <<"," << next[i].second.first << "," << next[i].second.second << ")\n";
	}
	*/
	//cout << next[0].second.first << "\n";
}


void PddlResultConverter::splitByWhiteSpace(void) {
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

void PddlResultConverter::splitByDelimiter(string& str) {
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
		/*
		if (index == 1) { x_pos = token;}
		if (index == 2) {
			y_pos = token;
			//ROS_INFO("next.push_back", obj.c_str(), x_pos.c_str(), y_pos.c_str());
			chunk_components.push_back(make_pair(obj, make_pair(x_pos, y_pos)));
		}
		*/
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
vector <pair<string, pair<string, string> > >  PddlResultConverter::getChunkComponents(void){
	return chunk_componentsn;
}
*/

int main(void){
	ROS_INFO("pddl_result_converter");
	return 0;
}