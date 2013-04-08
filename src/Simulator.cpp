/*
 * Simulator.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: raed
 */

#include "Simulator.h"


Simulator::Simulator(vector<FA_State*> DFA_ , vector<string> patterns_)
{
	DFA = DFA_;
	patterns = patterns_;
}



bool Simulator::open_file(const char* file)
{
	src_file.open(file ,ifstream::in);
	return src_file.is_open();
}

/**
 * assuming space as separator and space as a whitespace
 */
string Simulator::next_token()
{
	string ERROR = "";
	if(token.empty())
		if(src_file.good())
			src_file >> token;
		else
			return ERROR;
	int state = 0 , i;
	for(i  = 0; i < (int)token.length();i++)
	{
		vector<FA_State*> next_state;
		DFA[state]->getTransition(token[i] , next_state);
		if(next_state.empty()) // no more matching get token
			break;
		state = next_state[0]->id; // always size = 1 ???
	}
	if (DFA[state]->acceptingState) // matched pattern
	{
		int pattern = DFA[state]->matched_pattern;
		token = token.substr(i); // update input pointer
		// TODO insert in symbol table ?!!!
		return patterns[pattern];
	} else
	{
		// TODO i == 0 ??!!
		printf("ERROR"); // TODO remove the whole token or whatever & don't return null
		return ERROR;
	}
	printf("ERROR"); // TODO token wasn't matched
	return ERROR;
}
Simulator::~Simulator()
{
}

