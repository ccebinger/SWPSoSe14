#ifndef BYTECODE_H
#define BYTECODE_H

#include <vector>
#include <map>

#include <backend/classfile/constant_pool.h>
#include <backend/codegen/local_variable_stash.h>
#include <frontend/Graphs.h>
#include <common/Env.h>

/**
 * The codegen namespace contains everything to generate Java Bytecode.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>, Sascha Kretzschmann, Vincent Bohlen, Jonas Dettlaff
 */
namespace codegen {

  /**
  * The enum represents the MNEMONIC Java opcodes.
  * Each enum member has his own hex representation.
  * See for more information http://en.wikipedia.org/wiki/Java_bytecode_instruction_listings.
  *
  */
  enum MNEMONIC {
    I2C = '\x92',
    ILOAD_0 = '\x1a',
    ILOAD_1 = '\x1b',
    ILOAD_2 = '\x1c',
    ILOAD_3 = '\x1d',
    ICONST_0 = '\x03',
    ICONST_1 = '\x04',
    ISTORE_0 = '\x3b',
    ISTORE_1 = '\x3c',
    ISTORE_2 = '\x3d',
    ISTORE_3 = '\x3e',
    IF_ACMPNE = '\xa6',
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


  /**
  * The Bytecode class represents the Java Bytecode which should be generated from the Rail-Compiler.
  * It contains a std::map to map each Rail-Command to a function which generates different Java Bytecode.
  * The generate bytecode is saved in a std::vector and can be return by calling get_bytecode.
  * To generate and reference to the constantpool in the classfile a Constantpool must be given by creating a Bytecode class object.
  * After that the Java bytecode can be created by calling the build method. This method expects a Graph_ptr which points
  * to a Graph class object. The Graph represents a function in Rail, is also named ASG in the compiler.
  * The Bytecode provides some useful methods to add bytecode to the current bytecode vector.
  * Each of these methods returns a pointer of the current Bytecode class object, which makes it possible to
  * chain the method calls. (syntactic sugar see example below)
  * <p>Example chaining</p>
  * <pre><code class="c++">
  *   code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
  *       ->globalstack_pop()
  *       ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
  *       ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.str_idx.length_idx)
  *       ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx)
  *       ->globalstack_push();
  * </code></pre>
  *
  * <p>To create Java Bytecode see the following Example:</p>
  * <pre><code class="c++">
  * codegen::Bytecode* code = new codegen::Bytecode(ConstantPool);
  * code->build(Graph);
  * ...//some code
  * //clean up
  * delete code;
  * </code></pre>
  * <p> The build method creates the Java Bytecode for each node of the Graph step by step.
  * That means for each node the map will be checked and for the corresponding command type the function will be called.
  * For example for the Command::Type::PUSH the push_Bytecode function will be called.
  * <b>That makes it easy to override these functions and change the created bytecode later.</b>
  * Each function have the same appearance and should look like void (*func_ptr) (Bytecode::Current_state state).
  * The parameter state is a struct Current_state which contains the current node of the graph and also the current state
  * of the Bytecode. That means the struct contains a pointer of the current Bytecode class object.
  *
  * @see Graph, Graphs, Constantpool
  * @author Christopher Zell <Zelldon91@googlemail.com>
  *
  */
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
