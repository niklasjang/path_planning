#ifndef __PDDL_RESULT_CONVERTER_H__
#define __PDDL_RESULT_CONVERTER_H__  

#include "std_msgs/String.h"
#include <ros/ros.h>
#include <string>
#include <vector>

#include "niklasjang_path_planning/rontroller.h"

using namespace std;

/*
Roomba object : to get roomba's orientation
Rontroller objet :to calculate turn operation
pddl_subscriber : to get PDDL_result from server
*/

class PddlResultConverter{
private:
	ros::Subscriber pddl_subscriber[HOW_MANY_ROOMBAS+1];
	ros::NodeHandle nh;    
	vector <string> chunk_list[HOW_MANY_ROOMBAS+1];          //instruction string vector
	vector <pair<string, string> > next_move[HOW_MANY_ROOMBAS+1];
	Rontroller rontroller;
	bool run_check;
public:
	PddlResultConverter();
	void chunkPddlResult(string, int);
	void splitByWhiteSpace(string, int);
	void splitByDelimiter(string&, int);
	void resetAll(void);
	void reset(int index);
	~PddlResultConverter();

	vector <pair<string, string> > getNextMove(int index);
	void r1pddlResultCallback(const std_msgs::StringConstPtr& _pddl_result);
	void r2pddlResultCallback(const std_msgs::StringConstPtr& _pddl_result);
	void r3pddlResultCallback(const std_msgs::StringConstPtr& _pddl_result);
	void r4pddlResultCallback(const std_msgs::StringConstPtr& _pddl_result);
	void run(vector <pair<string, string> > data, int idx);
	void setRunCheck(void);
	bool getRunCheck(void);

};

#endif


