/*!
* \mainpage backend.cc
* \author Backend group & friends
* \date SoSe 2014
*
* This class generates the target code from an input graph.
*
*/
#include "backend/backend.h"

#include "backend/classfile/classfile_writer.h"
#include "backend/codegen/bytecode_generator.h"
#include "frontend/Graphs.h"

/*!
 * \brief Generates target code from a serialized graph
 * \param graphIn The serialized graph
 * \param codeOut The target code on the output stream
 *
 * \returns Generated class-file
 */
Backend::Status Backend::Generate(const std::string& graphIn,
									std::ostream& codeOut) {
  Graphs graphs;
  graphs.unmarshall(graphIn, ';');
  Backend::Status ret = Backend::Generate(graphs, codeOut);
  return ret;
}
/*!
* \brief Generates target code from a graph
* \param graphs The graph
* \param codeOut The target code on the output stream
*
* Start in the graph at the main function. Went on with generating the constant pool
* to write it and the other member as java class-file.
* Is called after the serialized graph was unmarshalled.
*
* \returns Status of the created file
*/
Backend::Status Backend::Generate(Graphs& graphs, std::ostream& codeOut) {
  std::string entryFunctionName("main");
  Graphs::Graph_ptr mainFunction = graphs.find(entryFunctionName);
  ConstantPool constantPool;

  std::vector<char> mainCode = BytecodeGenerator::GenerateCodeFromFunctionGraph(mainFunction,
		  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	  	constantPool);
  std::map<std::string, std::vector<char>&> codeMap{{"main", mainCode}};

  ClassfileWriter writer(ClassfileWriter::JAVA_7, constantPool, codeMap, codeOut);
  writer.WriteClassfile();
  return Backend::Status::SUCCESS;
}

/*!
 * \brief Generates status message
 * \param status Status of backend function
 *
 * \returns nothing (TODO)
 */
std::string Backend::ErrorMessage(Backend::Status status) {
  switch (status) {
    case Backend::Status::SUCCESS:
      return "";
      break;
      // TODO handle other cases
    default:
      return "";
  }
}
