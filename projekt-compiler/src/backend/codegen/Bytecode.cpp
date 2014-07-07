#include <backend/codegen/Bytecode.h>
#include <backend/classfile/classfile_writer.h>


codegen::Bytecode::CODE_FUNC_MAPPING
codegen::Bytecode::func_map= {
  {Command::Type::OUTPUT, &output_ByteCode},
  {Command::Type::PUSH_CONST, &push_ByteCode},
  {Command::Type::ADD, &add_ByteCode},
  {Command::Type::SUB, &sub_ByteCode},
  {Command::Type::MULT, &mult_ByteCode},
  {Command::Type::DIV, &div_ByteCode},
  {Command::Type::MOD, &mod_ByteCode},
  {Command::Type::CUT, &cut_ByteCode},
  {Command::Type::APPEND, &append_ByteCode},
  {Command::Type::SIZE, &size_ByteCode},
  {Command::Type::CALL , &call_ByteCode},
  {Command::Type::NIL, &null_ByteCode},
  {Command::Type::LIST_CONS, &list_push_ByteCode},
  {Command::Type::LIST_BREAKUP, &list_pop_ByteCode},
  {Command::Type::FALSE, &false_ByteCode},
  {Command::Type::GREATER, &greater_ByteCode},
  {Command::Type::EQUAL, &equal_ByteCode},
  {Command::Type::TRUE, &true_ByteCode},
  //Command::Type::REFLECTOR
  //Command::Type::START
  //Command::Type::FINISH
  //Command::Type::LAMBDA
  {Command::Type::BOOM, &boom_ByteCode},
  {Command::Type::EOF_CHECK, &eof_ByteCode},
  {Command::Type::INPUT, &input_ByteCode},
  {Command::Type::UNDERFLOW_CHECK, &underflow_ByteCode},
  {Command::Type::TYPE_CHECK, &type_ByteCode},
  {Command::Type::EASTJUNC, &if_or_while_ByteCode},
  {Command::Type::WESTJUNC, &if_or_while_ByteCode},
  {Command::Type::NORTHJUNC, &if_or_while_ByteCode},
  {Command::Type::SOUTHJUNC, &if_or_while_ByteCode},
  {Command::Type::VAR_POP, &pop_Variable},
  {Command::Type::VAR_PUSH, &push_Variable}
};

codegen::Bytecode::Bytecode(ConstantPool& p) : pool(p), local_count(1), locals(4) {}

codegen::Bytecode::~Bytecode() {}


codegen::Bytecode* codegen::Bytecode::build(Graphs::Graph_ptr graph) {
  Graphs::Node_ptr current_node(graph->start());
  while (current_node && current_node->command.type != Command::Type::FINISH) {
    if (current_node->command.type == Command::Type::LAMBDA)
      add_lambda_call(graph, current_node);
    else {
      func_ptr f = func_map.at(current_node->command.type);
      if (f) {
        Current_state state;
        state.current_code = this;
        state.current_node = current_node;
        f(state);
      }
    }
    current_node = current_node->successor1;
  }
  bytecode.push_back(codegen::MNEMONIC::RETURN);
  return this;
}

codegen::Bytecode* codegen::Bytecode::build(Graphs::Node_ptr current_node) {
  while (current_node && current_node->command.type != Command::Type::FINISH) {
    func_ptr f = func_map.at(current_node->command.type);
    if (f) {
      Current_state state;
      state.current_code = this;
      state.current_node = current_node;
      f(state);
    }
    current_node = current_node->successor1;
  }
  bytecode.push_back(codegen::MNEMONIC::RETURN);
  return this;
}

//================================================================================
//==================================GETTER========================================
//================================================================================

size_t codegen::Bytecode::length() {
  return bytecode.size();
}

int codegen::Bytecode::get_local_count() {
  return local_count + locals.current_var_count();
}

ConstantPool& codegen::Bytecode::get_constant_pool() {
  return pool;
}

