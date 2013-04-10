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
	NFATable[0]->AddTransition(EPSILON, NFATable[1]);
	NFATable[0]->AddTransition(EPSILON, NFATable[7]);
	NFATable[1]->AddTransition(EPSILON, NFATable[2]);
	NFATable[1]->AddTransition(EPSILON, NFATable[4]);
	NFATable[2]->AddTransition('a', NFATable[3]);
	NFATable[4]->AddTransition('b', NFATable[5]);
	NFATable[3]->AddTransition(EPSILON, NFATable[6]);
	NFATable[5]->AddTransition(EPSILON, NFATable[6]);
	NFATable[6]->AddTransition(EPSILON, NFATable[1]);
	NFATable[6]->AddTransition(EPSILON, NFATable[7]);
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

	NFATable[0]->AddTransition(EPSILON, NFATable[1]);
	NFATable[1]->AddTransition('a', NFATable[2]);

	NFATable[0]->AddTransition(EPSILON, NFATable[3]);
	NFATable[3]->AddTransition('a', NFATable[4]);
	NFATable[4]->AddTransition('b', NFATable[5]);
	NFATable[5]->AddTransition('b', NFATable[6]);

	NFATable[0]->AddTransition(EPSILON, NFATable[7]);
	NFATable[7]->AddTransition('a', NFATable[7]);
	NFATable[7]->AddTransition('b', NFATable[8]);
	NFATable[8]->AddTransition('b', NFATable[8]);

	DFA_Builder* DFA = new DFA_Builder(NFATable, patterns, all_inputs);
	cout << " Conversion Function " << endl;
	DFA->NFA_to_DFA();
}
int main()
{
	freopen("out.txt", "w", stdout); 					// TODO remove this line

	cout << "Enter file name for lexical rules" << endl;
	string file = "test";
//	cin >> file; 										// TODO uncomment this line
//	freopen("Transition_Table_log.txt", "w", stdout);  // TODO uncomment this line
	FileReader *f = new FileReader();

	f->readTheFile((char*)file.c_str());

	f->initializeForNFA();

	NFA *n = new NFA(f->regularExpressions);
	n->defs = f->defs;
	n->matchedExps = f->expressionsID;
	n->createAll();

	cout << "  Start Conversion NFA to DFA " << endl;
	vector<char> all_inputs;
	for (set<char>::iterator i = n->input.begin(); i != n->input.end(); i++)
	{
		all_inputs.push_back(*i);
		cout << *i << " " << int(*i) << endl;
	}

	DFA_Builder* DFA = new DFA_Builder(n->NFATable, n->matchedExps, all_inputs);
	cout << " Conversion Function " << endl;
	DFA->NFA_to_DFA();
//	DFA->minimize_DFA();





//	cout << "Enter file name for source code" << endl; //TODO uncomment this line
	string src_file = "src_file";
	//	cin >> src_file; 							//TODO uncomment this line

	cout << "  Start Simulator" << endl; 			// TODO remove this line
	Simulator* sim = new Simulator(DFA->DFA, DFA->patterns);
	sim->open_file(src_file.c_str());

//	freopen("LA_output.txt", "w", stdout); 			// TODO uncomment this line
	string lex;
	cout << "Start tokens" << endl; 				// TODO remove this line
	lex = sim->next_token().first;
	while (!lex.empty())
	{
		cout << lex << endl;
		lex = sim->next_token().first;
	}
	cout << "End of tokens" << endl; // TODO remove this line

//	test_simulator();
//	test_conc();
//	test_DFA1();
//	test_DFA2();
	return 0;
}
