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

		f->readTheFile("test");
		f->initializeForNFA();

		NFA *n = new NFA(f->regularExpressions);
		n->defs = f->defs;
		n->createAll();

		return 0;
}
