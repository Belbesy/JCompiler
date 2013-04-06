/*
 * NFA.cpp
 *
 *  Created on: Mar 21, 2013
 *      Author: Amira
 */

#include "NFA.h"
#include <iostream>
#include <string>
NFA::NFA() {
	state_id = 0;
}
NFA::NFA(StringPair expression) {
	regex = expression;
	state_id = 0;
}
NFA::NFA(vector<StringPair> expr) {
	expressions = expr;
	state_id = 0;

}
/*create all definitions*/
void NFA::createNFADefs() {
	for (int var = 0; var < defs.size(); ++var) {
		create_NFA(defs[var].definition);
		defsNFA.insert(make_pair(defs[var].id, NFATable));
	}

}
/*
 * createAll : create all definitions and regular expressions
 * */
void NFA::createAll() {
	createNFADefs();
	//create NFA
	for (int var = 0; var < expressions.size(); ++var) {
		create_NFA(expressions[var].definition);
		NFAs.push_back(NFATable);
	}
	for (int var = 0; var < NFAs.size(); ++var) {
		OperandStack.push(NFAs[var]);
	}
	while (OperandStack.size() > 1) {
		Union();
	}
	pop(NFATable);
	cout << "------------------------------" << endl;
	cout << "Front State " << NFATable.front()->id << " End State "
			<< NFATable.back()->id << endl;
	NFATable[NFATable.size() - 1]->acceptingState = true;
	for (int var = 0; var < NFATable.size(); ++var) {
		NFATable[var]->toString();
	}
}
/*create_NFA:
 * Creates Nondeterministic Finite Automata from a Regular Expression
 * */
void NFA::create_NFA(string def) {
	string temp = "";
	for (int i = 0; i < def.size(); ++i) {
		bool isNormalOperator = true;
		char curr = def[i];
		/* if char is input*/
		if (IsInput(curr)) {
			temp += curr;
		} /*if char is operator*/
		else {
			if (temp != "") {
				/*if reserved symbol then an operator*/
				if (temp != "\\L") {

					if (temp.size() == 1 && temp[0] == '\\') {
						isNormalOperator = false;
						temp = "";
						temp += curr;
					} else if (temp[0] == '\\') {
						/*remove \\ from string*/
						temp = temp.substr(1, temp.size() - 1);

					}
				}
				/*get NFA for this definition from map*/
				FSA_TABLE defin = defsNFA[temp];
				if (defin.size() != 0) {
					/*insert this NFA into operand stack*/
					OperandStack.push(defin);
					temp = "";
				} else {
					/*make NFA for it then push into operand stack*/
					push_NFA(temp);
					temp = "";
				}
			}
			/*if not reserved symbol */
			if (isNormalOperator) {
				/*insert into operand stack if empty*/
				if (OperatorStack.empty()) {
					OperatorStack.push(curr);
				} else if (IsLeftParanthesis(curr)) {
					OperatorStack.push(curr);
				} else if (IsRightParanthesis(curr)) {
					while (!IsLeftParanthesis(OperatorStack.top())) {
						evaluate();

					}
					OperatorStack.pop();
				} else {
					//Normal operator
					while (!OperatorStack.empty()
							&& Presedence(curr, OperatorStack.top())) {
						evaluate();

					}
					OperatorStack.push(curr);
				}
			}

		}

	}
	if (temp != "") {
		FSA_TABLE defin = defsNFA[temp];
		if (defin.size() != 0) {
			OperandStack.push(defin);
			temp = "";
		} else {
			push_NFA(temp);
			temp = "";
		}
	}
	while (!OperatorStack.empty()) {
		evaluate();

	}
	pop(NFATable);
}
/*push_NFA:
 * make NFA for this string then push into operand stack*/
void NFA::push_NFA(string s) {
	//change to
	FA_State *state0 = new FA_State(state_id++);
	FA_State *state1 = new FA_State(state_id++);
	state0->AddTransition(s, state1);
	FSA_TABLE NFATable;
	NFATable.push_back(state0);
	NFATable.push_back(state1);
	OperandStack.push(NFATable);

}
/*evaluate :
 * check what is the operator then call proper method
 * */
bool NFA::evaluate() {
	if (OperatorStack.size() > 0) {
		char top = OperatorStack.top();
		OperatorStack.pop();
		if (top == 42) {
			return Star();
		} else if (top == 124) {
			return Union();
		} else if (top == 32) {
			return concat();
		} else if (top == 43) {
			return plus();
		}
	}
	return false;
}
/*Precedence : Returns operator precedence.
 * Returns true if precedence of opLeft <= opRight.
 * Klein's Closure & Plus	- highest
 * Concatenation			- middle
 * Union					- lowest
 */
