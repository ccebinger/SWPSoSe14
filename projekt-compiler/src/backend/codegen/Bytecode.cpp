#include <backend/codegen/Bytecode.h>


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

codegen::Bytecode::Bytecode(ConstantPool& p) : pool(p), locals(4) {}

codegen::Bytecode::~Bytecode() {}


codegen::Bytecode* codegen::Bytecode::build(Graphs::Graph_ptr graph)
{
  Graphs::Node_ptr current_node(graph->start());
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

size_t codegen::Bytecode::length()
{
  return bytecode.size();
}

int codegen::Bytecode::get_local_count()
{
  return local_count + locals.current_var_count();
}

ConstantPool& codegen::Bytecode::get_constant_pool()
{
  return pool;
}

codegen::Bytecode::Code& codegen::Bytecode::get_bytecode()
{
  return bytecode;
}


//================================================================================
//==================================INDICIES======================================
//================================================================================
uint16_t codegen::Bytecode::get_class_idx(const std::string& class_name)
{
  return pool.addClassRef(pool.addString(class_name));
}

uint16_t codegen::Bytecode::get_name_type_idx(const std::string& name, const std::string& type) {
  uint16_t name_idx = pool.addString(name);
  uint16_t type_idx = pool.addString(type);
  return pool.addNameAndType(name_idx, type_idx);
}

uint16_t codegen::Bytecode::get_method_idx(const std::string& class_name, const std::string& member_name, const std::string& descriptor)
{
  return pool.addMethRef(get_class_idx(class_name), get_name_type_idx(member_name, descriptor));
}

uint16_t codegen::Bytecode::get_field_idx(const std::string& class_name, const std::string& member_name, const std::string& descriptor)
{
  return pool.addFieldRef(get_class_idx(class_name), get_name_type_idx(member_name, descriptor));
}


uint16_t  codegen::Bytecode::get_stack_method_idx(const std::string& method, const std::string& descriptor)
{
  return get_method_idx("java/util/ArrayDeque", method, descriptor);
}

uint16_t codegen::Bytecode::get_stack_field_idx()
{
  return get_field_idx(Env::getDstClassName(), "stack", "Ljava/util/ArrayDeque;");
}

//================================================================================
//==================================CODE ADD======================================
//================================================================================

codegen::Bytecode* codegen::Bytecode::add_index(uint16_t indexInPool)
{
  return add_index(indexInPool, bytecode);
}


codegen::Bytecode* codegen::Bytecode::add_index(uint16_t indexInPool, std::vector<unsigned char>& code)
{
  code.push_back((indexInPool & 0xFF00U) >> 8);
  code.push_back(indexInPool & 0x00FFU);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_conditional_with_instruction(unsigned char conditional_stmt, unsigned char* conditional_body)
{
  int length = sizeof conditional_body / sizeof conditional_body[0];
  int length_plus_branch = length + 2;
  std::stringstream sstream;
  sstream.fill('\x0');
  sstream.width(4);
  sstream << std::hex << length_plus_branch;
  std::string branch = sstream.str();

  bytecode.push_back(conditional_stmt);
  for (int i = 0; i < 4; i++)
    bytecode.push_back(branch.at(i));
  for (int i = 0; i < length; i++) {
    bytecode.push_back(conditional_body[i]);
  }
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_conditional_with_else_branch(unsigned char conditional_stmt, unsigned char* conditional_body, unsigned char* else_body)
{
  int if_length = sizeof conditional_body / sizeof conditional_body[0];
  std::vector<char> if_body;
  if_body.insert(if_body.begin(), conditional_body, conditional_body + if_length);

  int else_length = sizeof else_body / sizeof else_body[0];
  int branch_idx = else_length + 2; ///TODO: CHECK IF BRANCH IS CORRECT should be after else branch

  std::stringstream sstream;
  sstream.fill('\x0');
  sstream.width(4);
  sstream << std::hex << branch_idx;
  std::string branch = sstream.str();
  if_body.push_back(codegen::MNEMONIC::GOTO);
  for (int i = 0; i < 4; i++)
    if_body.push_back(branch.at(i));


  bytecode.push_back(conditional_stmt);
  add_index(if_body.size()); /// TODO: CHECK IF BRANCH IS CORRECT should be after goto!!
  bytecode.insert(bytecode.end(), if_body.begin(), if_body.end());
  bytecode.insert(bytecode.end(), else_body, else_body + else_length);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_opcode_with_idx(codegen::MNEMONIC opcode, uint16_t idx)
{
  return add_opcode_with_idx(opcode, idx, bytecode);
}


codegen::Bytecode* codegen::Bytecode::add_opcode_with_idx(codegen::MNEMONIC opcode, uint16_t idx, std::vector<unsigned char>& code)
{
  code.push_back(opcode);
  add_index(idx, code);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_type_check(uint16_t class_idx)
{
  add_opcode_with_idx(codegen::MNEMONIC::INSTANCE_OF, class_idx);
  std::vector<unsigned char> body;
  add_opcode_with_idx(codegen::MNEMONIC::NEW, get_class_idx("java/lang/IllegalArgumentException"), body);
  body.push_back(codegen::MNEMONIC::ATHROW);
  add_conditional_with_instruction(codegen::MNEMONIC::IFNE, &body[0]);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_static_field_method_call(uint16_t field_idx, uint16_t method_idx)
{
  add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, field_idx);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, method_idx);
  return this;
}

codegen::Bytecode* codegen::Bytecode::add_integer_calculation(MNEMONIC calculation)
{
  uint16_t intValue_idx = pool.int_idx.int_value_idx;


  add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, pool.arr_idx.field_idx);
  globalstack_pop();
  //We may want to change the checkcast here. I needed it for it to work, but maybe we find another way
  add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.int_idx.class_idx);
  //BytecodeGenerator::add_instance_of(constantPool.addClassRef(constantPool.addString("java/lang/Integer")), constantPool, result);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, intValue_idx);
  bytecode.push_back(codegen::MNEMONIC::ISTORE_1);
  globalstack_pop();
  add_opcode_with_idx(codegen::MNEMONIC::CHECKCAST, pool.int_idx.class_idx);
  //BytecodeGenerator::add_instance_of(constantPool.addClassRef(constantPool.addString("java/lang/Integer")), constantPool, result);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, intValue_idx);
  bytecode.push_back(codegen::MNEMONIC::ILOAD_1);
  bytecode.push_back(calculation);
  add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx);
  globalstack_push();

  return this;
}

codegen::Bytecode* codegen::Bytecode::add_opcode(codegen::MNEMONIC opcode)
{
  bytecode.push_back(opcode);
  return this;
}

//================================================================================
//=================================GLOBAL STACK===================================
//================================================================================


codegen::Bytecode* codegen::Bytecode::globalstack_pop()
{
  return add_static_field_method_call(pool.arr_idx.field_idx, pool.arr_idx.pop_idx);
}

codegen::Bytecode* codegen::Bytecode::globalstack_push()
{
  return add_static_field_method_call(pool.arr_idx.field_idx, pool.arr_idx.push_idx);
}

/*
Bytecode* add_invoke_virtual(uint16_t method_idx);
Bytecode* add_invoke_(uint16_t method_idx);
Bytecode* add__field(uint16_t field_idx);
Bytecode* add_new_object(uint16_t class_idx);

Bytecode* add_instance_of(uint16_t class_idx);
Bytecode* add_cast(uint16_t class_idx);
Bytecode* add_throw_exception(uint16_t class_idx);
*/
//================================================================================
//==================================FUNCTORS======================================
//================================================================================

void codegen::output_ByteCode(Bytecode::Current_state state)
{
  Bytecode* code = state.current_code;
  // push system.out+
  // get <Field java/lang/System.out:Ljava/io/PrintStream;>
  uint16_t field_system_idx = code->get_field_idx("java/lang/System", "out", "Ljava/io/PrintStream;");
  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, field_system_idx)->globalstack_pop()
      ->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, code->get_constant_pool().obj_idx.toString);
  uint16_t println_idx = code->get_method_idx("java/io/PrintStream", "println", "(Ljava/lang/String;)V");
  code->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_VIRTUAL, println_idx);
}

