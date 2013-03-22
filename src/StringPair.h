/*
 * StringPair.h
 *
 *  Created on: Mar 22, 2013
 *      Author: Saad Dawy
 */

#ifndef STRINGPAIR_H_
#define STRINGPAIR_H_
#include <string>
using namespace std;
class StringPair {
public:
	/** points for the name of the definition for example letter or id
	 */
	string id;
	/**
	 * points for the definition for example a-z or letter (letter)*
	 */
	string definition;
	StringPair();
	virtual ~StringPair();
};

#endif /* STRINGPAIR_H_ */