codegen::Bytecode::Code& codegen::Bytecode::get_bytecode() {
  return bytecode;
}

LocalVariableStash& codegen::Bytecode::get_locals() {
  return locals;
}

//================================================================================
//==================================SETTER========================================
//================================================================================
void codegen::Bytecode::inc_local_count(int inc)
{
  local_count += inc;
}

void codegen::Bytecode::set_local_count(int count)
{
  local_count = count;
}

//================================================================================
//==================================INDICIES======================================
//================================================================================
uint16_t codegen::Bytecode::get_class_idx(const std::string& class_name) {
  return pool.addClassRef(pool.addString(class_name));
}

uint16_t codegen::Bytecode::get_name_type_idx(const std::string& name,
                                              const std::string& type) {
  uint16_t name_idx = pool.addString(name);
  uint16_t type_idx = pool.addString(type);
  return pool.addNameAndType(name_idx, type_idx);
}

uint16_t codegen::Bytecode::get_method_idx(const std::string& class_name,
                                           const std::string& member_name,
                                           const std::string& descriptor) {
  return pool.addMethRef(get_class_idx(class_name),
                         get_name_type_idx(member_name, descriptor));
}

uint16_t codegen::Bytecode::get_field_idx(const std::string& class_name,
                                          const std::string& member_name,
                                          const std::string& descriptor) {
  return pool.addFieldRef(get_class_idx(class_name),
                          get_name_type_idx(member_name, descriptor));
}

uint16_t  codegen::Bytecode::get_stack_method_idx(const std::string& method,
                                                  const std::string& descriptor) {
  return get_method_idx("java/util/ArrayDeque", method, descriptor);
}

uint16_t codegen::Bytecode::get_stack_field_idx() {
  return get_field_idx(Env::getDstClassName(), "stack",
                       "Ljava/util/ArrayDeque;");
}

//================================================================================
//==================================CODE ADD======================================
//================================================================================
codegen::Bytecode* codegen::Bytecode::add_index(uint8_t indexInPool) {
  bytecode.push_back(indexInPool & 0xFF);
  return this;
}
codegen::Bytecode* codegen::Bytecode::add_index(uint16_t indexInPool) {
  return add_index(indexInPool, bytecode);
}

