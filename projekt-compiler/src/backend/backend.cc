#include "backend/backend.h"

#include "backend/classfile/classfile_writer.h"
#include "backend/codegen/bytecode_generator.h"
#include "frontend/Graphs.h"

Backend::Status Backend::Generate(const std::string& graphIn,
                                  std::ostream& codeOut) {
  Graphs graphs;
  graphs.unmarshall(graphIn, ';');
  Backend::Status ret = Backend::Generate(graphs, codeOut);
  return ret;
}

Backend::Status Backend::Generate(Graphs& graphs, std::ostream& codeOut) {
  std::string entryFunctionName("main");
  Graphs::Graph_ptr mainFunction = graphs.find(entryFunctionName);
  ConstantPool constantPool;
  std::vector<char> mainCode = BytecodeGenerator::GenerateCodeFromFuntionGraph(mainFunction, constantPool);
  std::map<std::string, std::vector<char>&> codeMap{{"main", mainCode}};

  ClassfileWriter writer(ClassfileWriter::JAVA_7, constantPool, codeMap, codeOut);
  writer.WriteClassfile();
  return Backend::Status::SUCCESS;
}

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
