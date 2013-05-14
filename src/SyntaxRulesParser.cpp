/*
 * SyntaxRulesParser.cpp
 *
 *  Created on: May 11, 2013
 *      Author: raed
 */

#include "SyntaxRulesParser.h"


using namespace std;

SyntaxRulesParser::SyntaxRulesParser()
{
}

/**
 * this method parses syntax rules file and add syntax productions to productions
 */
void SyntaxRulesParser::parse(const char* fileName)
{
	productions.clear();
	LHS_index.clear();
	// read file and get productions

}

SyntaxRulesParser::~SyntaxRulesParser()
{
}

