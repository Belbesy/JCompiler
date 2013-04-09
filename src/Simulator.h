/*
 * Simulator.h
 *
 *  Created on: Apr 7, 2013
 *      Author: raed
 */
#include "FA_State.h"
#include <string>
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#ifndef SIMULATOR_H_
#define SIMULATOR_H_


using namespace std;



class Simulator {
public:
	vector<pair<string , string> >SYM_table;

	Simulator(vector<FA_State*> DFA_ , vector<string> patterns_);
	bool open_file(const char* file);
	pair<string,int> next_token();
	virtual ~Simulator();
private:
	string token;
	vector<FA_State*> DFA;
	ifstream src_file;
	vector<string> patterns; // keywords first then patterns in the order they were declared
};

#endif /* SIMULATOR_H_ */
