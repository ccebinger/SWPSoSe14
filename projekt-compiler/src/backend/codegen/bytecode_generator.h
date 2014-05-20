#ifndef BYTECODE_GENERATOR_H
#define BYTECODE_GENERATOR_H

#include "backend/classfile/constant_pool.h"
#include "common/ast/ast.h"
#include "frontend/Graphs.h"

class BytecodeGenerator
{
public:
  static std::vector<char> GenerateCodeFromFunctionGraph(Graphs::Graph_ptr graph,
                               ConstantPool& constantPool);

private:
  /**
   * Nur statische Methoden.
   */
  BytecodeGenerator();
};

#endif // BYTECODE_GENERATOR_H
