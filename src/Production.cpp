/*
 * Production.cpp
 *
 *  Created on: May 11, 2013
 *      Author: raed
 */

#include "Production.h"

using namespace std;

Production::Production(string LHS_)
{
	LHS = LHS_;
	RHS = vector<vector<Term>>();
}

void Production::addRHS(vector<Term> newRHS)
{
	RHS.push_back(newRHS);
}

Production::~Production()
{
}

