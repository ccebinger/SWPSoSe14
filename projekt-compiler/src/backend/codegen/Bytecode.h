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
    ALOAD_3 = '\x2d',
    ASTORE = '\x3a',
    ASTORE_0 = '\x4b',
    ASTORE_1 = '\x4c',
    ASTORE_2 = '\x4d',
    ASTORE_3 = '\x4e',
    BIPUSH = '\x10',
    NEW = '\xbb',
    INVOKE_VIRTUAL = '\xb6',
    INVOKE_STATIC = '\xb8',
    INVOKE_SPECIAL = '\xb7',
    INVOKE_INTERFACE = '\xb9',
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
class Bytecode {
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

  /**
   * The build method to build/create the Java Bytecode for the corresponding Graph (Rail-function).
   * This function get called recursivly to generate the bytecode inside the junction
   * @param current_node the Node_ptr which points on the Graph for which the bytecode will be created
   * @return             the Bytecode class object
   */
  Bytecode* build(Graphs::Node_ptr current_node,
                  std::vector<Graphs::Node_ptr> *_proccessedList);

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


  /**
  * Returns the index of the Lambda closure interface method in the constant pool.
  * @return          the closure method index
  */
  uint16_t get_lambda_closure_idx();
  //SETTER
  /*
   *
   * Increments the current local variable count with the given value.
   * @param inc        the value which will be used to increment the local variable count
   *
  void inc_local_count(int inc);

   *
   * Replaces the current local variable count with the given value.
   * @param count        the value which will be used to replace the current local variable count
   *
  void set_local_count(int count);
  */
  //INDECIES
  /**
   * Checks the constant pool for the given class name and returns the idx on the constantpool if the
   * class exists. If not the class name is added and the new idx is returned.
   *
   * @param  class_name        the class name
   * @return         the idx of the class in the constantpool
   */
  uint16_t get_class_idx(const std::string& class_name);

  /**
   * Checks the constant pool for the given name and type and returns the idx on the constantpool if the
   * name and type exists. If not the name and type is added and the new idx is returned.
   *
   * @param name     the name
   * @param type     the type
   * @return         the idx of the name and type in the constantpool
   */
  uint16_t get_name_type_idx(const std::string& name, const std::string& type);


  /**
   * Checks the constant pool for the given method and returns the idx on the constantpool if the
   * method exists. If not the method is added and the new idx is returned.
   * The method consists of the class_name and a name and type which is the method name and the method descriptor.
   *
   * @param class_name     the name of the class on which the method belongs to
   * @param member_name    the name of the method
   * @param descriptor     the method descriptor like ()V for return void and void param
   * @return               the idx of the method in the constantpool
   */
  uint16_t get_method_idx(const std::string& class_name, const std::string& member_name, const std::string& descriptor);

  /**
   * Checks the constant pool for the given field and returns the idx on the constantpool if the field exists.
   * If not the field is added and the new idx is returned.
   * The field consists of the class_name, member name (in the class) and a descriptor (contains the type).
   *
   * @param class_name         the name of the class on which the field belongs to
   * @param member_name        the name of the field in the class
   * @param descriptor         the type of the field
   * @return                   the idx of the field in the constantpool
   */
  uint16_t get_field_idx(const std::string& class_name, const std::string& member_name, const std::string& descriptor);

  /**
   * Checks the constant pool for the given method of the stack field (from the own classfile ArrayDeque field)
   * and returns the idx on the constantpool if the method exists.
   * If not the method is added and the new idx is returned.
   *
   * @param method         the name of the method
   * @param descriptor     the method descriptor like ()V for return void and void param
   * @return               the idx of the method in the constantpool
   */
  uint16_t get_stack_method_idx(const std::string& method, const std::string& descriptor);

