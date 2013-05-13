/*
 * ParserGenerator.cpp
 *
 *  Created on: May 13, 2013
 *      Author: raed
 */

#include "ParserGenerator.h"

namespace std {

ParserGenerator::ParserGenerator(vector<Production> productions_,map<string, int> LHS_index_)
{
	productions = productions_;
	LHS_index = LHS_index_;
}

void ParserGenerator::constructTable()
{
	int nonTermCount = (int) productions.size();
	table = new vector<map<string, int> >(nonTermCount);
	for (int i = 0; i < nonTermCount; i++)
	{
		table[i] = map<string, int>();
	}
	for(int i = 0; i < nonTermCount;i++)
	{
		int size = (int)first[i].size();
		for(int j = 0 ; j  < size ;j++)
		{
			table[i][] = i;
		}
	}
}



ParserGenerator::~ParserGenerator()
{
}

} /* namespace std */
