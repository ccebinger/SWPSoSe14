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


  static const char ILOAD_0 = '\x1a';
  static const char ICONST_0 = '\x03';
  static const char ISTORE_0 = '\x3b';
  static const char ALOAD_1 = '\x2b';
  static const char ALOAD_2 = '\x2c';
  static const char ASTORE_1 = '\x4c';
  static const char ASTORE_2 = '\x4d';
  static const char NEW = '\xbb';
  static const char INVOKE_VIRTUAL = '\xb6';
  static const char GET_STATIC = '\xb2';
  static const char RETURN = '\xb1';
  static const char LDC = '\x12';
  static const char DUP = '\x59';
  static const char IADD = '\x60';
  static const char ISUB = '\x64';
  static const char IMULT = '\x68';
  static const char IDIV = '\x6c';
  static const char IREM = '\x70';
};

void output_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void push_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
//ARITMETIC OPERATIONS
void add_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void sub_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void mult_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void div_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void mod_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
//STRING OPERATIONS
void cut_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void append_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void size_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
#endif // BYTECODE_GENERATOR_H
