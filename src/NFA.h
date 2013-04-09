/*
 * NFA.h
 *
 *  Created on: Mar 21, 2013
 *      Author: Amira
 */
#include "FA_State.h"
#ifndef NFA_H_
#define NFA_H_
#include "StringPair.h"
#include <map>
#include <string>

class NFA {
private:
	bool isDef;
public:
	std::set<char> input;
	vector<StringPair> defs;
	map<string, string> defsNFA;
	/*regular expressions*/
	vector<StringPair> expressions;
	/*NFAs for regular expressions*/
	vector<FSA_TABLE> NFAs;
	/**vector for all matched expression**/
	vector<string> matchedExps;

	int currID;
	NFA();
	NFA(StringPair reg_expression);
	NFA(vector<StringPair> expressions);
	virtual ~NFA();

	//table contains all states of automata
	FSA_TABLE NFATable;
	//stack for operands each operand considered an NFA
	stack<FSA_TABLE> OperandStack;
	//stack for operators
	stack<char> OperatorStack;
	//state id
	int state_id;

	/*METHODS*/

	void create_NFA(string expr, stack<char> OperatorStack);
	bool concat();
	bool Star();
	bool Union();
	bool plus();
	string ConcatExpand(string strRegEx);
	bool IsOperator(char c);
	bool Presedence(char op1, char op2);
	bool IsInput(char c);
	bool IsLeftParanthesis(char c);
	bool IsRightParanthesis(char c);
	void push_NFA(char s);
	void push_NFA(string s);
	bool evaluate(stack<char> OperatorStack);
	bool pop(FSA_TABLE& NFATable);
	void createAll();
	void createNFADefs();
	bool isReservedSymbol(char c);
	string getExpression(string s);
};

#endif /* NFA_H_ */
