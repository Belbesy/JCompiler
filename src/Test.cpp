//============================================================================
// Name        : Test.cpp
// Author      : Omniaa
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "FileReader.h"
#include <vector>
#include "NFA.h"
#include <map>
using namespace std;

int main() {
	FileReader *f = new FileReader();
	vector<string> myv;
	myv.push_back("{boolean if}");
	myv.push_back("id: letter (letter|digit)*");
	myv.push_back("letters: letter*");
	myv.push_back("letter = a-z|A-Z");
	myv.push_back("digit = 0-9");
	myv.push_back("digits = digit+");
	myv.push_back("addop: \\+|-");
	myv.push_back("mulop: \\*|/");
	myv.push_back("[; , \\( \\) { }]");
	f->fileRead = myv;
	f->initializeForNFA();

	NFA *n = new NFA(f->regularExpressions);
	n->defs = f->defs;
	n->matchedExps =f->expressionsID;
	n->createAll();
	std::set<char> myset = n->input;
	std::set<char>::iterator it;

	std::cout << "myset contains:";
	for (it = myset.begin(); it != myset.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

//	cout << n->getExpression("0-a");
//	n->push_NFA("a-z");
	return 0;
}
