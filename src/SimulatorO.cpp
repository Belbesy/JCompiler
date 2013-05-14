/*
 * SimulatorO.cpp
 *
 *  Created on: May 14, 2013
 *      Author: raed
 */

#include "SimulatorO.h"

using namespace std;

SimulatorO::SimulatorO() {
	tokens.push_back("id");
	tokens.push_back("+");
	tokens.push_back("id");
	t = 0;
}

pair <string , int> SimulatorO::next_token()
{
	if(t >= (int)tokens.size())
		return make_pair("" , 0);
	else
		return make_pair(tokens[t++] , 0);
}

SimulatorO::~SimulatorO() {
}
