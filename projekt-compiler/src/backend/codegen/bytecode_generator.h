#ifndef BYTECODE_GENERATOR_H
#define BYTECODE_GENERATOR_H

#include <backend/classfile/constant_pool.h>
#include <common/ast/ast.h>
#include <frontend/Graphs.h>

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
public:
  static const char ILOAD_0;
  static const char ICONST_0;
  static const char ISTORE_0;
  static const char ALOAD_1;
  static const char ALOAD_2;
  static const char ASTORE_1;
  static const char ASTORE_2;
  static const char NEW;
  static const char INVOKE_VIRTUAL;
  static const char GET_STATIC;
  static const char RETURN ;
  static const char LDC;
  static const char DUP;
  static const char IADD;
  static const char ISUB;
  static const char IMULT;
  static const char IDIV;
  static const char IREM;

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