  /**
   * Checks the constant pool for the stack field (from the own classfile ArrayDeque field)
   * and returns the idx on the constantpool if the field exists.
   * If not the field is added and the new idx is returned.
   *
   * @return               the idx of the field in the constantpool
   */
  uint16_t get_stack_field_idx();
  //ADD CODE
  /**
   * Adds given conditional and instructions to the current bytecode.
   * The instructions after the conditional instruction is given as std::vector.
   *
   * @param conditional_stmt       the conditional statement which should be added
   * @param conditional_body       the instructions which should be also added after the cond. stmt.
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_conditional_with_instruction(unsigned char conditional_stmt, std::vector<unsigned char> conditional_body);

  /**
   * Adds given conditional and instructions to the current bytecode.
   * The instructions after the conditional instruction is given as std::vector.
   *
   * @param conditional_stmt       the conditional statement which should be added
   * @param conditional_body       the instructions which should be also added after the cond. stmt.
   * @param code                   the vector which should get the bytecode
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_conditional_with_instruction(unsigned char conditional_stmt, std::vector<unsigned char> conditional_body,
                                             std::vector<unsigned char>& code);

  /**
   * Adds given conditional, some instructions and also an else branch with instructions to the current bytecode.
   * The instructions after the conditional instruction is given as std::vector, also the else body.
   *
   * @param conditional_stmt       the conditional statement which should be added
   * @param conditional_body       the instructions which should be also added after the cond. stmt.
   * @param else_body              the instructions for the else body which should be added to the bytecode
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_conditional_with_else_branch(unsigned char conditional_stmt, std::vector<unsigned char> conditional_body, std::vector<unsigned char> else_body);

  /**
   * Adds an u8 index from the constantpool to the bytecode.
   *
   * The index will be cast to big endian via:
   *<pre>
   * (idx & 0xFF)
   *</pre>
   *
   * @param indexInPool            the index in the constantpool
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_index(uint8_t indexInPool);

  /**
   * Adds an u16 index from the constantpool to the bytecode.
   * The index will be cast to big endian via:
   *<pre>
   * (idx & 0xFF00U) >> 8
   * (idx & 0xFF00U)
   *</pre>
   *
   * @param indexInPool            the index in the constantpool
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_index(uint16_t indexInPool);

  /**
   * Adds an u16 index from the constantpool to the given code vector.
   * The index will be cast to big endian via:
   *<pre>
   * (idx & 0xFF00U) >> 8
   * (idx & 0xFF00U)
   *</pre>
   *
   * @param indexInPool            the index in the constantpool
   * @param code                   the code vector on which the index should be pushed
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_index(uint16_t indexInPool, std::vector<unsigned char>& code);

  /**
   * Adds a given opcode with an index to the bytecode.
   *
   * @param opcode                 the Java opcode should be a member of the MNEMONIC enum
   * @param idx                    the index which should be added to the bytecode after the opcode
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_opcode_with_idx(MNEMONIC opcode, uint16_t idx);


  /**
   * Adds a given opcode with an index to the given code vector.
   *
   * @param opcode                 the Java opcode should be a member of the MNEMONIC enum
   * @param idx                    the index which should be added to the code vector after the opcode
   * @param code                   the vector which should get the bytecode
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_opcode_with_idx(codegen::MNEMONIC opcode, uint16_t idx, std::vector<unsigned char>& code);


  /**
   * Adds a given opcode to the bytecode.
   *
   * @param opcode                 the Java opcode should be a member of the MNEMONIC enum
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_opcode(MNEMONIC opcode);


  /**
   * Adds a given byte value to the bytecode
   *
   * @param byte                   the byte which should be added to the bytecode
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_byte(uint8_t byte);

  /**
   * Adds a static field method call to the bytecode.
   * Like invoke_virtual push from ArrayDeque.
   *
   * @param field_idx              the index of the field in the constantpool
   * @param method_idx             the index of the method in the constantpool
   * @return                       the pointer of the current Bytecode class object
   */
  Bytecode* add_static_field_method_call(uint16_t field_idx, uint16_t method_idx);

  /**
   * Adds for the given calculation opcode like IADD, IDIV, IREM etc. the
   * correct bytecode with type checking.
   *
   * @param  calculation     the opcode for the calculation
   * @return                 the pointer of the current Bytecode class object
   */
  Bytecode* add_integer_calculation(MNEMONIC calculation);

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
   * @param comparator        the comparator used when comparing two ints
   * @return Bytecode*        the pointer of the current Bytecode class object
   */
  Bytecode* add_two_int_compare(MNEMONIC comparator);

  /**
   * Adds bytecode to load a given constant within a method.
   * Do not misunderstand with loading/push a constant in rail.
   *
   * @param  constant        the constant you want to load
   * @return                 the pointer of the current Bytecode class object
   */
  Bytecode* add_ldc_string(const std::string& constant);

  /**
   * Adds bytecode into a char vector to load a given constant within a method.
   * Do not misunderstand with loading/push a constant in rail.
   *
   * @param  constant        the constant you want to load
   * @param  code            the vector which should get the bytecode
   * @return                 the pointer of the current Bytecode class object
   */
  Bytecode* add_ldc_string(const std::string& constant, std::vector<unsigned char>& code);

  /**
  * Adds bytecode for the lambda declaration, means creates an Object
  * which implements the Lambda interface.
  * The Classname is given via the Node.arg. The object will be pushed into the ArrayDeque (globalstack).
  * Adds all necessary entries into the constantpool before the code is generated.
  * Set the lambda_closure_idx field to the corresponding constantpool index.
  *
  * @param current_node       the current node
  * @return                   the pointer of the current Bytecode class object
  */
  Bytecode* add_lambda_declaration(Graphs::Node_ptr current_node);
  //GLOBAL STACK
  /**
   * Adds bytecode which pops a value from the static ArrayDeque field.
   * (Field is named stack)
   *
   * @return        The bytecode generated by the method
   */
  Bytecode* globalstack_pop();

  /**
   * Adds bytecode which pushes a value into the static ArrayDeque field.
   * (Field is named stack)
   *
   * @return       The bytecode generated by the method
   */
  Bytecode* globalstack_push();

  /**
   * list of processed node needed for loop detection.
   */
  std::vector<Graphs::Node_ptr> proccessedList;

 private:
  /**
   * The bytecode vector which contains the created Java Bytecode.
   */
  Code bytecode;

  /**
   * The constantpool which is used for the bytecode creation.
   */
  ConstantPool& pool;

