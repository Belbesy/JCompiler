/*
 * ParserGenerator.cpp
 *
 *  Created on: May 13, 2013
 *      Author: raed
 */

#include "ParserGenerator.h"
#include <cstdio>
#include <set>
using namespace std;

ParserGenerator::ParserGenerator(vector<Production> productions_, map<string,
		int> LHS_index_) {
	productions = productions_;
	LHS_index = LHS_index_;

}

bool ParserGenerator::generateParser() {
	first = vector<vector<FirstEntry> > (productions.size());
	follow = vector<vector<FirstEntry> > (productions.size());
	// calculate first and follow
	initFirst();
	//	for (size_t i = 0; i < productions.size(); i++) {
	//		printf("(%s) first->  ", productions[i].LHS.c_str());
	//		for (size_t j = 0; j < first[i].size(); j++) {
	//			printf("[ '%s'   <=>%d], ", first[i][j].terminal.c_str(),
	//					first[i][j].production);
	//		}
	//		puts("");
	//	}
	initFollow();

	puts("===============================");
	puts("===============================");

	//	for (size_t i = 0; i < productions.size(); i++) {
	//		printf("(%s) follow->  ", productions[i].LHS.c_str());
	//		for (size_t j = 0; j < follow[i].size(); j++) {
	//			printf("[ '%s' <=>  %d], ", follow[i][j].terminal.c_str(),
	//					follow[i][j].production);
	//		}
	//		puts("");
	//	}
	// construct parsing table
	return constructTable();
}

/**
 * method to print content of a vector
 */
void printf_left_derivation(vector<string>& left_derivation) {
	int size = (int) left_derivation.size();
	for (int i = 0; i < size; i++)
		printf("%s ", left_derivation[i].c_str());
	cout << endl;
}
/**
 * method to print content of a vector
 */
void replace_left_most_nonTerminal(vector<string>& left_derivation, string tof,
		vector<Term>& prod) {
	size_t i;
	for (i = 0; i < left_derivation.size(); i++)
		if (left_derivation[i] == tof)
			break;
	stack<string> left_d_stack;
	size_t ptr = left_derivation.size() - 1;
	string left_most = left_derivation.back();
	left_derivation.pop_back();
	while (ptr > i) {
		left_d_stack.push(left_most);
		left_most = left_derivation.back();
		left_derivation.pop_back();
		ptr--;
	}
	for (i = 0; i < prod.size(); i++)
		left_derivation.push_back(prod[i].name);
	while (!left_d_stack.empty()) {
		left_derivation.push_back(left_d_stack.top());
		left_d_stack.pop();
	}
}
bool contains(vector<FirstEntry> p, string s) {
	for (size_t i = 0; i < p.size(); i++)
		if (p[i].terminal == s)
			return true;
	return false;
}
/**
 * this method parses (Syntax analysis) tokens that come from simulator (LA)
 */
void ParserGenerator::SimuParser(Simulator* sim) {
	vector<string> left_derivation; // print handling
	string end_of_input = "$";
	stack<Term> parseStack;
	// push end of input
	parseStack.push(Term(end_of_input, true));
	// push initial state
	parseStack.push(Term(productions[0].LHS, false));
	left_derivation.push_back(parseStack.top().name); // print handling
	string lex;
	lex = sim->next_token().first;
	while (parseStack.size()) {
		printf_left_derivation(left_derivation); // print handling
		Term tof = parseStack.top();
		parseStack.pop();
		if (!tof.isTerminal) {
			// top of stack is non terminal symbol
			int index = LHS_index[tof.name];
			if (parseTable[index].count(lex)) // check whether table cell is empty or not
			{
				// apply input to non terminal
				// get which production to use
				int prodIndex = parseTable[index][lex];
				if (prodIndex != EPSILON_TRANSITION) {
					// get top of stack production index
					int tofProduction = LHS_index[tof.name];
					// get resulting production
					vector<Term> prod =
							productions[tofProduction].RHS[prodIndex];
					// add terms to stack
					for (int i = (int) prod.size() - 1; i >= 0; i--)
						parseStack.push(prod[i]);
					// print handling
					if (!tof.isTerminal)
						replace_left_most_nonTerminal(left_derivation,
								tof.name, prod); // print handling
				} else {
					vector<Term> prod;
					replace_left_most_nonTerminal(left_derivation, tof.name,
							prod);
				}
			} else {
				// error . not valid input
				printf("ERRRRRROOORRR\n");
				// search for the first synchronization symbol
				while (!contains(follow[index], lex) && !lex.empty()) // error panic mode , follow heuristic
				{
					printf("unmatched input%s , discard input\n", lex.c_str());
					lex = sim->next_token().first;
					if (lex.empty()) // add end of input symbol to the end of the stream
					{
						lex = end_of_input;
						end_of_input = ""; // to match it only once
					}
				}
			}
		} else {
			// top of stack is terminal symbol
			if (lex == tof.name) {
				lex = sim->next_token().first;
				//				cout << "TOKEN TOKEN :" << lex << endl;
				if (lex.empty()) // add end of input symbol to the end of the stream
				{
					lex = end_of_input;
					end_of_input = ""; // to match it only once
				}
			} else {
				// error . not valid input
				printf("Error missing token %s \n", tof.name.c_str());
			}
		}
	}
}

