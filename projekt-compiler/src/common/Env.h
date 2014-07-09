/*
 * Env.h
 *
 *  Created on: Jun 15, 2014
 *      Author: Miro B.
 */

#ifndef ENV_H_
#define ENV_H_


#include <time.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <exception>
#include <vector>

/**
 * Possible Warning/Exception sources
 */
enum Source {
  ENVIRONMENT,
  FRONTEND,
  FRONTEND_LEXER,
  FRONTEND_PARSER,
  ASG,
  ASG_SERIALIZE,
  ASG_DESERIALIZE,
  ASG_GRAPHVIZ,
  BACKEND,
  BACKEND_BYTECODE_GENERATOR,
  BACKEND_BYTECODE_WRITER,
  BACKEND_CONSTANT_POOL,
  UNKNOWN,
};

/**
 * Formats a line, pos tuple
 */
static inline std::string getLineString(int32_t line=-1, int32_t pos=-1) {
  std::stringstream s;
  if (line >= 0 && pos >= 0) {
    s << "[@" << line << "," << pos << "]";
  }
  return s.str();
}

/**
 * Returns the String-representation of a Source
 *
 * @param src	Source to be converted
 * @return		string-representation
 */
static inline std::string getSourceName(Source src) {
  switch (src) {
    case ENVIRONMENT:                 return "Environment";
    case FRONTEND:                    return "Frontend";
    case FRONTEND_LEXER:              return "Frontend-Lexer";
    case FRONTEND_PARSER:             return "Frontend-Parser";
    case ASG:                         return "Asg";
    case ASG_SERIALIZE:               return "Asg-Serialize";
    case ASG_DESERIALIZE:             return "Asg-Deserialize";
    case ASG_GRAPHVIZ:                return "Asg-GraphViz";
    case BACKEND:                     return "Backend";
    case BACKEND_BYTECODE_GENERATOR:  return "Backend-Bytecode-Generator";
    case BACKEND_BYTECODE_WRITER:     return "Backend-Bytecode-Writer";
    case BACKEND_CONSTANT_POOL:       return "Backend-Constant-Pool";
    case UNKNOWN:
    default:                          return "Unknown";
  }
}

/**
 * Offers a default Exception for Compiler-Exceptions
 *
 * @author Miro B.
 */
class EnvException: public std::exception {

private:
	const std::string msg;

public:
	/**
	* Creates a formatted Exception
	*
	* @param src	Error source
	* @param emsg	Exception Message
	* @param line	optional, line number
	* @param pos	optional, position number
	*/
	EnvException(Source src, const std::string&& emsg, int32_t line=-1, int32_t pos=-1)
	: msg("[Exception][" + getSourceName(src) + "]" + getLineString(line, pos) + " " + emsg) {

	}
	inline void showMessage() {
		std::cerr << msg << std::endl;
	}
private:
	virtual const char* what() const throw() {
		return (msg).c_str();
	}
};

/**
 * "static" class to parse commandline parameters, store/show warnings and output styling
 *
 * @author Miro B.
 */
class Env {

private:
	static std::string srcFile;
	static std::string srcDeserialize;
	static std::string dstClassFile;
	static std::string dstClassName;
	static std::string dstSerialize;
	static std::string dstGraphviz;
	static bool isQuiet;
	static bool isHelp;


public:
	// ------------------------------------------------------------------------------
	// Init
	// ------------------------------------------------------------------------------

	/**
	 * Assures the presence of folder "io"
	 */
	static inline void initIoDirectory() {
		if(access("io", F_OK) == -1) {
			#ifdef _WIN32
			int rMkdir = mkdir("io");
			#else
			int rMkdir = mkdir("io", 0777);
			#endif
			if (rMkdir == -1) {
				throw EnvException(ENVIRONMENT, "Folder io cannot be created");
			}
		}
	}


