/*
 * ConcatenationHandler.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: raed
 */

#include "ConcatenationHandler.h"


ConcatenationHandler::ConcatenationHandler(vector<string> patterns ,string separator_)
{
	separator = separator_;
	pattersnNum = (int) patterns.size();
	for (int i = 0; i < pattersnNum; i++)
	{
		build(patterns[i]);
	}
	matched_len = new int[pattersnNum];
}

int ConcatenationHandler::getL(char a, int l,int patternID)
{
	while (l > 0 && a != p[patternID][l])
		l = F[patternID][l - 1];
	if (a == p[patternID][l])
		return l + 1;
	return l;
}

void ConcatenationHandler::build(string pattern)
{
	int len = pattern.length();

	p.push_back((char*)pattern.c_str());

	F.push_back(new int[len]);

	length.push_back(len);
	int id = p.size()-1;
	F[id][0] = 0;
	for (int i = 1; i < len; i++)
		F[id][i] = getL(p[id][i], F[id][i - 1] , id);
}

void ConcatenationHandler::reset_all()
{
	for (int i = 0; i < (int) F.size(); i++)
		matched_len[i] = 0;
}
string ConcatenationHandler::handle(string lexeme)
{
	int len = lexeme.length();
	int* matches = new int[len];
	reset_all();
	for(int i = 0; i < len;i++)
	{
		matches[i] = 1;
		for(int j = 0; j < pattersnNum;j++)
		{
			matched_len[j] = getL(lexeme[i], matched_len[j] , j);
			if (matched_len[j] == length[j]) // match
			{
				// mark the start of the match with pattern length
				matches[i - matched_len[j] + 1] = matched_len[j];
				reset_all();
				break;
			}
		}
	}
	string result = "";
	int i = 0 , token_len;
	while(i < len)
	{
		token_len = matches[i];
		if(!result.empty())
			result += separator;
		result += lexeme.substr(i , token_len);
		i+= token_len;
	}
	return result;
}

ConcatenationHandler::~ConcatenationHandler()
{
}

