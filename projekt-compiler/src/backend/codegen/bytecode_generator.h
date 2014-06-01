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

  typedef void (*func_ptr) (ConstantPool& pool, std::vector<char>& code);

   static const std::map<Command::Type, func_ptr> CODE_FUNC_MAPPING;
  void push_ByteCode(ConstantPool& pool, std::vector<char>& code);
};

  void output_ByteCode(ConstantPool& pool, std::vector<char>& code);
#endif // BYTECODE_GENERATOR_H
