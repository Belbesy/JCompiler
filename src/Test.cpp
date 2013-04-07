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
#include "Simulator.h"
#include <map>
using namespace std;

void test_simulator()
{
	vector<FA_State*> DFA;
	vector<string> patterns;
	patterns.push_back("ERROR");
	patterns.push_back("id");
	patterns.push_back("ERROR");
	FA_State* state0 = new FA_State(0);
	FA_State* state1 = new FA_State(1);
	state1->acceptingState = true;
	state1->matchedPattern = 1;
	state0->AddTransition("a", state1);
	DFA.push_back(state0);
	DFA.push_back(state1);
	Simulator* sim = new Simulator(DFA, patterns);
	sim->open_file("sim_test");
	string lex;
	cout << "Start tokens" << endl;
	lex = sim->next_token();
	while (!lex.empty())
	{
		cout << lex << endl;
		lex = sim->next_token();
	}
	cout << "End of tokens" << endl;
}

int main()
{
	FileReader *f = new FileReader();

	f->readTheFile("test1");
	f->initializeForNFA();

	NFA *n = new NFA(f->regularExpressions);
	n->defs = f->defs;
	n->createAll();
//	test_simulator();
	return 0;
}
