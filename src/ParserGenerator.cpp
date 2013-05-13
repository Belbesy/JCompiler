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

bool ParserGenerator::constructTable()
{
	int nonTermCount = (int) productions.size();
	table = new vector<map<string, int> >();
	for (int i = 0; i < nonTermCount; i++)
		table.push_back(map<string, int>());
	for (int i = 0; i < nonTermCount; i++)
	{
		int size = (int) first[i].size();
		bool addFollow = false;
		for (int j = 0; j < size; j++)
		{
			FirstEntry entry = first[i][j];
			if (entry.terminal.size() > 0)
			{
				if (table[i].count(entry.terminal) == 0)
					table[i][entry.terminal] = entry.production;
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
	return true;
}

bool ParserGenerator::addFollowSet(int nonTerminal)
{
	int size = follow[nonTerminal].size();
	for (int j = 0; j < size; j++)
	{
		FirstEntry entry = follow[nonTerminal][j];
		if (table[nonTerminal].count(entry.terminal) == 0)
			table[nonTerminal][entry.terminal] = EPSILON_TRANSITION;
		else
			return false;
	}
	return true;
}

ParserGenerator::~ParserGenerator()
{
}

