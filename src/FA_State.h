/* 
 * File:   FA_State.h
 * Author: Omnia Dawy
 *
 * Created on March 21, 2013, 5:06 PM
 */
#include <string>
#include <stack>
#include <map>
#include <deque>
#include <vector>
#include <set>
#include <algorithm>
#include <list>
#include <iostream>
using namespace std;
#ifndef FA_STATE_H
#define	FA_STATE_H

/*
 * The state of the finite Automata
 *
 * */
class FA_State {
public:
	/*----------------------------------------Attributes---------------------------------------*/

	/**id of this state*/
	int id;
	/**Transitions from this state to others*/
	multimap<char, FA_State*> transitions_to;
	/**if this state is accepting state or not*/
	bool acceptingState;
	/**Transitions from others to this state*/
	set<FA_State*> transitions_from;
	/*index to the accepting token*/
	int matched_pattern;
	/*--------------------------------------Constructors-----------------------------------------*/
	FA_State();
	FA_State(int s_id);
	FA_State(set<FA_State*> state, int s_id);
	virtual ~FA_State();
	/*----------------------------------------METHODS--------------------------------------------*/
	void AddTransition(char input, FA_State* state);
	void removeTransition(FA_State* state);
	void getTransition(char input, vector<FA_State*> &states);
	void operator=(const FA_State& other);
	bool operator==(const FA_State& other);
	void toString();

};

#endif	/* FA_STATE_H */


// NFA Table
typedef deque<FA_State*> FSA_TABLE;

