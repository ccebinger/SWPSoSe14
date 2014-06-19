/*
 * Env.h
 *
 *  Created on: Jun 15, 2014
 *      Author: Miro B.
 */

#ifndef PROJEKT_COMPILER_SRC_COMMON_ENV_H_
#define PROJEKT_COMPILER_SRC_COMMON_ENV_H_

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <exception>
#include <vector>
#include <string>

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
    default:                          return "???";
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
  static std::string dstSerialize;
  static std::string dstGraphviz;
  static bool isQuiet;

  static std::vector<std::string> warnings;

 public:
  static void initIoDirectory();
  static void parseParams(int argc, char *argv[]);
  static inline std::string getSrcFile();
  static inline std::string getSrcDeserialize();
  static inline std::string getDstClassfile();
  static inline std::string getDstSerialize();
  static inline std::string getDstGraphviz();
  static inline bool quiet();
  static inline bool verbose();
  static inline bool hasSrcFile();
  static inline bool hasSrcDeserialize();
  static inline bool hasDstClassfile();
  static inline bool hasDstSerialize();
  static inline bool hasDstGraphviz();

  static inline void addWarning(Source src, std::string msg,
                         int32_t line=-1, int32_t pos=-1);
  static inline bool hasWarnings();
  static inline bool hasNoWarnings();
  static inline void clearWarnings();
  static void showWarnings();

  static inline void printCaption(const std::string& head);
};

#endif  // PROJEKT_COMPILER_SRC_COMMON_ENV_H_
