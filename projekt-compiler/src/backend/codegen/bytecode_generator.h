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

  typedef void (*func_ptr) (ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
  static const std::map<Command::Type, func_ptr> CODE_FUNC_MAPPING;
};

void output_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void push_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void add_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void sub_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void mult_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void div_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void mod_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
#endif // BYTECODE_GENERATOR_H
