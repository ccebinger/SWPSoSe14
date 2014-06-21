/*
 * Lexer.h
 *
 *  Created on: May 28, 2014
 *      Author: Miro B.
 */

#ifndef LEXER_H_
#define LEXER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <memory>


#include <common/Env.h>
#include <common/Encoding.h>


/**
 * Representing one Rail function
 * Holds it's name, the width and height of the Function and all it's characters as Unicode CodePoints
 *
 * @author Miro B.
 */
class RailFunction {

	friend class Lexer;

private:
	std::string name;
	int32_t width = 0;
	int32_t height = 0;

	std::vector<std::vector<uint32_t>> data;

public:
	/*
	 * Source file line position
	 */
	const int32_t funcStartLine;

	/**
	 * Creates a Rail function
	 * @param name		Name of the Rail function
	 */
	RailFunction(std::string _name, int32_t _funcStartLine) : name(_name), funcStartLine(_funcStartLine) {

	}

	virtual ~RailFunction() {}


	/**
	 * Getter for the Rail function name
	 * @return	Function name
	 */
	inline std::string getName() const {
		return this->name;
	}


	/**
	 * Getter for the Unicode Codepoint 2d-Array representing the Rail Function
	 * @return	Unicode 2D-Array
	 */
	inline std::vector<std::vector<uint32_t>> getData() {
		return this->data;
	}

	/**
	 * Getter for the width of the Rail Function
	 * @return	width
	 */
	inline int32_t getWidth() {
		return this->width;
	}

	/**
	 * Getter for the height of the Rail Function
	 *
	 * @return	height
	 */
	inline int32_t getHeight() {
		return this->height;
	}

	/**
	 * Getter for one Unicode Codepoint
	 *
	 * @param line	Codepoint row
	 * @param pos	Codepoint column
	 * @return		Codepoint
	 */
	inline uint32_t get(uint32_t line, uint32_t pos) const {
		if(data.size() > line && data[line].size() > pos) {
			return data[line][pos];
		}
		return 0;
	}


	/**
	 * Prints the Rail function to std::cout
	 */
	void dump() {
		if(Env::verbose()) {
			for(auto it=data.begin(); it<data.end(); ++it) {
				std::vector<uint32_t> line = *it;
				for(auto itPos=line.begin(); itPos<line.end(); ++itPos) {
					std::cout << Encoding::unicodeToUtf8(*itPos);
				}
				std::cout << std::endl;
			}
		}
	}

};


/**
 * Splits Rail source code into it's functions, storing each function as RailFunction-Object
 *
 * @author Miro B.
 */
class Lexer {

public:
	std::vector<std::shared_ptr<RailFunction>> functions;

	Lexer();
	virtual ~Lexer();

	/**
	 * Splits Rail source code into it's functions, storing each function as RailFunction-Object
	 */
	void lex(const std::string srcFile);


	/**
	 * Determines weather the Lexer found at least one Rail function
	 */
	inline bool hasFunctions() {
		return functions.size() > 0;
	}
};

#endif /* LEXER_H_ */
