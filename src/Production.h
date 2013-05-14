/*
 * Production.h
 *
 *  Created on: May 11, 2013
 *      Author: raed
 */

#ifndef PRODUCTION_H_
#define PRODUCTION_H_
#include "Term.h"
#include <vector>
using namespace std;

class Production {
public:
	// LHS of the production (unique LHS)
	string LHS;
	/**
	 * LHS of a production each inner vector is a possible LHS production
	 * to make productions LHS unique (no 2 productions with the same LHS)
	 *
	 * for example
	 * stmt -> Type id | if cond then stmt
	 * stmt -> while cond then stmt end
	 *
	 * will be a production in the following form
	 *
	 * Production:
	 *    LHS = "stmt"
	 *    RHS = { {type , id} , {if , cond , then , stamt} , {while, cond, then, stmt, end}};
	 */
	vector<vector<Term> > RHS;
	Production();
	Production(string LHS_);
	// adds new possible RHS of LHS of the production
	void addRHS(vector<Term> newRHS);
	virtual ~Production();
};

#endif /* PRODUCTION_H_ */