/**
 * this method constructs parsing table and follow and first sets should be calculated before
 * calling this method
 *
 * returns whether this grammar is LL(1) grammar or not
 */
bool ParserGenerator::constructTable() {
	int nonTermCount = (int) productions.size();
	parseTable = vector<map<string, int> > ();
	for (int i = 0; i < nonTermCount; i++)
		parseTable.push_back(map<string, int> ());
	for (int i = 0; i < nonTermCount; i++) {
		int size = (int) first[i].size();
		bool addFollow = false;
		for (int j = 0; j < size; j++) {
			FirstEntry entry = first[i][j];
			if (entry.terminal.size() > 0) {
				if (parseTable[i].count(entry.terminal) == 0)
					parseTable[i][entry.terminal] = entry.production;
				else {
					cout << "Input " << entry.terminal
							<< " with 2 productions state " << i << " , "
							<< parseTable[i][entry.terminal] << " and "
							<< entry.production << endl;
					return false; /// not LL(1) grammar
				}
			} else
				// epsilon transition will add follow set
				addFollow = true;
		}
		if (addFollow)
			if (!addFollowSet(i)) // error while add follow set
				return false; // not LL(1) grammer
	}
	set<string> terminals;

	terminals.insert("$");
	// print table
	printf("                 <<<<<<<<<<<<<< Productions >>>>>>>>>>>>>>>> \n");
	for (size_t i = 0; i < productions.size(); i++) {
		printf("%s -> ", productions[i].LHS.c_str());
		for (size_t j = 0; j < productions[i].RHS.size(); j++) {
			for (size_t k = 0; k < productions[i].RHS[j].size(); k++) {
				if (productions[i].RHS[j][k].isTerminal
						&& productions[i].RHS[j][k].name != "")
					terminals.insert(productions[i].RHS[j][k].name);
				printf("%s ", productions[i].RHS[j][k].name.c_str());
			}
			if (j < productions[i].RHS.size() - 1)
				printf("|");
		}
		printf("\n");
	}
	printf("                <<<<<<<<<<<<<< Parse Table >>>>>>>>>>>>>>>> \n");
	for (size_t j = 0; j < parseTable.size(); j++) {
		printf("%s -> ", productions[j].LHS.c_str());
		for (set<string>::iterator i = terminals.begin(); i != terminals.end(); i++) {
			string terminal = *i;
			if (parseTable[j].count(terminal)) {
				if (parseTable[j][terminal] == EPSILON_TRANSITION)
					printf("('%s' ,'') ", terminal.c_str());
				else
					printf("('%s' ,%d) ", terminal.c_str(),
							parseTable[j][terminal]);
			} else
				printf("('%s',ERROR) ", terminal.c_str());
		}
		puts("");
	}
	cout << endl;
	return true;
}

/**
 * this method adds follow set of the non terminal "nonTerminal" to parsing table
 *
 * returns whether an error occurred or not while inserting in the table
 */
bool ParserGenerator::addFollowSet(int nonTerminal) {
	int size = follow[nonTerminal].size();
	for (int j = 0; j < size; j++) {
		FirstEntry entry = follow[nonTerminal][j];
		if (parseTable[nonTerminal].count(entry.terminal) == 0)
			parseTable[nonTerminal][entry.terminal] = EPSILON_TRANSITION;
		else {
			cout << "Input " << entry.terminal << " with 2 productions state "
					<< nonTerminal << " , "
					<< parseTable[nonTerminal][entry.terminal]
					<< " and  Epsilon" << endl;
			return false; // not LL(1) grammar
		}
	}
	return true;
}

void ParserGenerator::initFirst() {
	for (size_t s = 0; s < productions.size(); s++)
		find_first(s);
}
vector<FirstEntry> ParserGenerator::find_first(int p) {
	if (this->first[p].size())
		return this->first[p];

	vector<FirstEntry> ans;
	vector<vector<Term> > rules = this->productions[p].RHS;

	for (size_t i = 0; i < rules.size(); i++) {
		vector<Term> terms = rules[i];
		for (size_t j = 0; j < terms.size(); j++) {
			Term term = terms[j];
			if (term.isTerminal) {
				if (term.name == "") {
					if (j == terms.size() - 1)
						ans.push_back(FirstEntry("", i));
					continue;
				}
				ans.push_back(FirstEntry(term.name, i));
				break;
			} else {
				int index = this->LHS_index[term.name];
				vector<FirstEntry> child_firsts;
				if (index != p) {
					child_firsts = find_first(index);
					bool has_epsilon = false;
					for (size_t k = 0; k < child_firsts.size(); k++) {
						if (child_firsts[k].terminal == "") {
							has_epsilon = true;
						} else {
							ans.push_back(FirstEntry(child_firsts[k].terminal,
									i));
						}
					}
					if (has_epsilon) {
						if (j == terms.size() - 1)
							ans.push_back(FirstEntry("", i));
						continue;
					}
				}
				//	for (size_t k = 0; k < child_firsts.size(); k++)
				//		ans.push_back(child_firsts[i]);
				break;
			}
		}
	}
	//TODO:  check for empty
	this->first[p] = ans;
	return ans;
}

