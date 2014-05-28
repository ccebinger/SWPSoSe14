/*
 * Lexer.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Miro B.
 */

#include <frontend/parse/Lexer.h>

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
				 * FIXME error handling: found a $ but no function name given
				 * Options
				 * 		1. do nothing -> adds this line to the existing RailFunction
				 * 		2. continue; -> skip the whole line
				 * 		3. act = NULL; -> assume this is a new function (but misspelled -> ignore whole function)
				 */
				//continue;
				act = NULL;
			}
		}

		// Add line if a RailFunction is active
		if(act != NULL) {
			// Add line to RailFunction
			//FIXME switch between ASCII and UTF-8
			std::vector<uint32_t> data;
			for(auto it=line.begin(); it<line.end(); ++it) {
				data.push_back(*it);
			}
			act->data.push_back(data);


			if(act->width < line.length()) {
				act->width = line.length();
			}
			act->height++;
		}
	}


	std::cout << "done" << std::endl;

}


