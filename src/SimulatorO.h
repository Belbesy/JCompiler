/*
 * SimulatorO.h
 *
 *  Created on: May 14, 2013
 *      Author: raed
 */

#ifndef SIMULATORO_H_
#define SIMULATORO_H_

#include <vector>
#include <string>

using namespace std;

class SimulatorO {
public:
	vector<string> tokens;
	int t;
	SimulatorO();
	pair<string,int> next_token();
	virtual ~SimulatorO();
};

#endif /* SIMULATORO_H_ */