  /*
   * The local variable count.
   *
  int local_count;
  */

  /**
   * The LocalVariableStash class object which contains the mapping of the local variables.
   */
  LocalVariableStash locals;

  /**
   * Functor map which maps the command types to each corresponding function.
   */
  static CODE_FUNC_MAPPING func_map;

 /**
  * The index of the lambda closure interface method in the constant pool.
  * If the idx is 0 then no lambda object was declared before or the declaration was in another method.
  */
  uint16_t lambda_closure_idx = 0;
};

/**
 * The method generates the Java Bytecode to output an object.
 * The generated bytecode is the representation of the 'o' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void output_ByteCode(Bytecode::Current_state state);


/**
 * The method generates the Java Bytecode to push an object into the static ArrayDeque field (also named global Stack).
 * It creates also for the corresponding value an entry on the constant pool.
 * The generated bytecode is the representation of the '\[[A-Za-z0-9]\][0-9]' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void push_ByteCode(Bytecode::Current_state state);
//ARITMETIC OPERATIONS
/**
 * The method generates the Java Bytecode to add two numbers.
 * The values are pop from the global stack and the result is pushed into the stack.
 * The generated bytecode is the representation of the 'a' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void add_ByteCode(Bytecode::Current_state state);

/**
 * The method generates the Java Bytecode to subtract two numbers.
 * The values are pop from the global stack and the result is pushed into the stack.
 * The generated bytecode is the representation of the 's' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void sub_ByteCode(Bytecode::Current_state state);

/**
 * The method generates the Java Bytecode to multiplicate two numbers.
 * The values are pop from the global stack and the result is pushed into the stack.
 * The generated bytecode is the representation of the 'm' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void mult_ByteCode(Bytecode::Current_state state);


/**
 * The method generates the Java Bytecode to divide two numbers.
 * The values are pop from the global stack and the result is pushed into the stack.
 * The generated bytecode is the representation of the 'd' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void div_ByteCode(Bytecode::Current_state state);

/**
 * The method generates the Java Bytecode to calculate the remainder of two numbers.
 * The values are pop from the global stack and the result is pushed into the stack.
 * The generated bytecode is the representation of the 'r' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
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

/**
 * The method generates the Java Bytecode to call a Rail-function (Method in the Java class)
 * The function/method must be a valid name of an existing ASG.
 * The generated bytecode is the representation of the '\{[A-Za-z0-9]\}' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void call_ByteCode(Bytecode::Current_state state);
// LIST OPERATIONS

/**
 * The method generates the Java Bytecode to create a new List and push them into the globalstack.
 * The generated bytecode is the representation of the 'n' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void null_ByteCode(Bytecode::Current_state state);

/**
 * The method generates the Java Bytecode to push an object into a list.
 * If no list already exists a new one will be created and the current and the value before are pushed into
 * the list (chained together).
 * The generated bytecode is the representation of the ':' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void list_push_ByteCode(Bytecode::Current_state state);

/**
 * The method generates the Java Bytecode to pop an object from an existing list.
 * The generated bytecode is the representation of the '~' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
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
/**
 * The method generates the Java Bytecode to throw an Exception which simulates the boom behaivour of Rail.
 * The generated bytecode is the representation of the 'b' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void boom_ByteCode(Bytecode::Current_state state);


/**
 * The method generates the Java Bytecode to check if the input is empty.
 * The generated bytecode is the representation of the 'e' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void eof_ByteCode(Bytecode::Current_state state);


/**
 * The method generates the Java Bytecode to read an input.
 * The generated bytecode is the representation of the 'i' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void input_ByteCode(Bytecode::Current_state state);


/**
 * The method generates the Java Bytecode to get the size of the globalstack, the size are pushed in the stack.
 * The generated bytecode is the representation of the 'u' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void underflow_ByteCode(Bytecode::Current_state state);


/**
 * The method generates the Java Bytecode to get type information of the top of the stack object.
 * The generated bytecode is the representation of the '?' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void type_ByteCode(Bytecode::Current_state state);
//CONTROL STRUCTURE

/**
 * The method generates the Java Bytecode for while and if statements which are represents the junctions in Rail.
 * The generated bytecode is the representation of the '< | x | > | Y | ^ | * | + ' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void if_or_while_ByteCode(Bytecode::Current_state state);
//VARIABLES

/**
 * The method generates the Java Bytecode to pop a Variable.
 * <b>That means the value of the Variable are pushed into the globalstack.</b>
 * The generated bytecode is the representation of the '\([A-Za-z0-9]\)' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void pop_Variable(Bytecode::Current_state state);


/**
 * The method generates the Java Bytecode to push/create a Variable.
 * The generated bytecode is the representation of the ''\(![A-Za-z0-9]!\)'' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void push_Variable(Bytecode::Current_state state);

//LAMBDA
/**
 * The method generates the Java Bytecode to declare a new Lambda Object.
 * The generated bytecode is the representation of the '&' Rail-command.
 *
 * @param state        the current state of the Bytecode class object
 */
void lambda_Bytecode(Bytecode::Current_state state);

}  // namespace codegen
#endif // BYTECODE_H
