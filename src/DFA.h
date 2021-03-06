/*
 * DFA.h
 *
 *  Created on: Apr 6, 2013
 *      Author: raed
 */

#include "FA_State.h"

#ifndef DFA_H_
#define DFA_H_
#include "StringPair.h"
#include <map>
#include <string>
#include <queue>

using namespace std;



#define foreach(a, b) for(typeof((b).begin()) a = (b).begin(); a!=(b).end(); ++a)
#define all(a) (a).begin(), (a).end()

typedef set<int> SET;
typedef vector<SET> GROUP;

FSA_TABLE minimize(FSA_TABLE fa);
GROUP::iterator inside(SET a, GROUP g);
SET difference(SET Y, SET X);
SET intersect(SET Y, SET X);


class DFA_Builder {
public:

	// variables related to NFA
	FSA_TABLE NFATable;
	vector<string> patterns; // keywords first then patterns in the order they were declared
	vector<char> all_inputs;
//	int NFA_starting_state; TODO

	// variables related to DFA
	vector<FA_State*> DFA; // table to all DFA states
//	int DFA_starting; TODO

	DFA_Builder(FSA_TABLE NFATable_,vector<string> expression_,vector<char> all_inputs_);
	void NFA_to_DFA();
	vector<FA_State*> minimize(vector<FA_State*> fa);
	virtual ~DFA_Builder();

private:
	int state_id; // next state id
	int statesNum; // number of NFA_states
	bool* visited; // this array will help avoiding redundant
	FA_State** NFA_states;
	vector<vector<FA_State*> > DFA_states; // this table represents sub-states of each state
	int flush_new_state();
	void empty_closure(FA_State* state);
	int find(const vector<FA_State*>& state,const vector<vector<FA_State*> > states);
};

#endif /* DFA_H_ */
