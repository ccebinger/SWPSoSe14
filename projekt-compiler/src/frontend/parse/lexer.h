/*
 * parseFunctions.h
 *
 *  Created on: 12.05.2014
 *      Author: miro
 */

#ifndef PARSEFUNCTIONS_H_
#define PARSEFUNCTIONS_H_

#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>


using namespace std;

const int MAX_CHARS_PER_LINE = 1024;
const int MAX_LINES_PER_FUNCTION = 1024;

class RailFunction {
	string name;


	public:
		char code[MAX_CHARS_PER_LINE][MAX_LINES_PER_FUNCTION];
		string getName();
		void setName(string name);
		void initData();
		char getData(int offsetX, int offsetY);
		void setData(int offsetX, int offsetY, char c);
		void printRails();
		void lex(const string& filename);
};

class Lexer {
	public:
		vector<RailFunction> functions;
		void lex(const string& filename);
};


#endif /* PARSEFUNCTIONS_H_ */
