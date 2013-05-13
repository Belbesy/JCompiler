/*
 * FirstEntry.h
 *
 *  Created on: May 13, 2013
 *      Author: raed
 */

#ifndef FIRSTENTRY_H_
#define FIRSTENTRY_H_

using namespace std;

class FirstEntry {
public:
	string terminal;
	int production;
	FirstEntry(string terminal_,int production_);
	virtual ~FirstEntry();
};

#endif /* FIRSTENTRY_H_ */
