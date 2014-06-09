#ifndef BYTECODE_GENERATOR_H
#define BYTECODE_GENERATOR_H

#include <backend/classfile/constant_pool.h>
#include <common/ast/ast.h>
#include <frontend/Graphs.h>
#include <ios>
#include <stdexcept>

class BytecodeGenerator
{
public:
  static std::vector<char> GenerateCodeFromFunctionGraph(Graphs::Graph_ptr graph,
                               ConstantPool& constantPool);
  static void add_conditional_with_instruction(char conditional_stmt, char* conditional_body, std::vector<char>& result);
  static void add_invoke_virtual(const std::string& method, ConstantPool& pool, std::vector<char>& code);
  static void add_static_field(const std::string& field, ConstantPool& pool, std::vector<char>& code);
  static void add_new_object(const std::string& class_name, ConstantPool& pool, std::vector<char>& code);
  static void add_index(uint16_t indexInPool, std::vector<char>& result);
  static void add_class(const std::string& class_name, ConstantPool& constantPool, std::vector<char>& result);
  static void add_instance_of(const std::string& class_name, ConstantPool& constantPool, std::vector<char>& result);
  static void add_type_check(const std::string& class_name, ConstantPool& constantPool, std::vector<char>& result);
  static void add_throw_exception(const std::string& class_name, ConstantPool& constantPool, std::vector<char>& result);
private:
  /**
   * Nur statische Methoden.
   */
  BytecodeGenerator();

  typedef void (*func_ptr) (ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node);
  static const std::map<Command::Type, func_ptr> CODE_FUNC_MAPPING;
public:


  enum MNEMONIC {
    ILOAD_0 = '\x1a',
    ILOAD_1 = '\x1b',
    ILOAD_2 = '\x1c',
    ICONST_0 = '\x03',
    ICONST_1 = '\x04',
    ISTORE_0 = '\x3b',
    ISTORE_1 = '\x3c',
    ISTORE_2 = '\x3d',
    IF_ICMPLE = '\xa4',
    IF_ICMPNE = '\xa6',
    ALOAD_1 = '\x2b',
    ALOAD_2 = '\x2c',
    ASTORE_1 = '\x4c',
    ASTORE_2 = '\x4d',
    NEW = '\xbb',
    INVOKE_VIRTUAL = '\xb6',
    GET_STATIC = '\xb2',
    GOTO = '\xa7',
    RETURN = '\xb1',
    LDC = '\x12',
    DUP = '\x59',
    IADD = '\x60',
    ISUB = '\x64',
    IMULT = '\x68',
    IDIV = '\x6c',
    IREM = '\x70',
    INSTANCE_OF = '\xc1',
    ATHROW = '\xbf',
    IFEQ = '\x99',
    IFNE = '\x9a'
  };
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
