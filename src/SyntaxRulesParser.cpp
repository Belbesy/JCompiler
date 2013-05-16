/*
 * SyntaxRulesParser.cpp
 *
 *  Created on: May 11, 2013
 *      Author: raed
 */

#include "SyntaxRulesParser.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

string currentLHS;
vector<Term> newRHS = vector<Term>();

SyntaxRulesParser::SyntaxRulesParser() {
}

void trim(string& s) {
	size_t p = s.find_first_not_of(" ");
	s.erase(0, p);
	p = s.find_last_not_of(" ");
	if (string::npos != p)
		s.erase(p + 1);
}

void SyntaxRulesParser::addToLHS(string lhs, vector<Term> terms) {

//	Production* production;
	if (LHS_index.count(lhs) > 0) { // LHS already exists in the map
		int index = LHS_index[lhs];
//		production = productions.at(index);
		productions[index].addRHS(terms);
	} else { // first time to appear in the grammar
		Production production = Production(lhs);
		production.addRHS(terms);
		productions.push_back(production);
		LHS_index.insert(pair<string, int>(lhs, productions.size() - 1));
	}

}

bool SyntaxRulesParser::ifTerminal(string line) {
	bool isTerminal = false;
	if (line.find('\'') != string::npos || line.find("\\L") != string::npos)
		isTerminal = true;
	return isTerminal;
}

void SyntaxRulesParser::concatenation(string line) {

	stringstream ss(line);
	string item;
	vector<Term> terms;
	while (std::getline(ss, item, ' ')) { // split at ' '
		bool isTerminal = ifTerminal(item);
		if (isTerminal && line.find("\\L") == string::npos)
			item = item.substr(1, item.length() - 2);
		Term t =Term(item, isTerminal);
		terms.push_back(t);
	}
	addToLHS(currentLHS, terms);

}
void SyntaxRulesParser::checkRHS(string line) {
	if (line.find('|') != string::npos) { // many RHS
		stringstream ss(line);
		string item;
		while (std::getline(ss, item, '|')) { // split at '|'
			trim(item);
			if (item.find(' ') != string::npos) { // need concatenation
				concatenation(item);
			} else { // one term
				vector<Term> vec;
				bool isTerminal = ifTerminal(item);
				if (isTerminal)
					item = item.substr(1, item.length() - 2);
				vec.push_back(Term(item, isTerminal));
				addToLHS(currentLHS, vec);

			}
		}
	} else { // one RHS
		trim(line);
		if (line.find(' ') != string::npos) { // need concatenation
			concatenation(line);
		} else { // one term
			vector<Term> vec;
			bool isTerminal = ifTerminal(line);
			if (isTerminal)
				line = line.substr(1, line.length() - 2);
			vec.push_back(Term(line, isTerminal));
			// term added to a lhs found using map
			addToLHS(currentLHS, vec);
			// term added to a lhs found using map

		}
	}
}
/**
 * this method parses syntax rules file and add syntax productions to productions
 */

void SyntaxRulesParser::parse(const char* fileName) {
	productions.clear();
	LHS_index.clear();

	// read file and get productions

	string line;

	ifstream myfile(fileName);
	if (myfile.is_open()) {
		while (myfile.good()) {
			getline(myfile, line);

			if (line.at(0) == '#') { // new Production (new LHS)
				line = line.substr(2);
				int equalIndex = line.find_first_of('=', 0);
				currentLHS = line.substr(0, equalIndex);
				trim(currentLHS);
				string rhs = line.substr(equalIndex + 2);
				trim(rhs);
				checkRHS(rhs);

			} else { // RHS
				line = line.substr(2); // remove '|' from the beginning of the line
				trim(line);
				checkRHS(line);
			}
		}
		myfile.close();
	}

	else
		cout << "Unable to open file" << endl;
}

SyntaxRulesParser::~SyntaxRulesParser() {
}


