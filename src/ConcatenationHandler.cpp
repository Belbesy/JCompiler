/*
 * ConcatenationHandler.cpp
 *
 *  Created on: Apr 7, 2013
 *      Author: raed
 */

#include "ConcatenationHandler.h"


const string esp_patterns[6] = {"\\-" , "\\|" , "\\+" , "\\*" , "\\(","\\)"};


ConcatenationHandler::ConcatenationHandler(vector<string> patterns ,string separator_)
{
	separator = separator_;
	patternsNum = (int) patterns.size();
	for (int i = 0; i < patternsNum; i++)
	{
		build(patterns[i]);
	}
	// just handle operators special case
	for(int k  =0 ; k < 6;k++)
		build(esp_patterns[k]);
	patternsNum += 6;

	matched_len = new int[patternsNum];
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

bool ConcatenationHandler::isOperator(const char ch)
{
	return ch== '-' || ch == '|' || ch == '+' ||ch =='*' ||ch == '(' || ch == ')';
}


string ConcatenationHandler::handle(string lexeme)
{
	int len = lexeme.length();
	int* matches = new int[len];
	reset_all();
	for(int i = 0; i < len;i++)
	{
		matches[i] = 1;
		for(int j = 0; j < patternsNum;j++)
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
		if (token_len == 1 && i + 1 < len && lexeme[i + 1] == '-')
			token_len = 3;
		if (!result.empty())
		{
			if(token_len > 1  || (lexeme[i] != '|' && lexeme[i] != '+' && lexeme[i] != '*' && lexeme[i] != ')'))
			{
				char last = result[result.length()-1];
				char prev = 0;
				if(result.length() > 1)
					prev = result[result.length()-2];
				if(prev == '\\' || (last != '|' && last != '('))
					result += separator;
			}
		}
		result += lexeme.substr(i , token_len);
		i+= token_len;
	}
	return result;
}

ConcatenationHandler::~ConcatenationHandler()
{
}

