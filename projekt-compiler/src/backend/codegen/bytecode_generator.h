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
  static void add_invoke_virtual(const std::string& method, ConstantPool& pool, std::vector<char>& code);
  static void add_static_field(const std::string& field, ConstantPool& pool, std::vector<char>& code);
  static void add_new_object(const std::string& class_name, ConstantPool& pool, std::vector<char>& code);
  static void add_index(uint16_t indexInPool, std::vector<char>& result);
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
  static const char ICONST_1;
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
//CALL
void call_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
// LIST OPERATIONS
void null_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void list_push_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void list_pop_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
//BOOLEAN ARITHMETIC
void false_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void greater_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void equal_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void true_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
//IO OPERATIONS
void boom_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void eof_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void input_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void underflow_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
void type_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
//CONTROL STRUCTURE
void if_or_while_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
#endif // BYTECODE_GENERATOR_H
