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
	lex = sim->next_token().first;
	while (!lex.empty())
	{
		cout << lex << endl;
		lex = sim->next_token().first;
	}
	cout << "End of tokens" << endl;
}

void test_conc()
{
	vector<string> patterns;
	patterns.push_back("SA");
	patterns.push_back("SASA");
	ConcatenationHandler handler(patterns, "+");
	cout << handler.handle("\\)\\(SASAT\\*FS-GT\\((SA|T)X|(SA|66)X\\)*TY")
			<< endl;
}

/**
 * this case accepts (a|b)*abb
 */
void test_DFA1()
{
	vector<char> all_inputs;
	FSA_TABLE NFATable;
	vector<string> patterns;

	all_inputs.push_back('a');
	all_inputs.push_back('b');
	for (int i = 0; i < 11; i++)
		NFATable.push_back(new FA_State(i));
	NFATable[10]->acceptingState = true;
	NFATable[0]->AddTransition(char(8), NFATable[1]);
	NFATable[0]->AddTransition(char(8), NFATable[7]);
	NFATable[1]->AddTransition(char(8), NFATable[2]);
	NFATable[1]->AddTransition(char(8), NFATable[4]);
	NFATable[2]->AddTransition('a', NFATable[3]);
	NFATable[4]->AddTransition('b', NFATable[5]);
	NFATable[3]->AddTransition(char(8), NFATable[6]);
	NFATable[5]->AddTransition(char(8), NFATable[6]);
	NFATable[6]->AddTransition(char(8), NFATable[1]);
	NFATable[6]->AddTransition(char(8), NFATable[7]);
	NFATable[7]->AddTransition('a', NFATable[8]);
	NFATable[8]->AddTransition('b', NFATable[9]);
	NFATable[9]->AddTransition('b', NFATable[10]);
	DFA_Builder* DFA = new DFA_Builder(NFATable, patterns, all_inputs);
	cout << " Conversion Function " << endl;
	DFA->NFA_to_DFA();
}

/**
 * this DFA accepts ab|abb|a*b+
 * */

void test_DFA2()
{
	vector<char> all_inputs;
	FSA_TABLE NFATable;
	vector<string> patterns;

	all_inputs.push_back('a');
	all_inputs.push_back('b');
	for (int i = 0; i < 9; i++)
		NFATable.push_back(new FA_State(i));

	NFATable[2]->acceptingState = true;
	NFATable[6]->acceptingState = true;
	NFATable[8]->acceptingState = true;

	NFATable[0]->AddTransition(char(8), NFATable[1]);
	NFATable[1]->AddTransition('a', NFATable[2]);

	NFATable[0]->AddTransition(char(8), NFATable[3]);
	NFATable[3]->AddTransition('a', NFATable[4]);
	NFATable[4]->AddTransition('b', NFATable[5]);
	NFATable[5]->AddTransition('b', NFATable[6]);

	NFATable[0]->AddTransition(char(8), NFATable[7]);
	NFATable[7]->AddTransition('a', NFATable[7]);
	NFATable[7]->AddTransition('b', NFATable[8]);
	NFATable[8]->AddTransition('b', NFATable[8]);

	DFA_Builder* DFA = new DFA_Builder(NFATable, patterns, all_inputs);
	cout << " Conversion Function " << endl;
	DFA->NFA_to_DFA();
}
int main()
{
	FileReader *f = new FileReader();

//	f->readTheFile("test");
	vector<string> myv;
			myv.push_back("{boolean if}");
			myv.push_back("id: letter (letter|digit)*");

			myv.push_back("num: digit+|digit+ . (\L|E digits)");
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
	n->matchedExps = f->expressionsID;
	n->createAll();
//	cout << "  Start Conversion NFA to DFA " << endl;
//	vector<char> all_inputs(n->input.begin(), n->input.end());
//	for (int i = 0; i < (int) all_inputs.size(); i++)
//		cout << all_inputs[i] << endl;
////	n->input ==> all_inputs 			TODO make this conversion from set to vector
//	DFA_Builder* DFA = new DFA_Builder(n->NFATable, n->matchedExps, all_inputs);
//	cout << " Conversion Function " << endl;
//	DFA->NFA_to_DFA();
//	DFA->minimize_DFA();
//	cout << "  Start Simulator" << endl;
//	Simulator* sim = new Simulator(DFA->DFA, DFA->patterns);
//	sim->open_file("src_file");
//
//	// TODO change this according to simulator changes
//	string lex;
//	cout << "Start tokens" << endl;
//	lex = sim->next_token().first;
//	while (!lex.empty())
//	{
//		cout << lex << endl;
//		lex = sim->next_token().first;
//	}
//	cout << "End of tokens" << endl;

//	test_simulator();
//	test_conc();
//	test_DFA1();
//	test_DFA2();
	return 0;
}
