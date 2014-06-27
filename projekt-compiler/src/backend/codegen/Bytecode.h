#ifndef BYTECODE_H
#define BYTECODE_H

#include <vector>
#include <map>

#include <backend/classfile/constant_pool.h>
#include <backend/codegen/local_variable_stash.h>
#include <frontend/Graphs.h>
#include <common/Env.h>

namespace codegen {

  enum MNEMONIC {
    I2C = '\x92',
    ILOAD_0 = '\x1a',
    ILOAD_1 = '\x1b',
    ILOAD_2 = '\x1c',
    ICONST_0 = '\x03',
    ICONST_1 = '\x04',
    ISTORE_0 = '\x3b',
    ISTORE_1 = '\x3c',
    ISTORE_2 = '\x3d',
    IF_ICMPGT = '\xa3',
    IF_ICMPLE = '\xa4',
    IF_ICMPLT = '\xa1',
    IF_ICMPNE = '\xa0',
    IF_ICMPGE = '\xa2',
    ALOAD = '\x19',
    ALOAD_0 = '\x2a',
    ALOAD_1 = '\x2b',
    ALOAD_2 = '\x2c',
    ASTORE = '\x3a',
    ASTORE_0 = '\x4b',
    ASTORE_1 = '\x4c',
    ASTORE_2 = '\x4d',
    BIPUSH = '\x10',
    NEW = '\xbb',
    INVOKE_VIRTUAL = '\xb6',
    INVOKE_STATIC = '\xb8',
    INVOKE_SPECIAL = '\xb7',
    GET_STATIC = '\xb2',
    GOTO = '\xa7',
    RETURN = '\xb1',
    SWAP = '\x5f',
    LDC = '\x12',
    DUP = '\x59',
    IADD = '\x60',
    ISUB = '\x64',
    IMULT = '\x68',
    IDIV = '\x6c',
    IREM = '\x70',
    INSTANCE_OF = '\xc1',
    CHECKCAST = '\xc0',
    ATHROW = '\xbf',
    IFEQ = '\x99',
    IFNE = '\x9a',
    POP = '\x57'
  };


  class Bytecode
  {
    public:
    //TYPDEFS
      struct Current_state {
        Bytecode* current_code;
        Graphs::Node_ptr current_node;
      };
      typedef void (*func_ptr) (Bytecode::Current_state state);
      typedef const std::map<Command::Type, func_ptr> CODE_FUNC_MAPPING;
      typedef std::vector<unsigned char> Code;
    //UTIL
      Bytecode(ConstantPool& pool);
      virtual ~Bytecode();
      Bytecode* build(Graphs::Graph_ptr graph);
    //GETTER
      size_t length();
      int get_local_count();
      ConstantPool& get_constant_pool();
      Code& get_bytecode();
      LocalVariableStash& get_locals();
    //SETTER
      void inc_local_count(int inc);
      void set_local_count(int count);
    //INDECIES
      uint16_t get_class_idx(const std::string& class_name);
      uint16_t get_name_type_idx(const std::string& name, const std::string& type);
      uint16_t get_method_idx(const std::string& class_name, const std::string& member_name, const std::string& descriptor);
      uint16_t get_field_idx(const std::string& class_name, const std::string& member_name, const std::string& descriptor);
      uint16_t get_stack_method_idx(const std::string& method, const std::string& descriptor);
      uint16_t get_stack_field_idx();
    //ADD CODE
      Bytecode* add_conditional_with_instruction(unsigned char conditional_stmt, std::vector<unsigned char> conditional_body);
      Bytecode* add_conditional_with_else_branch(unsigned char conditional_stmt, std::vector<unsigned char> conditional_body, std::vector<unsigned char> else_body);
      Bytecode* add_index(uint8_t indexInPool);
      Bytecode* add_index(uint16_t indexInPool);
      Bytecode* add_index(uint16_t indexInPool, std::vector<unsigned char>& code);
      Bytecode* add_opcode_with_idx(MNEMONIC opcode, uint16_t idx);
      Bytecode* add_opcode_with_idx(codegen::MNEMONIC opcode, uint16_t idx, std::vector<unsigned char>& code);
      Bytecode* add_opcode(MNEMONIC opcode);
      Bytecode* add_byte(uint8_t byte);
      Bytecode* add_static_field_method_call(uint16_t field_idx, uint16_t method_idx);
      Bytecode* add_integer_calculation(MNEMONIC calculation);
      Bytecode* add_type_check(uint16_t class_idx);
      Bytecode* add_two_int_compare(MNEMONIC comparator);
    //GLOBAL STACK
      Bytecode* globalstack_pop();
      Bytecode* globalstack_push();

    private:
      Code bytecode;
      ConstantPool& pool;
      int local_count;
      LocalVariableStash locals;

      static CODE_FUNC_MAPPING func_map;
  };

  void output_ByteCode(Bytecode::Current_state state);
  void push_ByteCode(Bytecode::Current_state state);
  //ARITMETIC OPERATIONS
  void add_ByteCode(Bytecode::Current_state state);
  void sub_ByteCode(Bytecode::Current_state state);
  void mult_ByteCode(Bytecode::Current_state state);
  void div_ByteCode(Bytecode::Current_state state);
  void mod_ByteCode(Bytecode::Current_state state);
  //STRING OPERATIONS
  void cut_ByteCode(Bytecode::Current_state state);
  void append_ByteCode(Bytecode::Current_state state);
  void size_ByteCode(Bytecode::Current_state state);
  //CALL
  void call_ByteCode(Bytecode::Current_state state);
  // LIST OPERATIONS
  void null_ByteCode(Bytecode::Current_state state);
  void list_push_ByteCode(Bytecode::Current_state state);
  void list_pop_ByteCode(Bytecode::Current_state state);
  //BOOLEAN ARITHMETIC
  void false_ByteCode(Bytecode::Current_state state);
  void greater_ByteCode(Bytecode::Current_state state);
  void equal_ByteCode(Bytecode::Current_state state);
  void true_ByteCode(Bytecode::Current_state state);
  //IO OPERATIONS
  void boom_ByteCode(Bytecode::Current_state state);
  void eof_ByteCode(Bytecode::Current_state state);
  void input_ByteCode(Bytecode::Current_state state);
  void underflow_ByteCode(Bytecode::Current_state state);
  void type_ByteCode(Bytecode::Current_state state);
  //CONTROL STRUCTURE
  void if_or_while_ByteCode(Bytecode::Current_state state);
  //VARIABLES
  void pop_Variable(Bytecode::Current_state state);
  void push_Variable(Bytecode::Current_state state);
}
#endif // BYTECODE_H
