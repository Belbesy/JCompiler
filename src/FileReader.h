/*
 * FileReader.h
 *
 *  Created on: Mar 22, 2013
 *      Author: Saad Dawy
 */

#ifndef FILEREADER_H_
#define FILEREADER_H_
#include <string>
#include <vector>
#include "StringPair.h"
#include <sstream>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;
class FileReader {

protected:

public:
	/**vector contains the strings in the file **/
	vector<string> fileRead;
	/** vector contains all the regular
	 */
	vector<StringPair> defs;
	/**vector contains all the regular expression in the file to be used later */
	vector<StringPair> regularExpressions;

	FileReader();
	void readTheFile(char fileName[]);
	bool initializeForNFA();
	bool isExpression(string exp);
	bool isDefinition(string exp);
	bool isKeyWord(string exp);
	bool isPunc(string exp);
	StringPair makeExpressionPair(string exp);
	StringPair makeDefinitonPair(string def);
	StringPair makeKeyPair(string key);
	StringPair makePuncPair(string punc);

	virtual ~FileReader();
};

#endif /* FILEREADER_H_ */