codegen::Bytecode* codegen::Bytecode::add_index(uint16_t indexInPool,
                                                std::vector<unsigned char>& code) {
  code.push_back((indexInPool & 0xFF00U) >> 8);
  code.push_back(indexInPool & 0x00FFU);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_conditional_with_instruction(unsigned char conditional_stmt,
                                                                       std::vector<unsigned char> conditional_body) {
  int length = conditional_body.size();
  uint16_t length_plus_branch = length + 2;
  bytecode.push_back(conditional_stmt);
  add_index(length_plus_branch);
  bytecode.insert(bytecode.end(), conditional_body.begin(), conditional_body.end());
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_conditional_with_instruction(unsigned char conditional_stmt,
                                                                       std::vector<unsigned char> conditional_body,
                                                                       std::vector<unsigned char>& code) {
  int length = conditional_body.size();
  uint16_t length_plus_branch = length + 2;
  code.push_back(conditional_stmt);
  add_index(length_plus_branch, code);
  code.insert(code.end(), conditional_body.begin(), conditional_body.end());
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_conditional_with_else_branch(unsigned char conditional_stmt,
                                                                       std::vector<unsigned char> conditional_body,
                                                                       std::vector<unsigned char> else_body) {
  std::vector<unsigned char> if_body;
  if_body.insert(if_body.begin(), conditional_body.begin(),
                 conditional_body.end());

  int else_length = else_body.size();
  uint16_t branch_idx = else_length + 3; //+1 to jump over the last else stmt
  if_body.push_back(codegen::MNEMONIC::GOTO);
  add_index(branch_idx, if_body);
  branch_idx = if_body.size() + 2 + else_length; //+ 2 for branch idx

  bytecode.push_back(conditional_stmt);
  add_index(branch_idx); /// TODO: CHECK IF BRANCH IS CORRECT should be after goto!!
  bytecode.insert(bytecode.end(), if_body.begin(), if_body.end());
  bytecode.insert(bytecode.end(), else_body.begin(), else_body.end());
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_opcode_with_idx(codegen::MNEMONIC opcode,
                                                          uint16_t idx) {
  return add_opcode_with_idx(opcode, idx, bytecode);
}


codegen::Bytecode* codegen::Bytecode::add_opcode_with_idx(codegen::MNEMONIC opcode,
                                                          uint16_t idx,
                                                          std::vector<unsigned char>& code) {
  code.push_back(opcode);
  add_index(idx, code);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_static_field_method_call(uint16_t field_idx,
                                                                   uint16_t method_idx) {
  add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, field_idx);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, method_idx);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_integer_calculation(MNEMONIC calculation) {
  uint16_t intValue_idx = pool.int_idx.int_value_idx;


  add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx);
  globalstack_pop();
  add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.int_idx.class_idx);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, intValue_idx);
  bytecode.push_back(codegen::MNEMONIC::ISTORE_1);
  globalstack_pop();
  add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.int_idx.class_idx);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, intValue_idx);
  bytecode.push_back(codegen::MNEMONIC::ILOAD_1);
  bytecode.push_back(calculation);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx);
  globalstack_push();
  inc_local_count(2);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_opcode(codegen::MNEMONIC opcode) {
  bytecode.push_back(opcode);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_byte(uint8_t byte) {
  bytecode.push_back(byte);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_two_int_compare(codegen::MNEMONIC comparator){
  uint16_t intValue_idx = pool.int_idx.int_value_idx;

  std::vector<unsigned char> conditional_body;
  std::vector<unsigned char> else_body;

  globalstack_pop();
  add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.int_idx.class_idx);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, intValue_idx);
  add_opcode(codegen::MNEMONIC::ISTORE_1);
  globalstack_pop();
  add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.int_idx.class_idx);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, intValue_idx);
  add_opcode(codegen::MNEMONIC::ISTORE_2);
  add_opcode(codegen::MNEMONIC::ILOAD_1);
  add_opcode(codegen::MNEMONIC::ILOAD_2);

  conditional_body.push_back(codegen::MNEMONIC::ICONST_1);
  else_body.push_back(codegen::MNEMONIC::ICONST_0);

  add_conditional_with_else_branch(comparator, conditional_body, else_body);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_ldc_string(const std::string& constant){
  bytecode.push_back(codegen::MNEMONIC::LDC);
  uint16_t string_idx = pool.addString(constant);
  uint16_t const_idx = pool.addConstString(string_idx);
  bytecode.push_back((uint8_t) const_idx);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_lambda_call(Graphs::Graph_ptr graph, Graphs::Node_ptr current_node) {
  //add Lambda anonymous class to pool
  //$anonymous class
  std::stringstream ss;
  ss << Env::getDstClassName() << "$" << current_node->command.arg;
  std::string anonymous_class_name = ss.str();
  size_t anonym_str_idx = pool.addString(anonymous_class_name);
  size_t anonym_class_idx = pool.addClassRef(anonym_str_idx);

  //$anonymous class <init> method
  size_t init_str_idx = pool.addString("<init>");
  size_t void_descriptor = pool.addString("()V");
  size_t anonym_init_idx = pool.addMethRef(anonym_class_idx, pool.addNameAndType(init_str_idx, void_descriptor));

  //$anonymous class closure method
  size_t closure_str_idx = pool.addString("closure");
  size_t lambda_str_idx = pool.addString("Lambda");
  size_t lambda_cls_idx = pool.addClassRef(lambda_str_idx);
  pool.addNameAndType(closure_str_idx, void_descriptor);
  //interface method!!!
  //CODE
  add_opcode_with_idx(codegen::MNEMONIC::NEW, anonym_class_idx);
  add_opcode(codegen::MNEMONIC::DUP);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_SPECIAL, anonym_init_idx);
  //add interface call method (closure call)
  //invokeinterface Lambda.closure 1 (1 stands for the object which will be used for the call, more than 1 are the arguments which are popped from the stack)

}
//================================================================================
//=================================GLOBAL STACK===================================
//================================================================================


