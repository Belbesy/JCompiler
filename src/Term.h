/*
 * Term.h
 *
 *  Created on: May 11, 2013
 *      Author: raed
 */

#ifndef TERM_H_
#define TERM_H_
#include "string.h"


using namespace std;

class Term {
public:
	// name of the term
	string name;
	// boolean indicates whether this term is terminal of not
	bool isTerminal;
	Term(string name, bool isTerminal);
	virtual ~Term();
};

#endif /* TERM_H_ */
