/*
 * StringOperations.h
 *
 *  Created on: Mar 22, 2013
 *      Author: Saad Dawy
 */

#ifndef STRINGOPERATIONS_H_
#define STRINGOPERATIONS_H_
#include <vector>
#include <string>
#include "StringOperations.h"
#include <sstream>
using namespace std;
class StringOperations {
public:
	StringOperations();
	static vector<string> split(string str);
	virtual ~StringOperations();

};


#endif /* STRINGOPERATIONS_H_ */
