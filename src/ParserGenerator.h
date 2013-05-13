/*
 * ParserGenerator.h
 *
 *  Created on: May 13, 2013
 *      Author: raed
 */

#ifndef PARSERGENERATOR_H_
#define PARSERGENERATOR_H_
#include <map>
#include "Production.h"
#include "FirstEntry.h"

using namespace std;
const int EPSILON_TRANSITION = -1;

class ParserGenerator {
public:
	// vector of syntax rules productions with no duplicate LHS (each LHS has one production)
	vector<Production> productions;
	// this map is used to get LHS production index in the productions vector (to avoid linear search while implementing first and follow sets)
	map<string, int> LHS_index;
	ParserGenerator(vector<Production> productions_,map<string, int> LHS_index_);
	virtual ~ParserGenerator();
private:
	vector<vector<FirstEntry> > first;
	vector<vector<FirstEntry> > follow;
	// table[state][input]  =  indx > 0 (got to production [state][indx]) || ind == -1 (epsilon) || nothing = error
	vector<map<string,int> > table;
	bool constructTable();
	bool addFollowSet(int nonTerminal);
};

#endif /* PARSERGENERATOR_H_ */
