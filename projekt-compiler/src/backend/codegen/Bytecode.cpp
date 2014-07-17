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
  {Command::Type::LAMBDA, &lambda_Bytecode},
  {Command::Type::BOOM, &boom_ByteCode},
  {Command::Type::EOF_CHECK, &eof_ByteCode},
  {Command::Type::INPUT, &input_ByteCode},
  {Command::Type::UNDERFLOW_CHECK, &underflow_ByteCode},
  {Command::Type::TYPE_CHECK, &type_ByteCode},
  {Command::Type::EASTJUNC, &if_ByteCode},
  {Command::Type::WESTJUNC, &if_ByteCode},
  {Command::Type::NORTHJUNC, &if_ByteCode},
  {Command::Type::SOUTHJUNC, &if_ByteCode},
  {Command::Type::VAR_POP, &pop_Variable},
  {Command::Type::VAR_PUSH, &push_Variable}
};

codegen::Bytecode::Bytecode(ConstantPool& p) : pool(p),
                                               locals(4),
                                               lambda_code(false){
}

codegen::Bytecode::Bytecode(ConstantPool& p,
                            std::string& name,
                            bool is_lambda_code) : pool(p),
                                                   locals(4),
                                                   lambda_code(is_lambda_code),
                                                   class_name(name) {}

codegen::Bytecode::~Bytecode() {}


codegen::Bytecode* codegen::Bytecode::build(Graphs::Graph_ptr graph) {
  Graphs::Node_ptr current_node(graph->start());
  while (current_node && current_node->command.type != Command::Type::FINISH) {
    func_ptr f = func_map.at(current_node->command.type);
    if (f) {
      Current_state state;
      state.current_code = this;
      state.current_node = current_node;
      f(state);
    }
    // end Program after junction, because junction resolve recursivly and after
    // both successor's path are build there is nothing to do
    if((current_node->command.type == Command::Type::EASTJUNC) ||
       (current_node->command.type == Command::Type::WESTJUNC) ||
       (current_node->command.type == Command::Type::NORTHJUNC) ||
       (current_node->command.type == Command::Type::SOUTHJUNC)){
      break;
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
  return this;
}

//================================================================================
//==================================GETTER========================================
//================================================================================

size_t codegen::Bytecode::length() {
  return bytecode.size();
}

int codegen::Bytecode::get_local_count() {
  return /*local_count*/ locals.reserved_var_count() + locals.current_var_count();
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


uint16_t codegen::Bytecode::get_lambda_closure_idx() {
  return lambda_closure_idx;
}

codegen::Bytecode::STRINGS codegen::Bytecode::get_lambdas() {
  return lambdas;
}

bool codegen::Bytecode::is_lambda_code() {
  return lambda_code;
}

bool codegen::Bytecode::find_lambda(std::string& name) {
  STRINGS::iterator pos = std::find(lambdas.begin(), lambdas.end(), name);
  return pos != lambdas.end();
}

LocalVariableStash codegen::Bytecode::get_lambda_locals(std::string& name) {
  return lamda_variables.at(name);
}

std::string codegen::Bytecode::get_class_name() {
  return class_name;
}
//================================================================================
//==================================SETTER========================================
//================================================================================

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
  uint16_t length_plus_branch = length + 3;
  bytecode.push_back(conditional_stmt);
  add_index(length_plus_branch);
  bytecode.insert(bytecode.end(), conditional_body.begin(), conditional_body.end());
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_conditional_with_instruction(unsigned char conditional_stmt,
                                                                       std::vector<unsigned char> conditional_body,
                                                                       std::vector<unsigned char>& code) {
  int length = conditional_body.size();
  uint16_t length_plus_branch = length + 3;
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
  branch_idx = if_body.size() + 3 ; //+ 3 jump over goto + branch idx

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
  //inc_local_count(2);
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

codegen::Bytecode* codegen::Bytecode::add_ldc_string(const std::string& constant, std::vector<unsigned char>& code){
  code.push_back(codegen::MNEMONIC::LDC);
  uint16_t string_idx = pool.addString(constant);
  uint16_t const_idx = pool.addConstString(string_idx);
  code.push_back((uint8_t) const_idx);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_lambda_declaration(Graphs::Node_ptr current_node) {
  //add Lambda anonymous class to pool
  //$anonymous class
  std::stringstream ss;
  std::string anonymous_class_idx = current_node->command.arg.replace(0,1, "$"); //replace & with $
  ss << Env::getDstClassName() << anonymous_class_idx;
  std::string anonymous_class_name = ss.str();
  lambdas.push_back(anonymous_class_idx);
  lamda_variables.insert(std::pair<std::string, LocalVariableStash> (anonymous_class_idx, locals)); //add locals which are exists on the time on which lambda is declared

  size_t anonym_str_idx = pool.addString(anonymous_class_name);
  size_t anonym_class_idx = pool.addClassRef(anonym_str_idx);

  //$anonymous class <init> method
  size_t init_str_idx = pool.addString("<init>");

  size_t count = locals.current_var_count();
  size_t init_descriptor = pool.addString(create_object_descriptor(count));


  size_t void_descriptor = pool.addString("()V");
  size_t anonym_init_idx = pool.addMethRef(anonym_class_idx, pool.addNameAndType(init_str_idx, init_descriptor));

  //$anonymous class closure method
  size_t closure_str_idx = pool.addString("closure");
  size_t lambda_str_idx = pool.addString("Lambda");
  size_t lambda_cls_idx = pool.addClassRef(lambda_str_idx);
  lambda_closure_idx = pool.addInterfaceMethodRef(lambda_cls_idx, pool.addNameAndType(closure_str_idx, void_descriptor));
  //CODE
  add_opcode_with_idx(codegen::MNEMONIC::NEW, anonym_class_idx);
  add_opcode(codegen::MNEMONIC::DUP);

  if (count > 0) {
    std::map<std::string, uint8_t> local_map = locals.get_variable_to_index_map();
    for (std::map<std::string, uint8_t>::iterator it = local_map.begin(); it != local_map.end(); it++) {
      add_opcode(codegen::MNEMONIC::ALOAD);
      add_byte(it->second);
    }
  }
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_SPECIAL, anonym_init_idx);
  globalstack_push();
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_pop_variable_code(std::string& var_name) {
  uint8_t var_index =  locals.getIndexForVar(var_name);
  add_opcode(codegen::MNEMONIC::ALOAD);
  add_byte(var_index);
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

std::string codegen::Bytecode::create_object_descriptor(size_t len) {
  std::stringstream ss;
  ss << "(";
  if (len > 0) {
    ss << "Ljava/lang/Object";
    for (size_t i = 0; i < (len - 1); i++)
    {
      ss << ", Ljava/lang/Object";
    }
    ss << ";";
  }
  ss << ")V";
  return ss.str();
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
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.stream_idx.print_idx);
}

void codegen::push_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->add_opcode(codegen::MNEMONIC::LDC);

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
      ->globalstack_push();
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
      ->globalstack_push();
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
	ConstantPool& pool = code->get_constant_pool();
	std::string value = state.current_node->command.extractAstCommandString();

  if (value.empty()) {
    uint16_t idx = code->get_lambda_closure_idx();

    if (!idx)
      idx = pool.addInterfaceMethodRef(code->get_class_idx("Lambda"),
                                 code->get_name_type_idx("closure", "()V"));

    code->globalstack_pop()
        ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_INTERFACE, idx)
        ->add_byte(1) // 1 stands for the object which will be used for the call, more than 1 are the arguments which are popped from the stack
        ->add_byte(0); // 0 is the sign for the end of arguments

  } else {
    code->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, code->get_method_idx(Env::getDstClassName(), value, "()V"));
  }
}

// LIST OPERATIONS
void codegen::null_ByteCode(Bytecode::Current_state state) {
  codegen::Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx)
      ->add_opcode_with_idx(codegen::MNEMONIC::NEW, pool.list_idx.class_idx)
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_SPECIAL, pool.list_idx.init_idx)
      ->globalstack_push();
}

