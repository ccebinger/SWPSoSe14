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
      /**
      * Represents the current state of the Bytecode object.
      */
      struct Current_state {
        /**
        * The pointer of the current Bytecode object.
        */
        Bytecode* current_code;

        /**
        * The current node which will be processed.
        */
        Graphs::Node_ptr current_node;
      };

      /**
      * The typedef for the functor which will be used to map the functions to each Command::Type.
      */
      typedef void (*func_ptr) (Bytecode::Current_state state);

      /**
      * The typedef for the std::map to map the functors and commands.
      */
      typedef const std::map<Command::Type, func_ptr> CODE_FUNC_MAPPING;

      /**
      * The typedef for the std::vector which should contain the Java Bytecode.
      */
      typedef std::vector<unsigned char> Code;
    //UTIL
      /**
      * The ctor to create a Bytecode class object.
      * @param pool     the constantpool which will be used to reference to the method and classes in the bytecode
      */
      Bytecode(ConstantPool& pool);

      /**
      * The destructor to clean up the Bytecode class object.
      */
      virtual ~Bytecode();

      /**
      * The build method to build/create the Java Bytecode for the corresponding Graph (Rail-function).
      * @param graph    the Graph_ptr which points on the Graph for which the bytecode will be created
      * @return         the Bytecode class object
      */
      Bytecode* build(Graphs::Graph_ptr graph);
    //GETTER
      /**
      * Returns the length of the bytecode.
      * @return         the length of the bytecode
      */
      size_t length();

      /**
      * Returns the local variable count of the current bytecode.
      * @return         the current local variable count
      */
      int get_local_count();

      /**
      * Returns the constantpool which is used to create the Bytecode.
      * @return         the constantpool object.
      */
      ConstantPool& get_constant_pool();

      /**
      * Returns the created bytecode as std::vector<unsinged char> (Code typedef).
      * @return         the created bytecode
      */
      Code& get_bytecode();

      /**
      * Returns the LocalVariableStash class object which contains the local variables of the current bytecode.
      * @return         the LocalVariableStash class object
      */
      LocalVariableStash& get_locals();
    //SETTER
      /**
      * Increments the current local variable count with the given value.
      * @param          the value which will be used to increment the local variable count
      */
      void inc_local_count(int inc);

      /**
      * Replaces the current local variable count with the given value.
      * @param          the value which will be used to replace the current local variable count
      */
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


    /**
     * The method compares two ints. The way of comparing is given dynamically via
     * the comparator parameter. It is needed to implement the greater method.
     * More utility may follow.
     *
     * IF_ICMPEQ     a != b
     * IF_ICMPGE      a < b
     * IF_ICMPGT     a <= b
     * IF_ICMPLE      a > b
     * IF_ICMPLZ     a >= b
     * IF_ICMPNE     a == b
     *
     * @param comparator        The comparator used when comparing two ints
     * @return Bytecode*        The bytecode generated by the method
     */
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
 /**
  * The method cuts a string at a given index.
  * It works with Java's substring() method.
  * It is the bytecode implementation of the rail command 'c'.
  *
  * @param state        The current state of the bytecode stream.
  */
  void cut_ByteCode(Bytecode::Current_state state);

  /**
  * The method appends a string with another one.
  * It works with Java's append() method in class StringBuilder.
  * It is the bytecode implementation of the rail command 'p'.
  *
  * @param state        The current state of the bytecode stream.
  */
  void append_ByteCode(Bytecode::Current_state state);

  /**
  * The method determines the size of a given string
  * It works with Java's length() method of the class java.lang.String.
  * It is the bytecode implementation of the rail command 'z'.
  *
  * @param state        The current state of the bytecode stream.
  */
  void size_ByteCode(Bytecode::Current_state state);
  //CALL
  void call_ByteCode(Bytecode::Current_state state);
  // LIST OPERATIONS
  void null_ByteCode(Bytecode::Current_state state);
  void list_push_ByteCode(Bytecode::Current_state state);
  void list_pop_ByteCode(Bytecode::Current_state state);
  //BOOLEAN ARITHMETIC

  /**
   * The method push a 0 on the stack to indicate a false.
   * It is the bytecode implementation of the rail command 'f'.
   *
   * @param state        The current state of the bytecode stream.
   */
  void false_ByteCode(Bytecode::Current_state state);

  /**
   * The method determines whether the first value is greater than the
   * second value and pushes a 1 for true or a 0 for false, respectively.
   * It uses the method @see add_two_int_compare to compare two ints.
   * It is the bytecode implementation of the rail command 'g'.
   *
   * @param state        The current state of the bytecode stream.
   */
  void greater_ByteCode(Bytecode::Current_state state);


  /**
   * The method determines whether the first value and the second value
   * are equal or not and pushes a 1 for true or a 0 for false, respectively.
   * It works with references and Java's equals() method.
   * It is the bytecode implementation of the rail command 'q'.
   *
   * @param state        The current state of the bytecode stream.
   */
  void equal_ByteCode(Bytecode::Current_state state);


  /**
   * The method push a 1 on the stack to indicate a true.
   * It is the bytecode implementation of the rail command 't'.
   *
   * @param state        The current state of the bytecode stream.
   */
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
