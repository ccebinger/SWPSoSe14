#include <frontend/parse/lexer.h>



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



void Lexer::lex(const string& filename) {

	// create a file-reading object
	ifstream fin;
	fin.open(filename); // open a file
	if (!fin.good()) {
		IO_Exception ie;
		ie.set_file(filename);
		throw ie;
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
				offsetY++;
				state = "find first semicolon";
			}
			else if(nextChar == '\n') {
				// Fixes a bug where $ was skipped
				offsetY = 0;
			}
			else {
				state = "ignore line while find function";
			}
			continue;
		} else if(state == "ignore line while find function") {
			if(nextChar == '\n') {
				state = "find function";
				offsetY = 0;
			}
			continue;
		} else if(state == "find first semicolon") {
			if(nextChar == '\n') {
				offsetX++;
				offsetY = 0;
			} else {
				r.setData(offsetX, offsetY, nextChar);
				offsetY++;
			}

			if(nextChar == '\'') {
				state = "parse function name";
			}
			continue;
		} else if(state == "parse function name") {
			if(nextChar == '\n') {
				offsetX++;
				offsetY = 0;
			} else {
				r.setData(offsetX, offsetY, nextChar);
				offsetY++;
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
				offsetX++;
				offsetY = 0;
			} else {
				r.setData(offsetX, offsetY, nextChar);
				offsetY++;
			}

			if(nextChar == '\n') {
				state = "parse rail";
				offsetY = 0;
			}
			continue;
		} else if(state == "parse rail") {
			if(nextChar == '$') {
				functions.push_back(r);
				r.initData();
				offsetX = 0;
				offsetY = 0;
				r.setData(offsetX, offsetY, nextChar);
				offsetY++;
				state = "find first semicolon";
			} else if(nextChar == '\n') {
				offsetX++;
				offsetY = 0;
			} else if(nextChar == -1) {
				break;
			} else {
				r.setData(offsetX, offsetY, nextChar);
				offsetY++;
			}
			continue;
		} else {
			cout << "State \'" << state << "\' is not yet implemented.";
			break;
		}
	}

	//add the last function also to the vector
	functions.push_back(r);


	//debug: print all functions and rails
	/*for(unsigned i=0; i<functions.size(); i++) {
		cout << "function " << functions[i].getName() << ":\n\n";
		functions[i].printRails();
	}*/

}

