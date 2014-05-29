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


#include <common/Encoding.h>
#include <frontend/IO_Exception.h>


class RailFunction {

	friend class Lexer;

private:
	uint32_t width = 0;
	uint32_t height = 0;
	std::string name;
	std::vector<std::vector<uint32_t>> data;


public:
	RailFunction(std::string name) {
		this->name = name;
	}
	virtual ~RailFunction() {}

	inline std::string getName() const {
		return this->name;
	}


	inline std::vector<std::vector<uint32_t>> getData() {
		return this->data;
	}

	inline uint32_t getWidth() {
		return this->width;
	}
	inline uint32_t getHeight() {
		return this->height;
	}
	inline uint32_t get(uint32_t line, uint32_t pos) const {
		if(data.size() > line && data[line].size() > pos) {
			return data[line][pos];
		}
		return 0;
	}

	void dump() {
		std::cout << "--- Lexed Rail Function -------------------------------------------------------" << std::endl;
		for(auto it=data.begin(); it<data.end(); ++it) {
			std::vector<uint32_t> line = *it;
			for(auto itPos=line.begin(); itPos<line.end(); ++itPos) {
				std::cout << Encoding::unicodeToUtf8(*itPos);
			}
			std::cout << std::endl;
		}
		std::cout << "-------------------------------------------------------------------------------" << std::endl;
	}

};


class Lexer {

public:
	std::vector<std::shared_ptr<RailFunction>> functions;

	Lexer();
	virtual ~Lexer();
	void lex(std::string srcFile, bool asUtf8=false);
};

#endif /* LEXER_H_ */