vector<FirstEntry> ParserGenerator::find_follow_after(int s) {
	string name = productions[s].LHS;
	vector<FirstEntry> sfollow;
	for (size_t p = 0; p < productions.size(); p++) {
		vector<vector<Term> > ors = productions[p].RHS;
		for (size_t o = 0; o < ors.size(); o++) {
			vector<Term> terms = ors[o];

			// check for last case
			int last = (int) terms.size() - 1;
			bool add_depend = false;
			while (last >= 0) {
				if (terms[last].name == name) {
					add_depend = true;
					break;
				}
				if (!terms[last].isTerminal) {
					int first_ind = LHS_index[terms[last].name];
					if (contains(first[first_ind], ""))
						last--;
					else
						break;
				} else
					break;
			}
			if (add_depend) {
				vector<FirstEntry> dependsFollow = follow[p];
				for (size_t df = 0; df < dependsFollow.size(); df++)
					if (dependsFollow[df].terminal != "" && !contains(sfollow,
							dependsFollow[df].terminal))
						sfollow.push_back(dependsFollow[df]);
			}

		}// for all other productions

	}
	return sfollow;
}

vector<FirstEntry> ParserGenerator::find_follow_before(int s) {
	string name = productions[s].LHS;
	vector<FirstEntry> sfollow;
	for (size_t p = 0; p < productions.size(); p++) {
		vector<vector<Term> > ors = productions[p].RHS;
		for (size_t o = 0; o < ors.size(); o++) {
			vector<Term> terms = ors[o];

			for (size_t t = 0; t < terms.size() - 1; t++) {
				Term term = terms[t];
				if (term.name == name)
				{
					size_t nxTerm = t+1;
					Term next = terms[nxTerm];
					bool hasEpsilon;
					do {
						hasEpsilon = false;
						next = terms[nxTerm];
						if (next.isTerminal) {
							if (!contains(sfollow, next.name))
								sfollow.push_back(FirstEntry(next.name, s));
						} else {
							vector<FirstEntry> nextFirst =
									first[LHS_index[next.name]];

							for (size_t nf = 0; nf < nextFirst.size(); nf++)
							{
								FirstEntry fe = nextFirst[nf];
								if (fe.terminal != "")
								{
									if (!contains(sfollow, fe.terminal))
										sfollow.push_back(fe);
								}else
									hasEpsilon = true;
							}
						}
						nxTerm++;
					} while (nxTerm < terms.size() && hasEpsilon);// check for  epsilon
				}
			}

		}// for all other productions

	}
	return sfollow;
}

void ParserGenerator::initFollow() {

	//topological sort begin

	// set dependencies

	vector<int> INDEG(productions.size());
	//INDEG.at(productions.size()); //TODO : resize
	map<pair<int, int> , int> g;
	for (size_t p = 0; p < productions.size(); p++) {
		vector<vector<Term> > ors = productions[p].RHS;
		for (size_t o; o < ors.size(); o++) {
			Term back = ors[o].back();
			if (!back.isTerminal && back.name != productions[p].LHS) {
				INDEG[this->LHS_index[back.name]]++;
				g[make_pair(p, this->LHS_index[back.name])] = 1;
			}
		}
	}

	queue<int> Q;

	for (size_t n = 0; n < productions.size(); n++) {
		if (INDEG[n] == 0)
			Q.push(n);
	}

	vector<int> topoList;

	while (!Q.empty()) {
		int N = Q.front();
		Q.pop();
		topoList.push_back(N);
		for (size_t M = 0; M < productions.size(); M++) {
			if (g[make_pair(N, M)] == 1) {
				INDEG[M]--;
				if (INDEG[M] == 0) {
					Q.push(M);
				}
			}
		}
	}
	// top sort end

	//	for (size_t s = 0; s < productions.size(); s++) {
	//		printf(" %s ", productions[topoList[s]].LHS.c_str());
	//	}
	//	puts("");

	for (size_t s = 0; s < productions.size(); s++) {
		this->follow[topoList[s]] = find_follow_before(topoList[s]);
	}

	this->follow[0].push_back(FirstEntry("$", 0));
	for (size_t s = 0; s < productions.size(); s++) {
		vector<FirstEntry> v = find_follow_after(topoList[s]);
		for (size_t z = 0; z < v.size(); z++)
			this->follow[topoList[s]].push_back(v[z]);
	}

}

ParserGenerator::~ParserGenerator() {
}

