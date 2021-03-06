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
pair<string,int> Simulator::next_token()
{
	string ERROR = "";
	if(token.empty())
		if(src_file.good())
			src_file >> token;
		else
			return make_pair(ERROR , -1);

//	cout << " Token  " <<  token << endl;
	int state = 0 , i;
	for(i  = 0; i < (int)token.length();i++)
	{
		vector<FA_State*> next_state;
		DFA[state]->getTransition(token[i] , next_state);
		if(next_state.empty()) // no more matching get token
			break;
		state = next_state[0]->id; // always size = 1 ???
	}
//	cout <<  "State " << state << endl;
	if (DFA[state]->acceptingState && i > 0) // matched pattern
	{
//		cout << " AC " << endl;
		int pattern = DFA[state]->matched_pattern;
		string matched_part = token.substr(0,i);
		token = token.substr(i); // update input pointer
		//  insert in symbol table ?!!!
		string pattern_name = patterns[pattern];
		int sym_table_ptr = -1;
		if(pattern_name != "punctuation" && pattern_name != "keyword") // not keyword nor punctuation mark
		{
			int size = SYM_table.size();
			for(int i = 0; i < size;i++)
				if(SYM_table[i].first == matched_part)
					sym_table_ptr = i;
			if (sym_table_ptr == -1)
			{
				SYM_table.push_back(make_pair(matched_part, pattern_name));
				sym_table_ptr = SYM_table.size() - 1;
			}
		}else
			pattern_name = matched_part;
//		cout << matched_part << " ";

		return make_pair(pattern_name , sym_table_ptr);
	} else
	{
		// ERROR
		cout << "ERROR unmatched token " << token << endl;
		token = "";
		return make_pair("ERROR" , -1);
	}
}
Simulator::~Simulator()
{
}