	/**
	 * Parses Parameters
	 *
	 * @param argc	Parameter from main()
	 * @param argv	Parameter from main()
	 */
	static void parseParams(int argc, char *argv[]) {
		srcFile = "";
		srcDeserialize = "";
		dstClassFile = "";
		dstClassName = "";
		dstSerialize = "";
		dstGraphviz = "";
		isQuiet = false;
		isHelp = false;

		// Parse parameters
		for (int i = 0; i < argc; ++i) {
			if (i + 1 < argc && strcmp(argv[i], "-i") == 0) { srcFile = argv[++i]; }
			else if (i + 1 < argc && strcmp(argv[i], "-d") == 0) { srcDeserialize = argv[++i]; }
			else if (i + 1 < argc && strcmp(argv[i], "-s") == 0) { dstSerialize = argv[++i]; }
			else if (i + 1 < argc && strcmp(argv[i], "-o") == 0) { dstClassFile = argv[++i]; }
			else if (i + 1 < argc && strcmp(argv[i], "-g") == 0) { dstGraphviz = argv[++i]; }
			else if (strcmp(argv[i], "-q") == 0) { isQuiet = true; }
			else if (i + 1 < argc && strcmp(argv[i], "-h") == 0) {
				isHelp = true;
				Env::printCaption("Command line help");
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
//		if (!hasSrcFile() && !hasSrcDeserialize()) { srcFile = "Tests/test-cases/helloworld.txt"; }
//		if (!hasDstClassfile()) { dstClassFile = "io/Main.class"; }
//		if (!hasDstSerialize()) { dstSerialize = "io/serialized.csv"; }
//		if (!hasDstGraphviz()) { dstGraphviz = "io/graphviz.dot"; }

		// reduce classname out of dstClassFile
		size_t pos = dstClassFile.find_last_of("\\/") + 1;
		dstClassName = dstClassFile.substr(pos, dstClassFile.find_last_of(".") - pos);


		// Sanitize
		if(hasSrcFile() && hasSrcDeserialize()) {
			srcDeserialize = "";
		}


		// Print parameters
		if(verbose()) {
			printCaption("Parameters");
			if(
				!hasSrcFile()
				&& !hasSrcDeserialize()
				&& !hasDstClassfile()
				&& !hasDstSerialize()
				&& !hasDstGraphviz()
				&& !isQuiet
				&& !isHelp
			) {
				std::cout << "  None set" << std::endl;
			}
			else {
				if (hasSrcFile()) { std::cout << "  -i " << srcFile << std::endl; }
				if (hasSrcDeserialize()) { std::cout << "  -d " << srcDeserialize << std::endl; }
				if (hasDstClassfile()) { std::cout << "  -o " << dstClassFile << std::endl; }
				if (hasDstSerialize()) { std::cout << "  -s " << dstSerialize << std::endl; }
				if (hasDstGraphviz()) { std::cout << "  -g " << dstGraphviz << std::endl; }
				if (isQuiet) { std::cout << "  -q" << std::endl; }
			}

			std::cout << std::endl;
		}


		// Test file-access
		if (hasSrcFile() && access(srcFile.c_str(), F_OK) != 0) {
			throw EnvException(ENVIRONMENT, "File not accessible: " + srcFile);
		}
		if (hasSrcDeserialize() && access(srcDeserialize.c_str(), F_OK) != 0) {
			throw EnvException(ENVIRONMENT, "File not accessible: " + srcDeserialize);
		}
	}

	/**
	 * @return If present, returns the source filename specified by -i, "" otherwise
	 */
	static inline std::string getSrcFile() {
		return srcFile;
	}

	/**
	 * @return If present, returns the source filename of a ASG-csv specified by -d, "" otherwise
	 */
	static inline std::string getSrcDeserialize() {
		return srcDeserialize;
	}

	/**
	 * @return If present, returns the destination filename for the resulting .class file (-o), "" otherwise
	 */
	static inline std::string getDstClassfile() {
		return dstClassFile;
	}

	/**
	* @return If present, returns the destination filename for the resulting .class file (-o), "" otherwise
	*/
	static inline std::string getDstClassName() {
		return dstClassName;
	}

	/**
	 * @return If present, returns the destination filename for ASG-serialisation (-s), "" otherwise
	 */
	static inline std::string getDstSerialize() {
		return dstSerialize;
	}

	/**
	 * @return If present, returns the destination filename for graphviz generation (-g), "" otherwise
	 */
	static inline std::string getDstGraphviz() {
		return dstGraphviz;
	}

	/**
	 * @return true if -q was specified, false otherwise
	 */
	static inline bool quiet() {
		return isQuiet;
	}

	/**
	 * @return false if -q was specified, true otherwise
	 */
	static inline bool verbose() {
		return !isQuiet;
	}

	/**
	 * Check if file specified by -i is usable
	 */
	static inline bool hasSrcFile() {
		return srcFile != "";
	}

	/**
	 * Check if file specified by -d is usable
	 */
	static inline bool hasSrcDeserialize() {
		return srcDeserialize != "";
	}

	/**
	 * Check if file specified by -o is usable
	 */
	static inline bool hasDstClassfile() {
		return dstClassFile != "";
	}

	/**
	 * Check if file specified by -s is usable
	 */
	static inline bool hasDstSerialize() {
		return dstSerialize != "";
	}

	/**
	 * Check if file specified by -g is usable
	 */
	static inline bool hasDstGraphviz() {
		return dstGraphviz != "";
	}

// ------------------------------------------------------------------------------
// Debugging
// ------------------------------------------------------------------------------

private:
	static std::vector<std::string> warnings;
	static std::vector<std::string> errors;
	static bool warningsOccurred;
	static bool errorsOccurred;


public:
	/**
	* Stores msg as warning
	* @param src	Source of the warning (e.g. "Lexer", "Parser", "ASG", "Backend")
	* @param msg	Textual warning
	* @param line	(optional) line corresponding to the warning
	* @param pos	(optional) cursor position corresponding to the warning
	*/
	static inline void addWarning(Source src, std::string msg, int32_t line=-1, int32_t pos=-1) {
		warnings.push_back("[Warning][" + getSourceName(src) + "]" + getLineString(line, pos) + " " + msg);
	}

	/**
	 * Determines if warnings exist(ed)
	 */
	static inline bool hasWarnings() {
		return warningsOccurred;
	}





	/**
	 * Show msg as error
	 * @param src	Source (e.g. "Lexer", "Parser", "ASG", "Backend")
	 * @param msg	Textual error
	 * @param line	(optional) line corresponding to the warning
	 * @param pos	(optional) cursor position corresponding to the warning
	 */
	static inline void addError(Source src, std::string msg, int32_t line = -1, int32_t pos = -1) {
		errorsOccurred = true;
		errors.push_back("[Error][" + getSourceName(src) + "]" + getLineString(line, pos) + " " + msg);
	}

	/**
	 * Determines if errors exist(ed)
	 */
	static inline bool hasErrors() {
		return errorsOccurred;
	}


	/**
	 * Dumps all pending warnings to std::cout
	 */
	static inline void showStatus() {
		for(std::string s : warnings) {
			std::cerr << s << std::endl;
		}
		warnings.clear();
		for(std::string s : errors) {
			std::cerr << s << std::endl;
		}
		errors.clear();
	}

// ------------------------------------------------------------------------------
// Timer
// ------------------------------------------------------------------------------
private:
	static timespec timeStart;

public:
	static inline void initTimer() {
		clock_gettime(CLOCK_MONOTONIC_RAW, &timeStart);
	}

// ------------------------------------------------------------------------------
// Prints
// ------------------------------------------------------------------------------

public:
	/**
	 * Caption formatting and std::cout
	 * @param head	caption text
	 */
	static inline void printCaption(const std::string head, bool forcePrint=false) {
		if(Env::verbose() || forcePrint) {
			std::cout << std::endl;
			std::cout << std::endl << "$ '" << head << "' (--)";
			std::cout << std::endl << " \\";
//			std::cout << std::endl << "  ---[ " << head << " ]";
//			for (size_t i = head.length(); i < 80; ++i) {
//				std::cout << "-";
//			}

			for (size_t i = 2; i < 80; ++i) {
				std::cout << "-";
			}


			std::cout << "#";
			std::cout << std::endl;
			std::cout << std::endl;
		}
	}



	static inline void printBuildStatus(bool success) {
		if(Env::verbose()) {
			timespec timeEnd;
			clock_gettime(CLOCK_MONOTONIC_RAW, &timeEnd);
			timespec diff;
			diff.tv_sec = timeEnd.tv_sec - timeStart.tv_sec;
			diff.tv_nsec = timeEnd.tv_nsec - timeStart.tv_nsec;
			double time = diff.tv_sec + (((double)diff.tv_nsec / 1000000))/1000;

			if(success && !hasErrors()) {
				std::cout << "Build successful (took " << ((int)(time*10000))/10000.0 << "s)" << std::endl;
			}
			else {
				std::cout << "Build error(s) occurred (took " << ((int)(time*10000))/10000.0 << "s)" << std::endl;
			}
		}
	}

};

#endif /* ENV_H_ */
