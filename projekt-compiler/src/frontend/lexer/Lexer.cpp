/*
 * Lexer.cpp
 *
 *  Created on: May 28, 2014
 *      Author: Miro B.
 */

#include <frontend/lexer/Lexer.h>
#include <set>

Lexer::Lexer() {

}

Lexer::~Lexer() {

}




void Lexer::lex(const std::string srcFile) {
	std::set<std::string> readFunctionNames;
	// open source file
	std::ifstream is;
	is.open(srcFile);
	if (!is.good()) {
		throw EnvException(FRONTEND_LEXER, "Cannot open " + srcFile + " for reading");
	}

	std::string line;
	std::string functionName = "";


	// Skip BOM if present
	if(is.get() != 0xEF || is.get() != 0xBB || is.get() != 0xBF) {
		is.seekg(0);
	}



	RailFunction* act = NULL;
	int32_t lineId = 0;
	while(!is.eof()) {

		// get next line
		std::getline(is, line);
		lineId++;

		if(line.length() > 0 && line.at(0) == '$') {
			// find function name
			size_t nameStart = line.find("'", 0);
			size_t nameEnd = line.find("'", nameStart+1, 1);

			// name exists && not empty
			if(nameStart != std::string::npos && nameEnd != std::string::npos && nameStart+1 != nameEnd) {
				functionName = line.substr(nameStart+1, nameEnd-nameStart-1);
				if(readFunctionNames.find(functionName) != readFunctionNames.end()){
					//functionname is new
					Env::addWarning(FRONTEND_LEXER, "Function in line " + line + " was already defined previously", lineId, 0);
				} else{
					readFunctionNames.insert(functionName);
				}
				act = new RailFunction(functionName, lineId);
				functions.push_back(std::shared_ptr<RailFunction>(act));
			}
			else {
				/*
				 * Error handling: found $ but no function name
				 * Options
				 * 		1. do nothing -> adds this line to the actual RailFunction
				 * 		2. act = NULL; -> assume this is a new (misspelled) function -> ignore whole function
				 */
				act = nullptr;
				Env::addWarning(FRONTEND_LEXER, "Found $ but no proper function name given (skipping...): " + line, lineId, 0);
			}
		}

		// Add line if a RailFunction is active
		if(act != NULL) {
			// skip empty line if last line was empty too
			if(line.length() != 0 || act->data.back().size() != 0) {
				// Add line to RailFunction
				std::vector<uint32_t> data;
				std::deque<uint32_t> utf8line;
				Encoding::utf8StringToUnicode(line, &utf8line, 0);
				for(auto it=utf8line.begin(); it<utf8line.end(); ++it) {
					data.push_back(*it);
				}
				act->data.push_back(data);

				if(act->width < (int32_t)utf8line.size()) {
					act->width = (int32_t)utf8line.size();
				}
				act->height++;
			}
		}
	}


	if(Env::verbose()) {
		for(auto it=functions.begin(); it<functions.end(); ++it) {
			(*it)->dump();
		}
	}

	if(Env::verbose()) {
		std::cout << "done" << std::endl;
	}
}


