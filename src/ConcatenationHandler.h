/*
 * ConcatenationHandler.h
 *
 *  Created on: Apr 7, 2013
 *      Author: raed
 */


#ifndef CONCATENATIONHANDLER_H_
#define CONCATENATIONHANDLER_H_
#include <vector>
#include <string>


using namespace std;


class ConcatenationHandler {
public:
	ConcatenationHandler(vector<string> patterns , string separator_);
	string handle(string lexeme);
	virtual ~ConcatenationHandler();
private:
	vector<int*> F;
	vector<char*> p;
	vector<int> length;
	int* matched_len , patternsNum;
	string separator;
	int getL(char a, int l,int patternID);
	void build(string pattern);
	void reset_all();
	bool isOperator(const char ch);
};

#endif /* CONCATENATIONHANDLER_H_ */
