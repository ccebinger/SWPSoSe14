/*
 * Status.h
 *
 *  Created on: Jun 7, 2014
 *      Author: asd
 */

#ifndef STATUS_H_
#define STATUS_H_

#include <vector>


enum Source : std::string {
	FRONTEND,
	FRONTEND_LEXER,
	FRONTEND_PARSER,
	ASG,
	BACKEND,
	BACKEND_CONSTANTPOOL,
	BACKEND_CLASSFILEWRITER,
	BACKEND_BYTECODEGENERATOR,
};


class Status {

private:
	std::vector<std::string> warnings;
	std::vector<std::string> errors;


public:

	Status() {

	}

	virtual ~Status() {

	}


	inline void addWarning(Source src, std::string msg, int32_t line=-1, int32_t pos=-1) {
		warnings.push_back(
			"[warning][" + source2String(src) + "]" + getLineString(line, pos) + " " + msg
		);
	}


	inline void addError(Source src, std::string msg, int32_t line=-1, int32_t pos=-1) {
		errors.push_back(
			"[ error ][" + source2String(src) + "]" + getLineString(line, pos) + " " + msg
		);
	}


	inline bool hasWarnings() {
		return warnings.size() > 0;
	}


	inline bool hasErrors() {
		return errors.size() > 0;
	}


	void dump() {
		for(auto it=warnings.begin(); it<warnings.end(); ++it) {
			std::cout << "(WW) " << (*it) << std::endl;
		}
		for(auto it=errors.begin(); it<errors.end(); ++it) {
			std::cerr << "(EE) " << (*it) << std::endl;
		}
	}


private:

	inline std::string getLineString(int32_t line=-1, int32_t pos=-1) const {
		if(line >= 0 && pos >= 0) {
			return "[@ " + line + "," + pos + "]";
		}
		return "";
	}

	inline std::string source2String(Source src) const {
		switch(Source) {
			case FRONTEND:                  return "fe    ";
			case FRONTEND_LEXER:            return "fe-l  ";
			case FRONTEND_PARSER:           return "fe-p  ";
			case ASG:                       return "asg   ";
			case BACKEND:                   return "be    ";
			case BACKEND_CONSTANTPOOL:      return "be-cp ";
			case BACKEND_CLASSFILEWRITER:   return "be-cfw";
			case BACKEND_BYTECODEGENERATOR: return "be-bg ";
			default:						return "???   ";
		}
	}

};

#endif /* STATUS_H_ */
