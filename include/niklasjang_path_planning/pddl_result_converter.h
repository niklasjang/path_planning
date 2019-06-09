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
	ros::Subscriber pddl_subscriber[2];
	ros::Publisher pddl_publisher;
	ros::NodeHandle nh;    
	vector <string> chunk_list;          //instruction string vector
	vector <pair<string, string> > next_move;
	Rontroller rontroller;
public:
	PddlResultConverter();
	void chunkPddlResult(string);
	void splitByWhiteSpace(string);
	void splitByDelimiter(string& );
	void reset(void);
	~PddlResultConverter();
	void pddlResultCallback(const std_msgs::StringConstPtr& _pddl_result);
	void runCallback(const std_msgs::StringConstPtr& ok_result);
	vector <pair<string, string> > getNextMove(void);
};
#endif


