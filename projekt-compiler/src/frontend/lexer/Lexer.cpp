/*
 * Lexer.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Miro B.
 */

#include <frontend/lexer/Lexer.h>

Lexer::Lexer() {

}

Lexer::~Lexer() {

}




void Lexer::lex(std::string srcFile) {
	// open source file
	std::ifstream is;
	is.open(srcFile);
	if (!is.good()) {
		IO_Exception ie;
		ie.set_file(srcFile);
		throw ie;
	}

	std::string line;
	std::string functionName = "";


	RailFunction* act = NULL;
	while(!is.eof()) {

		std::getline(is, line);


		if(line.at(0) == '$') {
			// find function name
			size_t nameStart = line.find("'", 0);
			size_t nameEnd = line.find("'", nameStart+1, 1);

			// name exists && not empty
			if(nameStart != std::string::npos && nameEnd != std::string::npos && nameStart+1 != nameEnd) {
				functionName = line.substr(nameStart+1, nameEnd-nameStart-1);
				act = new RailFunction(functionName);
				functions.push_back(std::shared_ptr<RailFunction>(act));
				//lines = std::vector<std::string>;
			}
			else {
				/*
				 * FIXME error handling: found a $ but no function name
				 * Options
				 * 		1. do nothing -> adds this line to the actual RailFunction
				 * 		2. act = NULL; -> assume this is a new function (misspelled -> ignore whole function)
				 */
				act = NULL;
			}
		}

		// Add line if a RailFunction is active
		if(act != NULL) {
			// Add line to RailFunction
			std::vector<uint32_t> data;
			std::deque<uint32_t> utf8line;
			Encoding::utf8StringToUnicode(line, &utf8line, 0);
			for(auto it=utf8line.begin(); it<utf8line.end(); ++it) {
				data.push_back(*it);
			}
			act->data.push_back(data);

			if(act->width < utf8line.size()) {
				act->width = utf8line.size();
			}
			act->height++;
		}
	}


	std::cout << "done" << std::endl;

}