codegen::Bytecode* codegen::Bytecode::globalstack_pop() {
  return add_static_field_method_call(pool.arr_idx.field_idx,
                                      pool.arr_idx.pop_idx);
}

codegen::Bytecode* codegen::Bytecode::globalstack_push() {
  add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx);
  // argument needs to be on top of stack
  add_opcode(codegen::MNEMONIC::SWAP);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.arr_idx.push_idx);
  return this;
}

//================================================================================
//==================================FUNCTORS======================================
//================================================================================

void codegen::output_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();
  // push system.out+
  // get <Field java/lang/System.out:Ljava/io/PrintStream;>

  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.system_idx.out_idx)
      ->globalstack_pop()
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.stream_idx.print_idx)
      ->inc_local_count(1);
}

void codegen::push_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->add_opcode(codegen::MNEMONIC::LDC);
  std::string value = state.current_node->command.extractAstCommandString();
  try {
    std::size_t end_parse;
    int int_val = std::stoi(value, &end_parse);
    if (end_parse != value.size()) {
      throw std::invalid_argument("Parsing int didn't reach end of string");
    }
    code->add_index((uint8_t) pool.addInt(int_val))
        ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx);
  }
  // the value is a string
  catch (const std::invalid_argument& ia) {
    uint16_t string_idx = pool.addString(value);
    uint16_t const_idx = pool.addConstString(string_idx);
    code->add_index((uint8_t) const_idx);
  }
  code->globalstack_push();
}

//ARITMETIC OPERATIONS
void codegen::add_ByteCode(Bytecode::Current_state state) {
  state.current_code->add_integer_calculation(codegen::MNEMONIC::IADD);
}

void codegen::sub_ByteCode(Bytecode::Current_state state) {
  state.current_code->add_integer_calculation(codegen::MNEMONIC::ISUB);
}

void codegen::mult_ByteCode(Bytecode::Current_state state) {
  state.current_code->add_integer_calculation(codegen::MNEMONIC::IMULT);
}

void codegen::div_ByteCode(Bytecode::Current_state state) {
  state.current_code->add_integer_calculation(codegen::MNEMONIC::IDIV);
}

void codegen::mod_ByteCode(Bytecode::Current_state state) {
  state.current_code->add_integer_calculation(codegen::MNEMONIC::IREM);
}

//STRING OPERATIONS
void codegen::cut_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->globalstack_pop()
      ->add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.int_idx.class_idx)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.int_idx.int_value_idx)
      ->add_opcode(codegen::MNEMONIC::ISTORE_1)
      ->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_2)
      ->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->add_opcode(codegen::MNEMONIC::ALOAD_2)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
      ->add_opcode(codegen::MNEMONIC::ICONST_0)
      ->add_opcode(codegen::MNEMONIC::ILOAD_1)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.str_idx.substring_2param_idx)
      ->globalstack_push()
      ->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->add_opcode(codegen::MNEMONIC::ALOAD_2)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
      ->add_opcode(codegen::MNEMONIC::ILOAD_1)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.str_idx.substring_idx)
      ->globalstack_push()
      ->inc_local_count(2);
}

void codegen::append_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_1)
      ->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->add_opcode_with_idx(codegen::MNEMONIC::NEW, pool.str_builder_idx.class_idx)
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->globalstack_pop()
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_SPECIAL, pool.str_builder_idx.init_idx)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.str_builder_idx.append_idx)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
      ->globalstack_push()
      ->inc_local_count(2);
}

void codegen::size_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code
      ->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->globalstack_pop()
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.str_idx.length_idx)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx)
      ->globalstack_push();
}

