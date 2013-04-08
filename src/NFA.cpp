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
	currID=-1;
	isDef = false;

}



NFA::NFA(vector<StringPair> expr) {
	expressions = expr;
	state_id = 0;
	currID=-1;
	isDef = false;

}
/*create all definitions*/
void NFA::createNFADefs() {
	for (int var = 0; var < (int)defs.size(); ++var) {
		create_NFA(defs[var].definition);
		defsNFA.insert(make_pair(defs[var].id, NFATable));
	}

}
/*
 * createAll : create all definitions and regular expressions
 * */
void NFA::createAll() {
	isDef=true;
	createNFADefs();
	//create NFA
	isDef=false;
	for (int var = 0; var < (int)expressions.size(); ++var) {
		string matched=expressions[var].id;
		for (int i = 0; i < (int)matchedExps.size(); ++i) {
			string currExp = matchedExps[i];

			if (matchedExps[i] == expressions[var].id) {
				currID = i;
				break;
			}
		}
		create_NFA(expressions[var].definition);

		NFAs.push_back(NFATable);
	}
	for (int var = 0; var < (int)NFAs.size(); ++var) {
		OperandStack.push(NFAs[var]);
	}
	while (OperandStack.size() > 1) {
		Union();
	}
	pop(NFATable);
	cout << "------------------------------" << endl;
	cout << "Front State " << NFATable.front()->id << " End State "
			<< NFATable.back()->id << endl;

	for (int var = 0; var < (int)NFATable.size(); ++var) {
		NFATable[var]->toString();
	}
}
/*create_NFA:
 * Creates Nondeterministic Finite Automata from a Regular Expression
 * */
void NFA::create_NFA(string def) {
	string temp = "";
	for (int i = 0; i < (int)def.size(); ++i) {
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
					if (temp.size() != 1) {
						if (temp[1] == '-') {
							push_NFA(temp);
						} else {
							//error
						}
					} else {
						push_NFA(temp[0]);
						//push to input set
						input.insert(temp[0]);
					}

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
						bool evaluatedone=evaluate();
						if(!evaluatedone){
							cout<<"Operation error on this definition "<<def<<endl;
						}

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
			if (temp.size() != 1) {
				if (temp[1] == '-') {
					push_NFA(temp);
				} else {
					//error
				}
			} else {
				push_NFA(temp[0]);
				//push to input set
				input.insert(temp[0]);
			}
		}
	}
	while (!OperatorStack.empty()) {
		evaluate();

	}
	pop(NFATable);
	if(!isDef){
		cout<<"Accepted with id "<<currID<<endl;
		NFATable[NFATable.size()-1]->matched_pattern =currID;
		NFATable[NFATable.size()-1]->acceptingState=true;
	}
}
/*push_NFA:
 * make NFA for this string then push into operand stack*/
void NFA::push_NFA(char s) {
//change to
	FA_State *state0 = new FA_State(state_id++);
	FA_State *state1 = new FA_State(state_id++);

	state0->AddTransition(s, state1);
	state0->matched_pattern  = currID;
	state1->matched_pattern  = currID;
	FSA_TABLE NFATable;
	NFATable.push_back(state0);
	NFATable.push_back(state1);
	OperandStack.push(NFATable);

}
void NFA::push_NFA(string s) {
	FA_State *startState = new FA_State(state_id++);
	FA_State *endState = new FA_State(state_id++);
	startState->matched_pattern  = currID;
	endState->matched_pattern  = currID;
	FSA_TABLE NFATable;
	NFATable.push_back(startState);
	for (int i = s[0]; i <= s[2]; i++) {
		FA_State* temp = new FA_State(state_id++);
		temp->matched_pattern  = currID;
		startState->AddTransition(i, temp);
		temp->AddTransition(char(8), endState);
		input.insert(i);
		NFATable.push_back(temp);
	}
	if(!isDef){


//	endState->acceptingState = true;
	}
	NFATable.push_back(endState);
	OperandStack.push(NFATable);
//	for (int i = 0; i < strRegEx.size(); ++i) {
//		// get the character
//		char c = strRegEx[i];
//
//		if (IsInput(c))
//			push_NFA(c);
//		else if (OperatorStack.empty())
//			OperatorStack.push(c);
//		else if (IsLeftParanthesis(c))
//			OperatorStack.push(c);
//		else if (IsRightParanthesis(c)) {
//			// Evaluate everything in parenthesis
//			while (!IsLeftParanthesis(OperatorStack.top()))
//				if (!evaluate())
//					return;
//			// Remove left parenthesis after the evaluation
//			OperatorStack.pop();
//		} else {
//			while (!OperatorStack.empty() && Presedence(c, OperatorStack.top()))
//				if (!evaluate())
//					return;
//			OperatorStack.push(c);
//		}
//	}
//
//	// Evaluate the rest of operators
//	while (!OperatorStack.empty())
//		if (!evaluate())
//			return;
//
//	// Pop the result from the stack
//	if (!pop(NFATable))
//		return;
//	cout << "------------------------------" << endl;
//		cout << "Front State " << NFATable.front()->id << " End State "
//				<< NFATable.back()->id << endl;
//		NFATable[NFATable.size() - 1]->acceptingState = true;
//		for (int var = 0; var < NFATable.size(); ++var) {
//			NFATable[var]->toString();
//		}
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
	stateStart->matched_pattern  = currID;
	endState->matched_pattern  = currID;

//make epsilon transition from start state to end state
	stateStart->AddTransition(char(8), endState);

//add epsilon transition from start state to the first state of the operand
	stateStart->AddTransition(char(8), table1[0]);

//add epsilon transition from end state of the operand to the end state
	table1[table1.size() - 1]->AddTransition(char(8), endState);

//add epsilon transition from end state of the operand to start state of the operand
	table1[table1.size() - 1]->AddTransition(char(8), table1[0]);

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
	A[A.size() - 1]->AddTransition(char(8), B[0]);
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
	for (int var = 0; var < (int)temp.size(); ++var) {
		temp[var]->id = state_id++;
	}
	OperandStack.push(temp);
	/*make operand star*/
	bool stardone=Star();
	/*concat with star*/
	bool concatdone=concat();
	if(concatdone&&stardone){
	return true;
	}
	return false;
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
	startState->matched_pattern  = currID;
	endState->matched_pattern  = currID;
	/*add transition from start state to first state of one of the operands*/
	startState->AddTransition(char(8), A[0]);

	/*add transition from start state to the other operand */
	startState->AddTransition(char(8), B[0]);

	/*add transition from the last state of both operands to the end state*/
	A[A.size() - 1]->AddTransition(char(8), endState);
	B[B.size() - 1]->AddTransition(char(8), endState);

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