void codegen::push_ByteCode(Bytecode::Current_state state)
{
  Bytecode* code = state.current_code;
  ConstantPool& pool = code->get_constant_pool();
  uint16_t field_idx = pool.arr_idx.field_idx;

  code->add_opcode_with_idx(codegen::MNEMONIC::GET_STATIC, field_idx)->add_opcode(codegen::MNEMONIC::LDC);
  std::string value = state.current_node->command.arg;
  try {
    int int_val = std::stoi(value);
    code->add_index(pool.addInt(int_val))->add_opcode_with_idx(codegen::MNEMONIC::INVOKE_STATIC, pool.int_idx.value_of_idx);
    //result.push_back(constantPool.addInt(int_val)); TODO check if add index is correct ?! or perhaps we should use directly push
  }
  // the value is a string
  catch (const std::invalid_argument& ia) {
    uint16_t string_idx = pool.addString(state.current_node->command.arg);
    uint16_t const_idx = pool.addConstString(string_idx);
    code->add_index(const_idx);
    //result.push_back(const_idx); TODO SAME HERE
  }
  code->globalstack_push();
}
//ARITMETIC OPERATIONS
void codegen::add_ByteCode(Bytecode::Current_state state)
{
  state.current_code->add_integer_calculation(codegen::MNEMONIC::IADD);
}
void codegen::sub_ByteCode(Bytecode::Current_state state)
{
  state.current_code->add_integer_calculation(codegen::MNEMONIC::ISUB);
}

