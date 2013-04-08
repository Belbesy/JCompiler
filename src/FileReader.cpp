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
	expressionsID.push_back("keyword");
		expressionsID.push_back("punctuation");

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

bool FileReader::isKeyWord(string exp) {

	if (exp[0] == '{') {
		return true;
	}
	return false;
}
bool FileReader::isPunc(string exp) {

	if (exp[0] == '[') {
		return true;
	}
	return false;
}
StringPair FileReader::makeDefinitonPair(string def) {
	int pos = def.find("=");
	string id = def.substr(0, pos - 1);
	string defin = def.substr(pos + 2);
	StringPair pair;

	pair.id = id;
	pair.definition = defin;
	for (int var = 0; var < defs.size(); ++var) {
			StringPair currSP = defs[var];
			if(currSP.id==id){

				cout<<"Error : this definition "<<def<<" is already defined before";
				//return error with making id=-1 in order not to push it on the regularExpressions vector
				StringPair errorPair;
				errorPair.id="-1";
				return errorPair;
			}
		}
	return pair;
}
StringPair FileReader::makeExpressionPair(string exp) {
	int pos = exp.find(": ");
	string id = exp.substr(0, pos);
	string defin = exp.substr(pos + 2);
	StringPair pair;
	pair.id = id;
	pair.definition = defin;
	for (int var = 0; var < regularExpressions.size(); ++var) {
		StringPair currSP = regularExpressions[var];
		if(currSP.id==id){

			cout<<"Error : this Expression "<<exp<<" is already defined before";
			//return error with making id=-1 in order not to push it on the regularExpressions vector
			StringPair errorPair;
			errorPair.id="-1";
			return errorPair;
		}
	}
	expressionsID.push_back(id);
	return pair;
}
StringPair FileReader::makeKeyPair(string key) {
	StringPair pair;
	string id = key;
	string defi = "";
	for (int var = 0; var < key.size(); ++var) {
		defi = defi + key[var] + " ";
	}
	defi = defi.substr(0, defi.size() - 1);
	pair.id = "keyword";
	pair.definition = defi;
	return pair;
}
StringPair FileReader::makePuncPair(string key) {
	StringPair pair;
	string id = "punctuation";
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
			StringPair regex = makeExpressionPair(curr);
			if(regex.id!="-1")
			regularExpressions.push_back(regex);
		} else if (isKeyWord(curr)) {
			int pos = curr.find("{");
			curr.replace(pos, 1, "");
			pos = curr.find("}");
			curr.replace(pos, 1, "");
			vector<string> keywords = StringOperations::split(curr);
			for (int j = 0; j < keywords.size(); ++j) {
				string keyWord = keywords[j];
				StringPair kw = makeKeyPair(keyWord);

				regularExpressions.push_back(kw);

			}
		} else if (isPunc(curr)) {
			int pos = curr.find("[");
			curr.replace(pos, 1, "");
			pos = curr.find("]");
			curr.replace(pos, 1, "");
			vector<string> punct = StringOperations::split(curr);
			for (int j = 0; j < punct.size(); ++j) {
				string punctu = punct[j];
				regularExpressions.push_back(makePuncPair(punctu));
			}
		}

		else if (isDefinition(curr)) {
			StringPair pair = makeDefinitonPair(curr);
			if(pair.id!="-1")
			defs.push_back(pair);
		} else {
			//error
		}
	}
	return true;
}

