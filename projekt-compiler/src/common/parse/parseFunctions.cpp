#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
using namespace std;

const int MAX_CHARS_PER_LINE = 1024;
const int MAX_LINES_PER_FUNCTION = 1024;

class RailFunction {
	string name;
	char code[MAX_CHARS_PER_LINE][MAX_LINES_PER_FUNCTION];

	public:
		string getName();
		void setName(string name);
		void initData();
		char getData(int offsetX, int offsetY);
		void setData(int offsetX, int offsetY, char c);
		void printRails();
};

string RailFunction::getName() {
	return this->name;
}

void RailFunction::setName(string name) {
	this->name = name;
}

void RailFunction::initData() {
	for(int y=0; y<MAX_LINES_PER_FUNCTION; y++) {
		for(int x=0; x<MAX_CHARS_PER_LINE; x++) {
			this->setData(x,y,0);
		}
	}
}

char RailFunction::getData(int offsetX, int offsetY) {
	return this->code[offsetX][offsetY];
}

void RailFunction::setData(int offsetX, int offsetY, char c) {
	code[offsetX][offsetY] = c;
}

void RailFunction::printRails() {
	bool needNewLine;

	for(int y=0; y<MAX_LINES_PER_FUNCTION; y++) {
		needNewLine = false;

		for(int x=0; x<MAX_CHARS_PER_LINE; x++) {
			char currentData = getData(x,y);
			if(currentData != 0) {
				cout << currentData;
				needNewLine = true;
			}
		}
		if(needNewLine) {
			cout << "\n";
		}
	}

	cout << "\n===========================================\n";
}

//the vector which holds all rail functions
vector<RailFunction> allRailFunctions;


int main(int argc, char* argv[]) {
	if(argc < 2) {
		cout << "Please pass the rail file path as argument!";
		return -1;
	} else if(argc > 2) {
		cout << "Only one argument (rail file path) is supported!";
		return -1;
	}

	// create a file-reading object
	ifstream fin;
	fin.open(argv[1]); // open a file
	if (!fin.good()) {
		cout << "file not found: " << argv[1];
		return -1; // exit if file not found
	}

	// work with states
	string state = "find function";

	//a rail function object
	RailFunction r;

	//initialize data
	r.initData();

	//current function name
	string currentFunctionName = "";

	//parse offsets x and y
	int offsetX = 0;
	int offsetY = 0;

	// read each line of the file
	while (!fin.eof()) {
		char nextChar = fin.get();

		if(state == "find function") {
			if(nextChar == '$') {
				r.setData(offsetX, offsetY, nextChar);
				offsetX++;
				state = "find first semicolon";
			} else {
				state = "ignore line while find function";
			}
			continue;
		} else if(state == "ignore line while find function") {
			if(nextChar == '\n') {
				state = "find function";
			}
			continue;
		} else if(state == "find first semicolon") {
			if(nextChar == '\n') {
				offsetY++;
			} else {
				r.setData(offsetX, offsetY, nextChar);
				offsetX++;
			}

			if(nextChar == '\'') {
				state = "parse function name";
			}
			continue;
		} else if(state == "parse function name") {
			if(nextChar == '\n') {
				offsetY++;
			} else {
				r.setData(offsetX, offsetY, nextChar);
				offsetX++;
			}

			if(nextChar == '\'') {
				r.setName(currentFunctionName);
				currentFunctionName = "";
				state = "ignore line after function found";
			} else {
				currentFunctionName += nextChar;
			}
			continue;
		} else if(state == "ignore line after function found") {
			if(nextChar == '\n') {
				offsetY++;
			} else {
				r.setData(offsetX, offsetY, nextChar);
				offsetX++;
			}

			if(nextChar == '\n') {
				state = "parse rail";
			}
			continue;
		} else if(state == "parse rail") {
			if(nextChar == '$') {
				allRailFunctions.push_back(r);
				r.initData();
				offsetX = 0;
				offsetY = 0;
				r.setData(offsetX, offsetY, nextChar);
				offsetX++;
				state = "find first semicolon";
			} else if(nextChar == '\n') {
				offsetY++;
			} else if(nextChar == -1) {
				break;
			} else {
				r.setData(offsetX, offsetY, nextChar);
				offsetX++;
			}
			continue;
		} else {
			cout << "State \'" << state << "\' is not yet implemented.";
			break;
		}
	}

	//add the last function also to the vector
	allRailFunctions.push_back(r);

	return allRailFunctions;

	//debug: print all functions and rails
	/*for(unsigned i=0; i<allRailFunctions.size(); i++) {
		cout << "function " << allRailFunctions[i].getName() << ":\n\n";
		allRailFunctions[i].printRails();
	}*/
}
