/*
 * ParserGenerator.h
 *
 *  Created on: May 13, 2013
 *      Author: raed
 */

#ifndef PARSERGENERATOR_H_
#define PARSERGENERATOR_H_

namespace std {

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
	vector<vector<Term> > follow;
	vector<map<string,int> > table;
	void constructTable();
};

} /* namespace std */
#endif /* PARSERGENERATOR_H_ */