//CALL
void codegen::call_ByteCode(Bytecode::Current_state state) {
	Bytecode* code = state.current_code;
	// ConstantPool& pool = code->get_constant_pool(); // comment out when pool is needed
	std::string value = state.current_node->command.extractAstCommandString();

	code->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, code->get_method_idx("Main", value, "()V"));
}

// LIST OPERATIONS
void codegen::null_ByteCode(Bytecode::Current_state state) {
  codegen::Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->add_opcode_with_idx(codegen::MNEMONIC::NEW, pool.list_idx.class_idx)
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_SPECIAL, pool.list_idx.init_idx)
      ->globalstack_push()
      ->inc_local_count(1);
}

void codegen::list_push_ByteCode(Bytecode::Current_state state) {
  codegen::Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  std::vector<unsigned char> conditional_body;

  // fill conditional branch
  code->add_opcode_with_idx(codegen::MNEMONIC::NEW, pool.list_idx.class_idx, conditional_body);
  conditional_body.push_back(codegen::MNEMONIC::DUP);
  code->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_SPECIAL, pool.list_idx.init_idx, conditional_body);
  conditional_body.push_back(codegen::MNEMONIC::ASTORE_1);

  code->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_1)
      ->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_2)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_opcode_with_idx(codegen::MNEMONIC::INSTANCE_OF, pool.list_idx.class_idx)
      ->add_conditional_with_instruction(codegen::MNEMONIC::IFNE, conditional_body)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.list_idx.class_idx)
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->add_opcode(codegen::MNEMONIC::ALOAD_2)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.list_idx.add_idx)
      ->add_opcode(codegen::MNEMONIC::POP)
      ->globalstack_push()

//  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
//      ->globalstack_pop()
//      ->add_opcode(codegen::MNEMONIC::ASTORE_1) // pop VALUE
//      ->globalstack_pop()
//      ->add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.list_idx.class_idx)
//      ->add_opcode(codegen::MNEMONIC::DUP)
//      ->add_opcode(codegen::MNEMONIC::ALOAD_1) // push value before list
//      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.list_idx.add_idx)
//      ->add_opcode(codegen::MNEMONIC::POP)
//      ->globalstack_push()
      ->inc_local_count(3);
}

void codegen::list_pop_ByteCode(Bytecode::Current_state state) {
  codegen::Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();


  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->globalstack_pop() //LIST
      ->add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.list_idx.class_idx)
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->add_opcode(codegen::MNEMONIC::ICONST_0)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.list_idx.remove_idx)
      ->add_opcode(codegen::MNEMONIC::ASTORE_1) //remove obj
      ->globalstack_push()
      ->add_opcode(codegen::MNEMONIC::ALOAD_1) //get saved obj
      ->globalstack_push()
      ->inc_local_count(1);

}

//BOOLEAN ARITHMETIC
void codegen::false_ByteCode(Bytecode::Current_state state) {
  push_ByteCode(state);
}

void codegen::greater_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->add_two_int_compare(codegen::MNEMONIC::IF_ICMPGE)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx)
      ->globalstack_push()
      ->inc_local_count(3);
}

void codegen::equal_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_1)
      ->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_2)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_opcode(codegen::MNEMONIC::ALOAD_2)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.equals)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx)
      ->globalstack_push()
      ->inc_local_count(3);
}

void codegen::true_ByteCode(Bytecode::Current_state state) {
  push_ByteCode(state);
}

//IO OPERATIONS
void codegen::boom_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;

  uint16_t init_idx = code->get_method_idx("java/lang/RuntimeException", "<init>", "()V");
  code->add_opcode_with_idx(codegen::MNEMONIC::NEW, code->get_class_idx("java/lang/RuntimeException"))
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_SPECIAL, init_idx)
      ->add_opcode(codegen::MNEMONIC::ATHROW);
}

