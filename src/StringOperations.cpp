/*
 * StringOperations.cpp
 *
 *  Created on: Mar 22, 2013
 *      Author: Saad Dawy
 */

#include "StringOperations.h"
#include <strstream>
#include <string>
StringOperations::StringOperations() {
	// TODO Auto-generated constructor stub

}



StringOperations::~StringOperations() {
	// TODO Auto-generated destructor stub
}
vector<string>StringOperations::split(string str){

                    string buf; // Have a buffer string
                    stringstream ss(str); // Insert the string into a stream

                    vector<string> tokens; // Create vector to hold our words

                    while (ss >> buf)
                        tokens.push_back(buf);
                    return tokens;
}

