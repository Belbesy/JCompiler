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
#include "Production.h"
#include "ParserGenerator.h"
#include "SyntaxRulesParser.h"
#include "ConcatenationHandler.h"
#include "DFA.h"
#include <map>
using namespace std;

void test_simulator() {
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
	while (!lex.empty()) {
		cout << lex << endl;
		lex = sim->next_token().first;
	}
	cout << "End of tokens" << endl;
}

void test_conc() {
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
void test_DFA1() {
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

void test_DFA2() {
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

void test_parser() {
	vector<Production> prod;
	vector<Term> v1, v2, v3, v4, v5, v6, v7, v8;

	v1.push_back(Term("T", false));
	v1.push_back(Term("E_", false));

	v2.push_back(Term("+", true));
	v2.push_back(Term("T", false));
	v2.push_back(Term("E_", false));

	v3.push_back(Term("", true));

	v4.push_back(Term("F", false));
	v4.push_back(Term("T_", false));

	v5.push_back(Term("*", true));
	v5.push_back(Term("F", false));
	v5.push_back(Term("T_", false));

	v6.push_back(Term("", true));

	v7.push_back(Term("(", true));
	v7.push_back(Term("E", false));
	v7.push_back(Term(")", true));

	v8.push_back(Term("id", true));

	Production p1("E");
	Production p2("E_");
	Production p3("T");
	Production p4("T_");
	Production p5("F");
	p1.addRHS(v1);
	p2.addRHS(v2);
	p2.addRHS(v3);
	p3.addRHS(v4);
	p4.addRHS(v5);
	p4.addRHS(v6);
	p5.addRHS(v7);
	p5.addRHS(v8);

	prod.push_back(p1);
	prod.push_back(p2);
	prod.push_back(p3);
	prod.push_back(p4);
	prod.push_back(p5);

	map<string, int> ind;
	ind["E"] = 0;
	ind["E_"] = 1;
	ind["T"] = 2;
	ind["T_"] = 3;
	ind["F"] = 4;

	ParserGenerator* parser = new ParserGenerator(prod, ind);
	parser->generateParser();
	//	parser->SimuParser(new SimulatorO());
}

void test_parser_parseFile() {
	string file = "syntax_test.txt";
	SyntaxRulesParser* syntaxParser = new SyntaxRulesParser();
	syntaxParser->parse(file.c_str());
	vector<Production> prods = syntaxParser->productions;

	printf("Print Productions...\n");
	for (size_t i = 0; i < prods.size(); i++) {
		Production prod = prods[i];
		printf("%s -> ", prod.LHS.c_str());
		for (size_t j = 0; j < prod.RHS.size(); j++) {
			for (size_t k = 0; k < prod.RHS[j].size(); k++)
				printf("(%s , %d)", prod.RHS[j][k].name.c_str(),
						prod.RHS[j][k].isTerminal);
			puts("");
			if (j < prod.RHS.size() - 1)
				printf("     |");
		}
		puts("");
	}
	printf("Index test...\n");
	for (size_t i = 0; i < prods.size(); i++) {
		Production prod = prods[i];
		printf("%s -> %d\n", prod.LHS.c_str(),
				syntaxParser->LHS_index[prod.LHS]);
	}
}
int main() {
	cout << "Enter file name for lexical rules" << endl;
	string file = "test";
	cin >> file;
	FileReader *f = new FileReader();
	f->readTheFile((char*) file.c_str());
	f->initializeForNFA();
	NFA *n = new NFA(f->regularExpressions);
	n->defs = f->defs;
	n->matchedExps = f->expressionsID;
	n->createAll();
	cout << "  Start Conversion NFA to DFA " << endl;
	vector<char> all_inputs;
	for (set<char>::iterator i = n->input.begin(); i != n->input.end(); i++) {
		all_inputs.push_back(*i);
	}
	DFA_Builder* DFA = new DFA_Builder(n->NFATable, n->matchedExps, all_inputs);
	cout << " Conversion Function " << endl;
	DFA->NFA_to_DFA();

	cout << "Enter file name for syntax rules" << endl;
	string syntax_file = "syntax_test.txt";
	cin >> syntax_file;
	// read syntax rules file and generate productions
	SyntaxRulesParser* syntaxParser = new SyntaxRulesParser();
	syntaxParser->parse(syntax_file.c_str());
	vector<Production> prods = syntaxParser->productions;
	printf("Print Productions...\n");
		for (size_t i = 0; i < prods.size(); i++) {
			Production prod = prods[i];
			printf("%s -> ", prod.LHS.c_str());
			for (size_t j = 0; j < prod.RHS.size(); j++) {
				for (size_t k = 0; k < prod.RHS[j].size(); k++)
					printf("%s ", prod.RHS[j][k].name.c_str());
				puts("");
				if (j < prod.RHS.size() - 1)
					printf("     |");
			}
			puts("");
		}
		cout << endl;
	// Generate parser
	ParserGenerator* parserGenerator  = new ParserGenerator(syntaxParser->productions , syntaxParser->LHS_index);
	bool isLL1 = parserGenerator->generateParser();
	if(!isLL1)
	{
		cout << "Not LL(1) grammar" << endl;
		return 0;
	}
	// read program source file
	cout << "Enter file name for source code" << endl;
	string src_file = "src_file";
	cin >> src_file;
	Simulator* sim = new Simulator(DFA->DFA, DFA->patterns);
	sim->open_file(src_file.c_str());

	// start reading src file tokens and parse it using LL(1) parser
//	freopen("Parser_output.txt", "w", stdout);
	parserGenerator->SimuParser(sim);
//		string lex;
//		lex = sim->next_token().first;
//		while (!lex.empty())
//		{
//			cout << lex << endl;
//			lex = sim->next_token().first;
//		}

	fclose(stdout);

	//	test_parser();
	//	test_parser_parseFile();
	return 0;
}
