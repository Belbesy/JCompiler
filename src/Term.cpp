/*
 * Term.cpp
 *
 *  Created on: May 11, 2013
 *      Author: raed
 */

#include "Term.h"

using namespace std;

Term::Term(string name_,bool isTerminal_)
{
	name= name_;
	isTerminal = isTerminal_;
}

Term::~Term()
{
}

