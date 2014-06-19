/*
 * Env.cpp
 *
 *  Created on: Jun 15, 2014
 *      Author: Miro B.
 */


#include <common/Env.h>

std::string Env::srcFile = "";
std::string Env::srcDeserialize = "";
std::string Env::dstClassFile = "";
std::string Env::dstSerialize = "";
std::string Env::dstGraphviz = "";
bool Env::isQuiet = false;

std::vector<std::string> Env::warnings;

/**
 * Assures the presence of folder "io"
 */
void Env::initIoDirectory() {
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

void Env::parseParams(int argc, char *argv[]) {
  srcFile = "";
  srcDeserialize = "";
  dstClassFile = "";
  dstSerialize = "";
  dstGraphviz = "";
  isQuiet = false;

  // Parse parameters
  for (int i = 0; i < argc; ++i) {
    if (i+1 < argc && strcmp(argv[i], "-i") == 0) {
      srcFile = argv[++i];
    }
    else if (i+1 < argc && strcmp(argv[i], "-d") == 0) {
      srcDeserialize = argv[++i];
    }
    else if (i+1 < argc && strcmp(argv[i], "-s") == 0) {
      dstSerialize = argv[++i];
    }
    else if (i+1 < argc && strcmp(argv[i], "-o") == 0) {
      dstClassFile = argv[++i];
    }
    else if (i+1 < argc && strcmp(argv[i], "-g") == 0) {
      dstGraphviz = argv[++i];
    }
    else if (strcmp(argv[i], "-q") == 0) {
      isQuiet = true;
    }
    else if (i+1 < argc && strcmp(argv[i], "-h") == 0) {
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
  if (!hasSrcFile()  && !hasSrcDeserialize()) { srcFile = "Tests/test-cases/helloworld.txt"; }
  if (!hasDstClassfile())                     { dstClassFile  = "io/Main.class"; }
  if (!hasDstSerialize())                     { dstSerialize = "io/serialized.csv"; }
  if (!hasDstGraphviz())                      { dstGraphviz = "io/graphviz.dot"; }

  // Sanitize
  if (hasSrcFile() && hasSrcDeserialize()) {
    srcDeserialize = "";
  }

  // Print parameters
  if (verbose()) {
    printCaption("Using parameters");
    if (hasSrcFile())         { std::cout << "  -i " << srcFile << std::endl; }
    if (hasSrcDeserialize())  { std::cout << "  -d " << srcDeserialize << std::endl; }
    if (hasDstClassfile())    { std::cout << "  -o " << dstClassFile << std::endl; }
    if (hasDstSerialize())    { std::cout << "  -s " << dstSerialize << std::endl; }
    if (hasDstGraphviz())     { std::cout << "  -g " << dstGraphviz << std::endl; }
    if (isQuiet)              { std::cout << "  -q"  << std::endl; }
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
inline std::string Env::getSrcFile() {
  return srcFile;
}

/**
 * @return If present, returns the source filename of a ASG-csv specified by -d, "" otherwise
 */
inline std::string Env::getSrcDeserialize() {
  return srcDeserialize;
}

/**
 * @return If present, returns the destination filename for the resulting .class file (-o), "" otherwise
 */
inline std::string Env::getDstClassfile() {
  return dstClassFile;
}

/**
 * @return If present, returns the destination filename for ASG-serialisation (-s), "" otherwise
 */
inline std::string Env::getDstSerialize() {
  return dstSerialize;
}

/**
 * @return If present, returns the destination filename for graphviz generation (-g), "" otherwise
 */
inline std::string Env::getDstGraphviz() {
  return dstGraphviz;
}

/**
 * @return true if -q was specified, false otherwise
 */
inline bool Env::quiet() {
  return isQuiet;
}

/**
 * @return false if -q was specified, true otherwise
 */
inline bool Env::verbose() {
  return !isQuiet;
}

/**
 * Check if file specified by -i is usable
 */
inline bool Env::hasSrcFile() {
  return srcFile != "";
}

/**
 * Check if file specified by -d is usable
 */
inline bool Env::hasSrcDeserialize() {
  return srcDeserialize != "";
}

/**
 * Check if file specified by -o is usable
 */
inline bool Env::hasDstClassfile()   { return dstClassFile != ""; }

/**
 * Check if file specified by -s is usable
 */
inline bool Env::hasDstSerialize()   { return dstSerialize != ""; }

/**
 * Check if file specified by -g is usable
 */
inline bool Env::hasDstGraphviz()    { return dstGraphviz != ""; }

// ------------------------------------------------------------------------------
// Debugging
// ------------------------------------------------------------------------------

/**
 * Stores msg as warning
 * @param src	Source of the warning (e.g. "Lexer", "Parser", "ASG", "Backend")
 * @param msg	Textual warning
 * @param line	(optional) line corresponding to the warning
 * @param pos	(optional) cursor position corresponding to the warning
 */
inline void Env::addWarning(Source src, std::string msg,
                            int32_t line=-1, int32_t pos=-1) {
  warnings.push_back("[Warning][" + getSourceName(src) + "]" +
                     getLineString(line, pos) + " " + msg);
}

/**
 * Determines if warnings exist
 */
inline bool Env::hasWarnings() {
  return warnings.size() > 0;
}

/**
 * Determines if no warnings exist
 */
inline bool Env::hasNoWarnings() {
  return warnings.size() == 0;
}

/**
 * Clears all pending warnings
 */
inline void Env::clearWarnings() {
  warnings.clear();
}

/**
 * Dumps all pending warnings to std::cout
 */
void Env::showWarnings() {
  for (std::string s : warnings) {
    std::cout << s << std::endl;
  }
  clearWarnings();
}

// ------------------------------------------------------------------------------
// Prints
// ------------------------------------------------------------------------------

/**
 * Caption formatting and std::cout
 * @param head	caption text
 */
inline void Env::printCaption(const std::string& head) {
  if (Env::verbose()) {
    std::cout << std::endl << "### " << head << " ";
    for (size_t i=head.length(); i<80; ++i) {
      std::cout << "#";
    }
    std::cout << std::endl;
  }
