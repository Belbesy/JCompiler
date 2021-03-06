/*
 * SyntaxRulesParser.h
 *
 *  Created on: May 11, 2013
 *      Author: raed
 */

#ifndef SYNTAXRULESPARSER_H_
#define SYNTAXRULESPARSER_H_

#include "Production.h"
#include <map>
using namespace std;

class SyntaxRulesParser {
public:
	// vector of syntax rules productions with no duplicate LHS (each LHS has one production)
	vector<Production> productions;
	// this map is used to get LHS production index in the productions vector (to avoid linear search while implementing first and follow sets)
	map<string , int> LHS_index;
	SyntaxRulesParser();
	void parse(const char* fileName);
	void concatenation(string line);
	bool ifTerminal(string line) ;
	void addToLHS(string lhs,vector<Term> terms);
	void checkRHS(string line);
	virtual ~SyntaxRulesParser();

};

#endif /* SYNTAXRULESPARSER_H_ */