void codegen::eof_ByteCode(Bytecode::Current_state state){
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  std::vector<unsigned char> conditional_body;
  std::vector<unsigned char> else_body;

  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.system_idx.in_idx)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.stream_idx.available_idx);

  conditional_body.push_back(codegen::MNEMONIC::ICONST_1);
  else_body.push_back(codegen::MNEMONIC::ICONST_0);

  code->add_conditional_with_else_branch(codegen::MNEMONIC::IFNE, conditional_body, else_body)
      ->add_opcode_with_idx(MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx)
      ->globalstack_push();
}

void codegen::input_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

   // Push String/Integer depending on input.

   // Read 1 byte of input.
   code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.system_idx.in_idx);
   code->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.stream_idx.read_idx);

   // if !isdigit(input) then goto [push String] else [push Integer]
   code->add_opcode(codegen::MNEMONIC::ISTORE_1);
   code->add_opcode(codegen::MNEMONIC::ILOAD_1);
   code->add_opcode(codegen::MNEMONIC::BIPUSH);
   code->add_byte('0');
   code->add_opcode_with_idx(codegen::MNEMONIC::IF_ICMPLT, 19);
   code->add_opcode(codegen::MNEMONIC::ILOAD_1);
   code->add_opcode(codegen::MNEMONIC::BIPUSH);
   code->add_byte('9');
   code->add_opcode_with_idx(codegen::MNEMONIC::IF_ICMPGT, 13);

   // [push Integer]: push Integer.valueOf(input - 48)
   code->add_opcode(codegen::MNEMONIC::ILOAD_1);
   code->add_opcode(codegen::MNEMONIC::BIPUSH);
   code->add_byte('0');
   code->add_opcode(codegen::MNEMONIC::ISUB);
   code->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx);
   code->add_opcode_with_idx(codegen::MNEMONIC::GOTO, 8);

   // [push String]: push String.valueOf((char) input)
   code->add_opcode(codegen::MNEMONIC::ILOAD_1);
   code->add_opcode(codegen::MNEMONIC::I2C);
   code->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.str_idx.value_of_idx);

   // PUUSH
   code->globalstack_push();

   code->inc_local_count(2);
}

void codegen::underflow_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();
  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.arr_idx.size)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx)
      ->globalstack_push();
}

void codegen::type_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->globalstack_pop()
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.getClass)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.toString)
      ->add_opcode(codegen::MNEMONIC::ASTORE_1)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      // load string we want to replace
      ->add_ldc_string("class java.lang.")
      // load replacement string
      ->add_ldc_string("")
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.str_idx.replace)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.str_idx.toLowerCase)
      ->globalstack_push();
}

//CONTROL STRUCTURE
void codegen::if_or_while_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;

  Bytecode *successor1 = code->build(state.current_node->successor1);
  Bytecode *successor2 = code->build(state.current_node->successor2);

  code->globalstack_pop()
      ->add_conditional_with_else_branch(codegen::MNEMONIC::IFNE,
                                         successor1->get_bytecode(),
                                         successor2->get_bytecode());

  // std::cout << "if_or_while_Bytecode: " << state.current_node->command.extractAstCommandString() << std::endl;
  // std::cout << "successor1: " << state.current_node->successor1->command.extractAstCommandString() << std::endl;
  // std::cout << "successor2: " << state.current_node->successor2->command.extractAstCommandString() << std::endl;
}

//VARIABLES
void codegen::pop_Variable(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  std::string var_name = state.current_node->command.extractAstCommandString();
  uint8_t var_index =  code->get_locals().getIndexForVar(var_name);
  code->add_opcode(codegen::MNEMONIC::ALOAD)
      ->add_index(var_index)
      ->globalstack_push();
}

void codegen::push_Variable(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  std::string var_name = state.current_node->command.extractAstCommandString();
  uint8_t var_index = code->get_locals().getIndexForVar(var_name);
  code->globalstack_pop()
      ->add_opcode(codegen::ASTORE)
      ->add_index(var_index);
}