void codegen::mult_ByteCode(Bytecode::Current_state state)
{
  state.current_code->add_integer_calculation(codegen::MNEMONIC::IMULT);
}

void codegen::div_ByteCode(Bytecode::Current_state state)
{
  state.current_code->add_integer_calculation(codegen::MNEMONIC::IDIV);
}

void codegen::mod_ByteCode(Bytecode::Current_state state)
{
  state.current_code->add_integer_calculation(codegen::MNEMONIC::IREM);
}
//STRING OPERATIONS
void codegen::cut_ByteCode(Bytecode::Current_state state) { }
void codegen::append_ByteCode(Bytecode::Current_state state) { }
void codegen::size_ByteCode(Bytecode::Current_state state) { }
//CALL
void codegen::call_ByteCode(Bytecode::Current_state state) { }
// LIST OPERATIONS
void codegen::null_ByteCode(Bytecode::Current_state state) { }
void codegen::list_push_ByteCode(Bytecode::Current_state state) { }
void codegen::list_pop_ByteCode(Bytecode::Current_state state) { }
//BOOLEAN ARITHMETIC
void codegen::false_ByteCode(Bytecode::Current_state state) { }
void codegen::greater_ByteCode(Bytecode::Current_state state) { }
void codegen::equal_ByteCode(Bytecode::Current_state state) { }
void codegen::true_ByteCode(Bytecode::Current_state state) { }
//IO OPERATIONS
void codegen::boom_ByteCode(Bytecode::Current_state state) { }
void codegen::eof_ByteCode(Bytecode::Current_state state) { }
void codegen::input_ByteCode(Bytecode::Current_state state) { }
void codegen::underflow_ByteCode(Bytecode::Current_state state) { }
void codegen::type_ByteCode(Bytecode::Current_state state) { }
//CONTROL STRUCTURE
void codegen::if_or_while_ByteCode(Bytecode::Current_state state) { }
//VARIABLES
void codegen::pop_Variable(Bytecode::Current_state state) { }
void codegen::push_Variable(Bytecode::Current_state state) { }
