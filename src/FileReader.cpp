/*
 * FileReader.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: Saad Dawy
 */

#include "FileReader.h"
#include "StringOperations.h"
#include <iostream>

FileReader::FileReader() {
	// TODO Auto-generated constructor stub

}

FileReader::~FileReader() {
	// TODO Auto-generated destructor stub
}
/**
 * read the file and put it in the vector fileRead
 */
void FileReader::readTheFile(char fileName[]) {
	string line;
	ifstream myfile(fileName);

	if (myfile.is_open()) {
		while (myfile.good()) {
			//line
			getline(myfile, line);
			fileRead.push_back(line);

		}
		myfile.close();
	}

	else
		cout << "Unable to open file";

}
/**check if it's regular definition
 *
 */
bool FileReader::isDefinition(string exp) {
	if (exp.find(" =") != string::npos) {

		return true;
	}
	return false;
}
/**check if it's regular expression
 *
 */
bool FileReader::isExpression(string exp) {
	if (exp.find(": ") != string::npos) {

		return true;
	}
	return false;
}
//IMPORTANT : how to know later it's a keyword !!
bool FileReader::isKeyWord(string exp) {

	if (exp[0] == '{') {
		return true;
	}
	return false;
}
StringPair FileReader::makeDefinitonPair(string def) {
	int pos = def.find("=");
	string id = def.substr(0, pos);
	string defin = def.substr(pos + 1);
	StringPair pair;
	pair.id = id;
	pair.definition = defin;
	return pair;
}
StringPair FileReader::makeExpressionPair(string exp) {
	int pos = exp.find(": ");
	string id = exp.substr(0, pos);
	string defin = exp.substr(pos + 1);
	StringPair pair;
	pair.id = id;
	pair.definition = defin;
	return pair;
}
StringPair FileReader::makeKeyPair(string key) {
	StringPair pair;
	string id = "{KEYWORD}";
	string defi = key;
	pair.id = id;
	pair.definition = defi;
	return pair;
}
/**this methods loop on the strings of the file if it's expression then just put it in the
 * regularExpressions vector if it's keyword split it and add to each string a curly braces
 * then put each string in the regularExpressions vector
 * else if it's regular definitions create the NFA for it and put it in the defs multimap
 * return false if there is an error
 *
 */
bool FileReader::initializeForNFA() {

	for (int i = 0; i < fileRead.size(); ++i) {
		string curr = fileRead[i];
		if (isExpression(curr)) {
			regularExpressions.push_back(makeExpressionPair(curr));
		} else if (isKeyWord(curr)) {
			int pos = curr.find("{");
			curr.replace(pos, 1, "");
			pos = curr.find("}");
			curr.replace(pos, 1, "");
			vector<string> keywords = StringOperations::split(curr);
			for (int j = 0; j < keywords.size(); ++j) {
				string keyWord = "{"+keywords[j]+"}";
				regularExpressions.push_back(makeKeyPair(keyWord));
			}
		} else if (isDefinition(curr)) {
			defs.push_back(makeDefinitonPair(curr));

		} else {
			//error
		}
	}
	return true;
}
