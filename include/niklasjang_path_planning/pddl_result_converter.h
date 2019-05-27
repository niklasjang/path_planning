#ifndef __PDDL_RESULT_CONVERTER_H__
#define __PDDL_RESULT_CONVERTER_H__  

#include "std_msgs/String.h"
#include <ros/ros.h>
#include <string>
#include <vector>
using namespace std;
class PddlResultConverter{
private:
	ros::Subscriber pddl_subscriber;
	string pddl_result;           //Get long pddl result from server
	ros::NodeHandle nh;    
	vector <string> chunk_list;          //instruction string vector
	//vector <pair<string, pair<string, string> > > chunk_components; // <roomba_number, x_pos, y_pos>, This is not used
	vector <pair<string, string> > next_move;
public:
	PddlResultConverter();
	~PddlResultConverter();
	void chunkPddlResult(void);
	void splitByWhiteSpace(void);
	void splitByDelimiter(string&);
	
	//vector <pair<string, pair<string, string> > >getChunkComponents(void);
	vector <pair<string, string> > getNextMove(void);
	void pddlResultCallback(const std_msgs::StringConstPtr& _pddl_result);

};

#endif


