/*
 * Env.h
 *
 *  Created on: Jun 15, 2014
 *      Author: Miro B.
 */

#ifndef ENV_H_
#define ENV_H_


#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <exception>
#include <vector>




class EnvException: public std::exception {
private:
	const std::string msg;
public:
	EnvException(const std::string&& emsg) : msg(emsg) {}
private:
	virtual const char* what() const throw() {
		return (msg).c_str();
	}
};






class Env {

private:
	static std::string srcFile;
	static std::string srcDeserialize;
	static std::string dstClassFile;
	static std::string dstSerialize;
	static std::string dstGraphviz;
	static bool isQuiet;


public:
	// ------------------------------------------------------------------------------
	// Init
	// ------------------------------------------------------------------------------

	static void initIoDirectory() {
		if(access("io", F_OK) == -1) {
			#ifdef _WIN32
			int rMkdir = mkdir("io");
			#else
			int rMkdir = mkdir("io", 0777);
			#endif
			if (rMkdir == -1) {
				throw EnvException("Env: Folder io cannot be created");
			}
		}
	}


	static void parseParams(int argc, char *argv[]) {
		srcFile = "";
		srcDeserialize = "";
		dstClassFile = "";
		dstSerialize = "";
		dstGraphviz = "";
		isQuiet = false;

		// Parse parameters
		for(int i = 0; i < argc; ++i) {
			if(i+1 < argc && strcmp(argv[i], "-i") == 0) {
				srcFile = argv[++i];
			}
			else if(i+1 < argc && strcmp(argv[i], "-d") == 0) {
				srcDeserialize = argv[++i];
			}
			else if(i+1 < argc && strcmp(argv[i], "-s") == 0) {
				dstSerialize = argv[++i];
			}
			else if(i+1 < argc && strcmp(argv[i], "-o") == 0) {
				dstClassFile = argv[++i];
			}
			else if(i+1 < argc && strcmp(argv[i], "-g") == 0) {
				dstGraphviz = argv[++i];
			}
			else if(strcmp(argv[i], "-q") == 0) {
				isQuiet = true;
			}
			else if(i+1 < argc && strcmp(argv[i], "-h") == 0) {
				std::cout << "Command line help:" << std::endl;
				std::cout << " -i <file> specifies input sourcefile" << std::endl;
				std::cout << " -d <file> deserialize csv to graph" << std::endl;
				std::cout << " -s <file> serializes graph to <file>" << std::endl;
				std::cout << " -o <file> specifies .class output file" << std::endl;
				std::cout << " -g <file> create graphViz" << std::endl;
				std::cout << " -q quiet" << std::endl;
				std::cout << " -h help" << std::endl;
				std::cout << "General:" << std::endl;
				std::cout << " -i > -d only use one of these." << std::endl;
			}
		}



		// Defaults
		if(!hasSrcFile()  && !hasSrcDeserialize()) { srcFile = "Tests/test-cases/helloworld.txt"; }
		if(!hasDstClassfile())                     { dstClassFile  = "io/out.class"; }
		if(!hasDstSerialize())                     { dstSerialize = "io/serialized.csv"; }
		if(!hasDstGraphviz())                      { dstGraphviz = "io/graphviz.dot"; }


		// Sanitize
		if(hasSrcFile() && hasSrcDeserialize()) {
			srcDeserialize = "";
		}


		// Print parameters
		if(verbose()) {
			printCaption("Using parameters");
			if(hasSrcFile())         { std::cout << "  -i " << srcFile << std::endl; }
			if(hasSrcDeserialize())  { std::cout << "  -d " << srcDeserialize << std::endl; }
			if(hasDstClassfile())    { std::cout << "  -o " << dstClassFile << std::endl; }
			if(hasDstSerialize())    { std::cout << "  -s " << dstSerialize << std::endl; }
			if(hasDstGraphviz())     { std::cout << "  -g " << dstGraphviz << std::endl; }
			if(isQuiet)              { std::cout << "  -q"  << std::endl; }
			std::cout << std::endl;
		}


		// Test file-access
		if(hasSrcFile() && access(srcFile.c_str(), F_OK) != 0) {
			throw EnvException("File not accessible: " + srcFile);
		}
		if(hasSrcDeserialize() && access(srcDeserialize.c_str(), F_OK) != 0) {
			throw EnvException("File not accessible: " + srcDeserialize);
		}
	}


	static inline std::string getSrcFile()          { return srcFile; }
	static inline std::string getSrcDeserialize()   { return srcDeserialize; }
	static inline std::string getDstClassfile()     { return dstClassFile; }
	static inline std::string getDstSerialize()     { return dstSerialize; }
	static inline std::string getDstGraphviz()      { return dstGraphviz; }
	static inline bool quiet()                      { return isQuiet; }
	static inline bool verbose()                    { return !isQuiet; }

	static inline bool hasSrcFile()        { return srcFile != ""; }
	static inline bool hasSrcDeserialize() { return srcDeserialize != ""; }
	static inline bool hasDstClassfile()   { return dstClassFile != ""; }
	static inline bool hasDstSerialize()   { return dstSerialize != ""; }
	static inline bool hasDstGraphviz()    { return dstGraphviz != ""; }




// ------------------------------------------------------------------------------
// Debugging
// ------------------------------------------------------------------------------

private:
	static std::vector<std::string> warnings;

	static inline std::string getLineString(int32_t line=-1, int32_t pos=-1) {
		std::stringstream s;
		if(line >= 0 && pos >= 0) {
			s << "[@ " << line << "," << pos << "]";
		}
		return s.str();
	}

public:
	static inline void addWarning(std::string src, std::string msg, int32_t line=-1, int32_t pos=-1) {
		warnings.push_back("[" + src + "]" + getLineString(line, pos) + " " + msg);
	}
	static inline bool hasWarnings()   { return warnings.size() > 0; }
	static inline bool hasNoWarnings() { return warnings.size() == 0; }
	static inline void clearWarnings() { warnings.clear(); }
	static void showWarnings() {
		for(std::string s : warnings) {
			std::cout << "[warning]" << s << std::endl;
		}
		clearWarnings();
	}




// ------------------------------------------------------------------------------
// Prints
// ------------------------------------------------------------------------------

public:
	static inline void printCaption(const std::string head) {
		if(Env::verbose()) {
			std::cout << std::endl << "### " << head << " ";
			for(size_t i=head.length(); i<80; ++i) {
				std::cout << "#";
			}
			std::cout << std::endl;
		}
	}
};








#endif /* ENV_H_ */