void codegen::list_push_ByteCode(Bytecode::Current_state state) {
  codegen::Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();

  std::vector<unsigned char> conditional_body;

  // fill conditional branch
  conditional_body.push_back(MNEMONIC::POP);
  code->add_opcode_with_idx(codegen::MNEMONIC::NEW, pool.list_idx.class_idx, conditional_body);
  conditional_body.push_back(codegen::MNEMONIC::DUP);
  code->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_SPECIAL, pool.list_idx.init_idx, conditional_body);
  // use variable 1 as first element of the list
  conditional_body.push_back(MNEMONIC::DUP);
  conditional_body.push_back(ALOAD_1);
  code->add_opcode_with_idx(MNEMONIC::INVOKE_VIRTUAL, pool.list_idx.add_idx, conditional_body);
  conditional_body.push_back(MNEMONIC::POP); // discard add() return value

  code->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_2)
      ->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_1)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_opcode(MNEMONIC::DUP)
      ->add_opcode_with_idx(codegen::MNEMONIC::INSTANCE_OF, pool.list_idx.class_idx)
      ->add_conditional_with_instruction(codegen::MNEMONIC::IFNE, conditional_body)
      ->add_opcode(codegen::MNEMONIC::ASTORE_1)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.list_idx.class_idx)
      ->add_opcode(codegen::MNEMONIC::DUP)
      ->add_opcode(codegen::MNEMONIC::ALOAD_2)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.list_idx.add_idx)
      ->add_opcode(MNEMONIC::POP) // discard add() return value
      ->globalstack_push();
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
      ->globalstack_push();

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
      ->globalstack_push();
      //->inc_local_count(3);
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
      ->globalstack_push();
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

  std::vector<unsigned char> first_conditional_body;
  std::vector<unsigned char> second_conditional_body;
  std::vector<unsigned char> third_conditional_body;
  std::vector<unsigned char> fourth_conditional_body;
  std::vector<unsigned char> else_branch;

  code->add_ldc_string("string", first_conditional_body);
  first_conditional_body.push_back(codegen::MNEMONIC::ASTORE_1);

  code->add_ldc_string("nil", third_conditional_body);
  third_conditional_body.push_back(codegen::MNEMONIC::ASTORE_1);

  code->add_ldc_string("lambda", fourth_conditional_body);
  fourth_conditional_body.push_back(codegen::MNEMONIC::ASTORE_1);

  second_conditional_body.push_back(codegen::MNEMONIC::ALOAD_2);
  code->add_ldc_string("list", second_conditional_body);
  second_conditional_body.push_back(codegen::MNEMONIC::ASTORE_1);
  code->add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.list_idx.class_idx, second_conditional_body)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.list_idx.isEmpty_idx, second_conditional_body)
      ->add_conditional_with_instruction(codegen::MNEMONIC::IFEQ, third_conditional_body, second_conditional_body);

  code->globalstack_pop()
      ->add_opcode(codegen::MNEMONIC::ASTORE_2)
      ->add_opcode(codegen::MNEMONIC::ALOAD_2)
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
      ->add_opcode(codegen::MNEMONIC::ASTORE_1)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_ldc_string("integer")
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.equals)
      ->add_conditional_with_instruction(codegen::MNEMONIC::IFEQ, first_conditional_body)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_ldc_string("class java.util.arraylist")
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.obj_idx.equals)
      ->add_conditional_with_instruction(codegen::MNEMONIC::IFEQ, second_conditional_body)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->add_ldc_string("class main")
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, pool.str_idx.startsWith_idx)
      ->add_conditional_with_instruction(codegen::MNEMONIC::IFEQ, fourth_conditional_body)
      ->add_opcode(codegen::MNEMONIC::ALOAD_1)
      ->globalstack_push();
}

