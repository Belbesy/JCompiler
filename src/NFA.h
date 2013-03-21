/*
 * NFA.h
 *
 *  Created on: Mar 21, 2013
 *      Author: Amira
 */
#include "FA_State.h"
#ifndef NFA_H_
#define NFA_H_

class NFA {
public:
	NFA();
	NFA(string reg_expression);
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

	void create_NFA(string reg_exp);
	bool concat();
	bool Star();
	bool Union();
	string ConcatExpand(string strRegEx);
	bool IsOperator(char c);
	bool Presedence(char op1, char op2);
	bool IsInput(char c);
	bool IsLeftParanthesis(char c) ;

};

#endif /* NFA_H_ */