bool NFA::Presedence(char opLeft, char opRight) {
	if (opLeft == opRight) {
		return true;
	}
	if (opLeft == '*')
		return false;
	if (opRight == '*')
		return true;
	if (opLeft == '+')
		return false;
	if (opRight == '+')
		return true;
	if (opLeft == ' ')
		return false;
	if (opRight == ' ')
		return true;
	if (opLeft == '|')
		return false;
	return true;
}
/*Pop :Pops an element from the operand stack
 * The return value is true if an element was
 * Popped successfully, otherwise it is
 * false (syntax error)
 */
bool NFA::pop(FSA_TABLE& NFATable) {
	if (!OperandStack.empty()) {
		NFATable = OperandStack.top();
		OperandStack.pop();
		return true;
	}
	return false;
}
/*Star :
 *  Evaluates the Kleen's closure - star operator
 *  Pops one operator from the stack and evaluates
 *  the star operator on it. It pushes the result
 *  on the operand stack again.
 */
bool NFA::Star() {
	cout << "Star " << endl;
	FSA_TABLE table1;
	/*Pop one Operand from operand stack*/
	if (!pop(table1)) {
		return false;
	}
	/*initialize start and end state*/
	FA_State* stateStart = new FA_State(state_id++);
	FA_State* endState = new FA_State(state_id++);

	//make epsilon transition from start state to end state
	stateStart->AddTransition("\\L", endState);

	//add epsilon transition from start state to the first state of the operand
	stateStart->AddTransition("\\L", table1[0]);

	//add epsilon transition from end state of the operand to the end state
	table1[table1.size() - 1]->AddTransition("\\L", endState);

	//add epsilon transition from end state of the operand to start state of the operand
	table1[table1.size() - 1]->AddTransition("\\L", table1[0]);

	/*add start and end state*/
	table1.push_back(endState);
	table1.push_front(stateStart);

	/*push operand to stack*/
	OperandStack.push(table1);
	return true;
}
/*concat : Evaluates the concatenation operator
 * This function pops two operands from the stack
 * and evaluates the concatenation on them, pushing
 * the result back on the stack.
 */
bool NFA::concat() {
	cout << "Concat" << endl;
	FSA_TABLE A, B;
	/*pop last two operands*/
	if (!pop(B) || !pop(A)) {
		return false;
	}
	/*add transition from A to B*/
	A[A.size() - 1]->AddTransition("\\L", B[0]);
	A.insert(A.end(), B.begin(), B.end());
	/*push back NFA to the operand stack*/
	OperandStack.push(A);

	return true;
}
/*Star :
 *  Evaluates plus operator
 *  Pops one operator from the stack and evaluates
 *  the plus operator on it. It pushes the result
 *  on the operand stack again.
 */
bool NFA::plus() {
	cout << "Plus" << endl;
	/*get last operand*/
	FSA_TABLE temp = OperandStack.top();
	for (int var = 0; var < temp.size(); ++var) {
		temp[var]->id = state_id++;
	}
	OperandStack.push(temp);
	/*make operand star*/
	Star();
	/*concat with star*/
	concat();
	return true;
}
/*Union:
 *  Evaluates the union operator '|'
 *  Pops 2 operands from the stack and evaluates
 *  the union operator pushing the result on the
 *  operand stack.
 */
bool NFA::Union() {
	cout << "Union" << endl;
	/*Pop two operands from operand Stack*/
	FSA_TABLE A, B;
	if (!pop(B) || !pop(A)) {
		return false;
	}
	/*initialize start and end state*/
	FA_State *startState = new FA_State(state_id++);
	FA_State *endState = new FA_State(state_id++);

	/*add transition from start state to first state of one of the operands*/
	startState->AddTransition("\\L", A[0]);

	/*add transition from start state to the other operand */
	startState->AddTransition("\\L", B[0]);

	/*add transition from the last state of both operands to the end state*/
	A[A.size() - 1]->AddTransition("\\L", endState);
	B[B.size() - 1]->AddTransition("\\L", endState);

	/*push end state in the operand B*/
	B.push_back(endState);

	/*push start state to operand A*/
	A.push_front(startState);

	/*make two NFA only one NFA*/
	A.insert(A.end(), B.begin(), B.end());

	/*Push NFA to the operand Stack*/
	OperandStack.push(A);

	return true;
}

/*---------------------------------------------------------------------------------*/
bool NFA::isReservedSymbol(char c) {
	if (c == '=' || c == '(' || c == ')' || c == '*' || c == '+')
		return true;
	return false;
}

bool NFA::IsInput(char c) {

	return (!IsOperator(c));
}
bool NFA::IsOperator(char ch) {
	return ((ch == 42)/*star*/|| (ch == 124)/*|*/|| (ch == 43)/*+*/|| (ch == 40)/*(*/
	|| (ch == 41)/*)*/|| (ch == 32) /*space means concatenation*/);
}
bool NFA::IsLeftParanthesis(char ch) {
	return (ch == 40);
}
bool NFA::IsRightParanthesis(char ch) {
	return (ch == 41);
}
NFA::~NFA() {
	// TODO Auto-generated destructor stub
}