//CONTROL STRUCTURE
void codegen::if_ByteCode(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();
  // store node because traversing successor1 end with last
  // node of successor1
  Graphs::Node_ptr store_node = state.current_node->successor2;
  // Code that will be executed if we encounter a non-bool value
  std::vector<unsigned char> crash_code;
  uint16_t exc_idx = code->get_method_idx("java/lang/RuntimeException", "<init>", "()V");

  code->add_opcode_with_idx(MNEMONIC::NEW, code->get_class_idx("java/lang/RuntimeException"), crash_code);
  crash_code.push_back(MNEMONIC::DUP);
  code->add_opcode_with_idx(MNEMONIC::INVOKE_SPECIAL, exc_idx, crash_code);
  crash_code.push_back(MNEMONIC::ATHROW);

  Bytecode successor1(state.current_code->get_constant_pool());
  successor1.build(state.current_node->successor1);
  Bytecode successor2(state.current_code->get_constant_pool());
  successor2.build(store_node);

  code->globalstack_pop()
      ->add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST,
                            pool.int_idx.class_idx)
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL,
                            pool.int_idx.int_value_idx)
      // test if we are really dealing with a bool
      ->add_opcode(MNEMONIC::DUP)
      ->add_opcode(MNEMONIC::DUP)
      // < 0? throw!
      ->add_opcode(MNEMONIC::ICONST_0)
      ->add_conditional_with_instruction(MNEMONIC::IF_ICMPGE, crash_code)
      // > 1? throw!
      ->add_opcode(MNEMONIC::ICONST_1)
      ->add_conditional_with_instruction(MNEMONIC::IF_ICMPLE, crash_code)
      // This is the real branch
      ->add_conditional_with_else_branch(codegen::MNEMONIC::IFEQ,
                                         successor1.get_bytecode(),
                                         successor2.get_bytecode());
}

//VARIABLES
void codegen::pop_Variable(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();
  std::string var_name = state.current_node->command.extractAstCommandString();
  if (code->is_lambda_code()) {
    LocalVariableStash& locals = code->get_locals();
    if (locals.current_var_count() > 0 && locals.exists(var_name)) {
      code->add_pop_variable_code(var_name);
    }
    else {
      std::stringstream ss;
      ss << "var$" << var_name;
      uint16_t field_idx = code->get_field_idx(code->get_class_name(), ss.str(), "Ljava/lang/Object;");
      code->add_opcode(codegen::MNEMONIC::ALOAD_0)
          ->add_opcode_with_idx(codegen::MNEMONIC::GETFIELD, field_idx);
    }
  } else {
    code->add_pop_variable_code(var_name);
  }
  code->globalstack_push();
}

void codegen::push_Variable(Bytecode::Current_state state) {
  Bytecode* code = state.current_code;
  std::string var_name = state.current_node->command.extractAstCommandString();
  uint8_t var_index = code->get_locals().getIndexForVar(var_name);
  code->globalstack_pop()
      ->add_opcode(codegen::ASTORE)
      ->add_index(var_index);
}

//LAMBDA

void codegen::lambda_Bytecode(Bytecode::Current_state state) {
  state.current_code->add_lambda_declaration(state.current_node);
}
