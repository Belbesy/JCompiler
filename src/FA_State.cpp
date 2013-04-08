/* 
 * File:   FA_State.cpp
 * Author: Omnia Dawy
 * 
 * Created on March 21, 2013, 5:06 PM
 */

#include "FA_State.h"
using namespace std;

/*Constructors*/
FA_State::FA_State() {
	id = -1;
	acceptingState = false;
	matched_pattern=-1;
}


FA_State::FA_State(int s_id) {
	id = s_id;
	acceptingState = false;
	matched_pattern=-1;

}

FA_State::FA_State(set<FA_State*> state, int s_id) {
	id = s_id;
	transitions_from = state;
	matched_pattern=-1;
	acceptingState = false;
}

FA_State::~FA_State() {
}

//-----------------------------------------METHODS-------------------------------------------//

/*
 *@param str : string contains regular expression.
 *@param state : FA_State represents the state the transition goes.
 * Add transition from this state to other*/
void FA_State::AddTransition(char str, FA_State* s) {
	//add transition to multimap
	transitions_to.insert(make_pair(str, s));

}
/**@param state : state to be removed
 * Remove state from its transitions
 * */
void FA_State::removeTransition(FA_State *state) {
	multimap<char, FA_State*>::iterator it;
	for (it = transitions_to.begin(); it != transitions_to.end();) {
		FA_State *toState = it->second;
		if (toState == state)
			transitions_to.erase(it++);
		else
			++it;
	}
}
/**
 *@param :: input : string contains regular expression.
 *@param :: states : will contain all transition from this state on specific input.
 *Get all Transitions from this state on Specific input.
 */
void FA_State::getTransition(char input, vector<FA_State*> &states) {
	// Iterate through all values with the key chInput
	multimap<char, FA_State*>::iterator iter;
	for (iter = transitions_to.lower_bound(input);
			iter != transitions_to.upper_bound(input); ++iter) {
		FA_State *pState = iter->second;
		states.push_back(pState);
	}
}
//! Override the assignment operator
void FA_State::operator=(const FA_State& other) {
	transitions_to = other.transitions_to;
	id = other.id;
	transitions_from = other.transitions_from;
}

//! Override the comparison operator
bool FA_State::operator==(const FA_State& other) {
	if (transitions_from.empty())
		return (id == other.id);
	else
		return (transitions_from == other.transitions_from);
}
void FA_State::toString() {
	multimap<char, FA_State*>::iterator it;
	for (it = transitions_to.begin(); it != transitions_to.end();) {
		char str = it->first;
		string s = " ";
		s += str;
		FA_State* state = it->second;
		cout << "From state " << id << s << "  To state " << state->id <<" matchedExpression "<<state->matched_pattern<<" Accepting  " <<state->acceptingState<<  endl;
		it++;
	}

}
