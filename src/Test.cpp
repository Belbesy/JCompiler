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
#include "ConcatenationHandler.h"
#include "DFA.h"
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
	state1->matched_pattern = 1;
	state0->AddTransition('a', state1);
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

void test_conc()
{
	vector<string> patterns;
	patterns.push_back("SA");
	patterns.push_back("SASA");
	ConcatenationHandler handler(patterns, "+");
	cout << handler.handle("\\)\\(SASAT\\*FS-GT\\((SA|T)X|(SA|66)X\\)*TY") << endl;
}

int main()
{
	FileReader *f = new FileReader();

	f->readTheFile("test");
	f->initializeForNFA();

	NFA *n = new NFA(f->regularExpressions);
	n->defs = f->defs;
	n->matchedExps=f->expressionsID;
	n->createAll();
	cout << "  Start Conversion NFA to DFA " << endl;
	vector<char> all_inputs(n->input.begin() , n->input.end());
	for(int i = 0 ; i < (int)all_inputs.size();i++)
		cout << all_inputs[i] << endl;
//	n->input ==> all_inputs 			TODO make this conversion from set to vector
	DFA_Builder* DFA = new DFA_Builder(n->NFATable , n->matchedExps , all_inputs);
	cout << " Conversion Function " << endl;
	DFA->NFA_to_DFA();
//	DFA->minimize_DFA();
//	cout << "  Start Simulator" << endl;
//	Simulator* sim =  new Simulator(DFA->DFA , DFA->patterns);
//	sim->open_file("src_file");
//
//	// TODO change this according to simulator changes
//	string lex;
//	cout << "Start tokens" << endl;
//	lex = sim->next_token();
//	while (!lex.empty())
//	{
//		cout << lex << endl;
//		lex = sim->next_token();
//	}
//	cout << "End of tokens" << endl;

//	test_simulator();
//	test_conc();
	return 0;
}
