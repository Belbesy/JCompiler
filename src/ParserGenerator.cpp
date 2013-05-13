/*
 * ParserGenerator.cpp
 *
 *  Created on: May 13, 2013
 *      Author: raed
 */

#include "ParserGenerator.h"

using namespace std;

ParserGenerator::ParserGenerator(vector<Production> productions_,
		map<string, int> LHS_index_)
{
	productions = productions_;
	LHS_index = LHS_index_;
}


bool ParserGenerator::generateParser()
{
	// calculate first and follow


	// construct parsing table
	return constructTable();
}


void printf_left_derivation(vector<string>& left_derivation)
{
	int size = (int)left_derivation.size();
	for(int i= 0 ; i < size;i++)
		printf("%s " , left_derivation[i].c_str());
	printf("\n");
}

void ParserGenerator::SimuParser(Simulator* sim)
{
	vector<string> left_derivation; // print handling
	string end_of_input = "$";
	stack<Term> parseStack;
	// push end of input
	parseStack.push(Term(end_of_input , true));
	// push initial state
	parseStack.push(Term(productions[0].LHS , false));
	left_derivation.push_back(parseStack.top().name); // print handling
	string lex;
	lex = sim->next_token().first;
	while (parseStack.size())
	{
		printf_left_derivation(left_derivation);  // print handling
		left_derivation.pop_back(); // print handling
		Term tof = parseStack.top();
		parseStack.pop();
		if(tof.isTerminal)
		{
			// top of stack is non temrinal symbol
			int index = LHS_index[tof.name];
			if(parseTable[index].count(lex)) // check whether table cell is empty or not
			{
				// apply input to non terminal
				// get which production to use
				int prodIndex = parseTable[index][lex];
				// get top of stack production index
				int tofProduction = LHS_index[tof.name];
				// get resulting production
				vector<Term> prod = productions[tofProduction].RHS[prodIndex];
				// add terms to stack
				for(int i = (int)prod.size()-1 ;i >= 0;i--)
				{
					parseStack.push(prod[i]);
					left_derivation.push_back(prod[i].name); // print handling
				}
			}else
			{
				// error . not valid input TODO
				printf("ERRRRRROOORRR");
			}
		}
		else
		{
			// top of stack is terminal symbol
			if(lex == tof.name)
			{
				lex = sim->next_token().first;
				if(lex.empty()) // add end of input symbol to the end of the stream
				{
					lex = end_of_input;
					end_of_input = ""; // to match it only once
				}
			}else{
				// error . not valid input TODO
				printf("ERRRRRROOORRR");
			}
		}
	}
}


bool ParserGenerator::constructTable()
{
	int nonTermCount = (int) productions.size();
	parseTable = new vector<map<string, int> >();
	for (int i = 0; i < nonTermCount; i++)
		parseTable.push_back(map<string, int>());
	for (int i = 0; i < nonTermCount; i++)
	{
		int size = (int) first[i].size();
		bool addFollow = false;
		for (int j = 0; j < size; j++)
		{
			FirstEntry entry = first[i][j];
			if (entry.terminal.size() > 0)
			{
				if (parseTable[i].count(entry.terminal) == 0)
					parseTable[i][entry.terminal] = entry.production;
				else
					return false;
			} else
				// epsilon transition will add follow set
				addFollow = true;
		}
		if (addFollow)
			if(!addFollowSet(i)) // error while add follow set
				return false;
	}
	// TODO print table
	return true;
}

bool ParserGenerator::addFollowSet(int nonTerminal)
{
	int size = follow[nonTerminal].size();
	for (int j = 0; j < size; j++)
	{
		FirstEntry entry = follow[nonTerminal][j];
		if (parseTable[nonTerminal].count(entry.terminal) == 0)
			parseTable[nonTerminal][entry.terminal] = EPSILON_TRANSITION;
		else
			return false;
	}
	return true;
}

ParserGenerator::~ParserGenerator()
{
}

