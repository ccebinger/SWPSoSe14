/*
 * Status.h
 *
 *  Created on: Jun 7, 2014
 *      Author: Miro B.
 */

#ifndef STATUS_H_
#define STATUS_H_

#include <exception>
#include <vector>
#include <sstream>


class StatusException: public std::exception {
private:
	const std::string msg;
public:
	StatusException(const std::string&& emsg) : msg(emsg) {}
private:
	virtual const char* what() const throw() {
		return (msg).c_str();
	}
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


	inline void addWarning(std::string src, std::string msg, int32_t line=-1, int32_t pos=-1) {
		warnings.push_back("[" + src + "]" + getLineString(line, pos) + " " + msg);
	}


	inline void addError(std::string src, std::string msg, int32_t line=-1, int32_t pos=-1) {
		errors.push_back("[" + src + "]" + getLineString(line, pos) + " " + msg);
	}


	inline bool hasWarnings() {
		return warnings.size() > 0;
	}

	inline bool hasErrors() {
		return errors.size() > 0;
	}


	inline bool hasNoWarnings() {
		return warnings.size() == 0;
	}


	inline bool hasNoErrors() {
		return errors.size() == 0;
	}



	inline void clearWarnings() {
		warnings.clear();
	}

	inline void clearErrors() {
		errors.clear();
	}



	void dumpWarnings() {
		for(auto it=warnings.begin(); it<warnings.end(); ++it) {
			std::cout << "[warning]" << (*it) << std::endl;
		}
	}

	void dumpErrors() {
		for(auto it=errors.begin(); it<errors.end(); ++it) {
			std::cerr << "[ error ]" << (*it) << std::endl;
		}
	}



	void dump() {
		dumpWarnings();
		dumpErrors();
	}


private:

	inline std::string getLineString(int32_t line=-1, int32_t pos=-1) const {
		std::stringstream s;
		if(line >= 0 && pos >= 0) {
			s << "[@ " << line << "," << pos << "]";

		}
		return s.str();
	}

};

#endif /* STATUS_H_ */
