/*[--**--]
Copyright (C) 2014  SWPSoSe14Cpp Group

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this
program; if not, see <http://www.gnu.org/licenses/>.*/

////////////////////////////////////////////////////////////////////////
// ! \file bytecode_generator.cpp
/// definition
////////////////////////////////////////////////////////////////////////

#include <backend/codegen/bytecode_generator.h>
#include <cstdint>
#include <vector>
#include <string>
#include <map>

const std::map<Command::Type, BytecodeGenerator::func_ptr>
BytecodeGenerator::CODE_FUNC_MAPPING = {
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

int BytecodeGenerator::localCount = 0;


//================================================================================================================
//================================================ADD INSTRUCTIONS================================================
//================================================================================================================


void BytecodeGenerator::add_conditional_with_instruction(char conditional_stmt,
                                                         char* conditional_body,
                                                         std::vector<char>& result) {
  int length = sizeof conditional_body / sizeof conditional_body[0];
  int length_plus_branch = length + 2;
  std::stringstream sstream;
  sstream.fill('\x0');
  sstream.width(4);
  sstream << std::hex << length_plus_branch;
  std::string branch = sstream.str();

  result.push_back(conditional_stmt);
  for (int i = 0; i < 4; i++)
    result.push_back(branch.at(i));
  for (int i = 0; i < length; i++) {
    result.push_back(conditional_body[i]);
  }
}

void BytecodeGenerator::add_conditional_with_else_branch(char conditional_stmt,
                                                         char* conditional_body,
                                                         char* else_body,
                                                         std::vector<char>& result) {

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
  if_body.push_back(BytecodeGenerator::GOTO);
  for (int i = 0; i < 4; i++)
    if_body.push_back(branch.at(i));


  result.push_back(conditional_stmt);
  add_index(if_body.size(), result); /// TODO: CHECK IF BRANCH IS CORRECT should be after goto!!
  result.insert(result.end(), if_body.begin(), if_body.end());
  result.insert(result.end(), else_body, else_body + else_length);
}

void BytecodeGenerator::add_invoke_virtual(uint16_t method_idx,
                                           ConstantPool& constantPool,
                                           std::vector<char>& result) {
  add_invoke_method(BytecodeGenerator::INVOKE_VIRTUAL, method_idx,
                    constantPool, result);
}

void BytecodeGenerator::add_invoke_static(uint16_t method_idx,
                                          ConstantPool& constantPool,
                                          std::vector<char>& result) {
  add_invoke_method(BytecodeGenerator::INVOKE_STATIC, method_idx,
                    constantPool, result);
}

void BytecodeGenerator::add_invoke_method(BytecodeGenerator::MNEMONIC opcode,
                                          uint16_t method_idx,
                                          ConstantPool& constantPool,
                                          std::vector<char>& result) {
  result.push_back(opcode);
  add_index(method_idx, result);
}

void BytecodeGenerator::add_index(uint16_t indexInPool,
                                  std::vector<char>& result) {
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
}

void BytecodeGenerator::add_static_field(uint16_t field_idx,
                                         ConstantPool& constantPool,
                                         std::vector<char>& result) {
  result.push_back(BytecodeGenerator::GET_STATIC);
  add_index(field_idx, result);
}

void BytecodeGenerator::add_static_field_method_call(uint16_t field_idx,
                                                     uint16_t method_idx,
                                                     ConstantPool& constantPool,
                                                     std::vector<char>& result)
{
  BytecodeGenerator::add_static_field(field_idx, constantPool, result);
  BytecodeGenerator::add_invoke_virtual(method_idx, constantPool, result);
}

void BytecodeGenerator::add_new_object(uint16_t class_idx,
                                       ConstantPool& constantPool,
                                       std::vector<char>& result) {
  result.push_back(BytecodeGenerator::NEW);
  add_index(class_idx, result);
}

void BytecodeGenerator::add_cast(uint16_t class_idx,
                                        ConstantPool& constantPool,
                                        std::vector<char>& result) {
  result.push_back(BytecodeGenerator::CHECKCAST);
  add_index(class_idx, result);
}


void BytecodeGenerator::add_instance_of(uint16_t class_idx,
                                        ConstantPool& constantPool,
                                        std::vector<char>& result) {
  result.push_back(BytecodeGenerator::INSTANCE_OF);
  add_index(class_idx, result);
}

void BytecodeGenerator::add_type_check(uint16_t class_idx,
                                       ConstantPool& constantPool,
                                       std::vector<char>& result) {
  add_instance_of(class_idx, constantPool, result);
  std::vector<char> body;
  add_throw_exception(add_class("java/lang/IllegalArgumentException", constantPool), constantPool, body);
  add_conditional_with_instruction(BytecodeGenerator::IFNE, &body[0], result);
}

void BytecodeGenerator::add_throw_exception(uint16_t class_idx,
                                            ConstantPool& constantPool,
                                            std::vector<char>& result) {
  add_new_object(class_idx, constantPool, result);
  result.push_back(BytecodeGenerator::ATHROW);
}


//================================================================================================================
//================================================ADD CONSTANTPOOL================================================
//================================================================================================================

uint16_t BytecodeGenerator::add_class(const std::string& class_name,
                                  ConstantPool& constantPool) {

  return constantPool.addClassRef(constantPool.addString(class_name));
}

uint16_t BytecodeGenerator::add_field(const std::string& class_name,
                                      const std::string& member_name,
                                      const std::string& descriptor,
                                      ConstantPool& constantPool)
{
  return constantPool.addFieldRef(add_class(class_name, constantPool), add_name_type(member_name, descriptor, constantPool));
}

uint16_t BytecodeGenerator::add_method(const std::string& class_name,
                                       const std::string& member_name,
                                       const std::string& descriptor,
                                       ConstantPool& constantPool)
{
  return constantPool.addMethRef(add_class(class_name, constantPool), add_name_type(member_name, descriptor, constantPool));
}

uint16_t BytecodeGenerator::add_name_type(const std::string& name,
                                          const std::string& type,
                                          ConstantPool& constantPool)
{
  uint16_t name_idx = constantPool.addString(name);
  uint16_t type_idx = constantPool.addString(type);
  return constantPool.addNameAndType(name_idx, type_idx);
}


//================================================================================================================
//================================================FUNCTIONS=======================================================
//================================================================================================================
void output_ByteCode(ConstantPool& constantPool,
                     std::vector<char>& result,
                     Graphs::Node_ptr current_node) {
  // push system.out+
  // get <Field java/lang/System.out:Ljava/io/PrintStream;>
  uint16_t field_system_idx = BytecodeGenerator::add_field("java/lang/System", "out", "Ljava/io/PrintStream;", constantPool);
  BytecodeGenerator::add_static_field(field_system_idx, constantPool, result);
  globalstack_pop(constantPool,result);
  uint16_t toString_idx = BytecodeGenerator::add_method("java/lang/Object", "toString", "()Ljava/lang/String;", constantPool);
  BytecodeGenerator::add_invoke_virtual(toString_idx, constantPool, result);
  uint16_t println_idx = BytecodeGenerator::add_method("java/io/PrintStream", "println", "(Ljava/lang/String;)V", constantPool);
  BytecodeGenerator::add_invoke_virtual(println_idx, constantPool, result);

  BytecodeGenerator::localCount++;
}

void push_ByteCode(ConstantPool& constantPool,
                   std::vector<char>& result,
                   Graphs::Node_ptr current_node) {

  uint16_t field_system_idx = BytecodeGenerator::add_field("Main", "stack", "Ljava/util/ArrayDeque;", constantPool);
  BytecodeGenerator::add_static_field(field_system_idx, constantPool, result);
	// ldc indexInPool
  result.push_back(BytecodeGenerator::LDC);
  std::string value = current_node->command.arg;
  try {
    int int_val = std::stoi(value);
    result.push_back(constantPool.addInt(int_val));
    uint16_t valueOf_idx = BytecodeGenerator::add_method("java/lang/Integer", "valueOf", "(I)Ljava/lang/Integer;", constantPool);
    BytecodeGenerator::add_invoke_static(valueOf_idx, constantPool,result);
  }
  // the value is a string
  catch (const std::invalid_argument& ia) {
    uint16_t string_idx = constantPool.addString(current_node->command.arg);
    uint16_t const_idx = constantPool.addConstString(string_idx);
    result.push_back(const_idx);
  }
  globalstack_push(constantPool, result);
}

void add_integer_calculation(BytecodeGenerator::MNEMONIC calculation,
                             ConstantPool& constantPool,
                             std::vector<char>& result) {

  uint16_t field_stack_idx = BytecodeGenerator::add_field("Main", "stack", "Ljava/util/ArrayDeque;", constantPool);
  uint16_t pop_idx = BytecodeGenerator::add_method("java/util/ArrayDeque", "pop", "()Ljava/lang/Object;", constantPool);
  uint16_t intValue_idx = BytecodeGenerator::add_method("java/lang/Integer", "intValue", "()I", constantPool);

  BytecodeGenerator::add_static_field(field_stack_idx, constantPool, result);
  globalstack_pop(constantPool,result);
  //We may want to change the checkcast here. I needed it for it to work, but maybe we find another way
  //TODO: checkcast entfernen
  result.push_back(BytecodeGenerator::CHECKCAST);
  //TODO: umrechnen auf 2 bytes
  result.push_back('\x00');
  result.push_back(constantPool.addClassRef(constantPool.addString("java/lang/Integer")));
  //BytecodeGenerator::add_instance_of(constantPool.addClassRef(constantPool.addString("java/lang/Integer")), constantPool, result);
  BytecodeGenerator::add_invoke_virtual(intValue_idx, constantPool, result);
  result.push_back(BytecodeGenerator::ISTORE_1);
  globalstack_pop(constantPool,result);
  //TODO: checkcast entfernen
  result.push_back(BytecodeGenerator::CHECKCAST);
  //TODO: umrechnen auf 2 bytes
  result.push_back('\x00');
  result.push_back(constantPool.addClassRef(constantPool.addString("java/lang/Integer")));
  //BytecodeGenerator::add_instance_of(constantPool.addClassRef(constantPool.addString("java/lang/Integer")), constantPool, result);
  BytecodeGenerator::add_invoke_virtual(intValue_idx, constantPool, result);
  result.push_back(BytecodeGenerator::ILOAD_1);
  result.push_back(calculation);
  uint16_t valueOf_idx = BytecodeGenerator::add_method("java/lang/Integer", "valueOf", "(I)Ljava/lang/Integer;", constantPool);
  BytecodeGenerator::add_invoke_static(valueOf_idx, constantPool,result);
  globalstack_push(constantPool, result);


  /*uint16_t integer_class = constantPool.int_idx.class_idx;
  if (integer_class == 0)
    integer_class = BytecodeGenerator::add_class("java/lang/Integer", constantPool);
  uint16_t integer_class_intValue_method = constantPool.int_idx.int_value_idx;
  if (integer_class_intValue_method == 0)
    integer_class_intValue_method = BytecodeGenerator::add_method("java/lang/Integer", "intValue", "()I", constantPool);
  uint16_t integer_class_static_value_of_method = constantPool.int_idx.value_of_idx;
  if (integer_class_static_value_of_method == 0)
    integer_class_static_value_of_method = BytecodeGenerator::add_method("java/lang/Integer", "valueOf", "(I)Ljava/lang/Integer;", constantPool);


  globalstack_pop(constantPool, result);
  globalstack_pop(constantPool, result);
  result.push_back(BytecodeGenerator::ASTORE_1);
  result.push_back(BytecodeGenerator::ASTORE_2);

  result.push_back(BytecodeGenerator::ALOAD_1);
  BytecodeGenerator::add_type_check(integer_class, constantPool, result);
  result.push_back(BytecodeGenerator::ALOAD_2);
  BytecodeGenerator::add_type_check(integer_class, constantPool, result);

  result.push_back(BytecodeGenerator::ALOAD_1);
  BytecodeGenerator::add_invoke_virtual(integer_class_intValue_method,
                                        constantPool, result);
  result.push_back(BytecodeGenerator::ALOAD_2);
  BytecodeGenerator::add_invoke_virtual(integer_class_intValue_method,
                                        constantPool, result);

  result.push_back(calculation);

  BytecodeGenerator::add_invoke_static(integer_class_static_value_of_method,
                                       constantPool, result);

  BytecodeGenerator::localCount += 2;

  globalstack_push(constantPool, result);*/
}

void add_ByteCode(ConstantPool& constantPool,
                  std::vector<char>& result,
                  Graphs::Node_ptr current_node) {
  add_integer_calculation(BytecodeGenerator::IADD, constantPool, result);
}

void sub_ByteCode(ConstantPool& constantPool,
                  std::vector<char>& result,
                  Graphs::Node_ptr current_node) {
  add_integer_calculation(BytecodeGenerator::ISUB, constantPool, result);
}

void mult_ByteCode(ConstantPool& constantPool,
                   std::vector<char>& result,
                   Graphs::Node_ptr current_node){
  add_integer_calculation(BytecodeGenerator::IMULT, constantPool, result);
}
void div_ByteCode(ConstantPool& constantPool,
                  std::vector<char>& result,
                  Graphs::Node_ptr current_node) {
  add_integer_calculation(BytecodeGenerator::IDIV, constantPool, result);
}
void mod_ByteCode(ConstantPool& constantPool,
                  std::vector<char>& result,
                  Graphs::Node_ptr current_node) {
  add_integer_calculation(BytecodeGenerator::IREM, constantPool, result);
}

void cut_ByteCode(ConstantPool& constantPool,
                  std::vector<char>& result,
                  Graphs::Node_ptr current_node) {

	uint16_t field_stack_idx = BytecodeGenerator::add_field("Main", "stack", "Ljava/util/ArrayDeque;", constantPool);
	uint16_t intValue_idx = BytecodeGenerator::add_method("java/lang/Integer", "intValue", "()I", constantPool);
	uint16_t toString_idx = BytecodeGenerator::add_method("java/lang/Object", "toString", "()Ljava/lang/String;", constantPool);
	uint16_t substring_idx = BytecodeGenerator::add_method("java/lang/String", "substring", "(II)Ljava/lang/String;", constantPool);
	uint16_t substring2_idx = BytecodeGenerator::add_method("java/lang/String", "substring", "(I)Ljava/lang/String;", constantPool);

	BytecodeGenerator::add_static_field(field_stack_idx, constantPool, result);
	globalstack_pop(constantPool,result);
	//TODO: checkcast entfernen?
	result.push_back(BytecodeGenerator::CHECKCAST);
	//TODO: umrechnen auf 2 bytes
	result.push_back('\x00');
	result.push_back(constantPool.addClassRef(constantPool.addString("java/lang/Integer")));
	//BytecodeGenerator::add_instance_of(constantPool.addClassRef(constantPool.addString("java/lang/Integer")), constantPool, result);
	BytecodeGenerator::add_invoke_virtual(intValue_idx, constantPool, result);
	result.push_back(BytecodeGenerator::ISTORE_1);
	BytecodeGenerator::add_static_field(field_stack_idx, constantPool, result);
	globalstack_pop(constantPool,result);
	result.push_back(BytecodeGenerator::ASTORE_2);
	BytecodeGenerator::add_static_field(field_stack_idx, constantPool, result);
	result.push_back(BytecodeGenerator::ALOAD_2);
	BytecodeGenerator::add_invoke_virtual(toString_idx, constantPool, result);
	result.push_back(BytecodeGenerator::ICONST_0);
	result.push_back(BytecodeGenerator::ILOAD_1);
	BytecodeGenerator::add_invoke_virtual(substring_idx, constantPool, result);
	globalstack_push(constantPool, result);
	BytecodeGenerator::add_static_field(field_stack_idx, constantPool, result);
	result.push_back(BytecodeGenerator::ALOAD_2);
	BytecodeGenerator::add_invoke_virtual(toString_idx, constantPool, result);
	result.push_back(BytecodeGenerator::ILOAD_1);
	BytecodeGenerator::add_invoke_virtual(substring2_idx, constantPool, result);
	globalstack_push(constantPool, result);


  /*// istore_0 to store the index for the cut
  globalstack_pop(constantPool, result);
  result.push_back(BytecodeGenerator::ISTORE_0);
  // astore_1 to store the begin string
  globalstack_pop(constantPool, result);
  result.push_back(BytecodeGenerator::ASTORE_1);
  // load begin string
  result.push_back(BytecodeGenerator::ALOAD_1);
  // iconst_0 for the begin of the string
  result.push_back(BytecodeGenerator::ICONST_0);
  // iload_0 to add the index until the cut should happend
  result.push_back(BytecodeGenerator::ILOAD_0);

  uint16_t method_idx = constantPool.str_idx.substring_2param_idx;

  if (method_idx == 0)
    method_idx = BytecodeGenerator::add_method("java/lang/String", "substring", "(II)Ljava/lang/String;", constantPool);
  BytecodeGenerator::add_invoke_virtual(method_idx,
                                        constantPool, result);
  // to get begin string
  result.push_back(BytecodeGenerator::ALOAD_1);
  // index for cut
  result.push_back(BytecodeGenerator::ILOAD_0);

  method_idx = constantPool.str_idx.substring_idx;
  if (method_idx == 0)
    method_idx = BytecodeGenerator::add_method("java/lang/String", "substring", "(I)Ljava/lang/String;", constantPool);
  BytecodeGenerator::add_invoke_virtual(method_idx,
                                        constantPool, result);
  globalstack_push(constantPool, result);*/
  BytecodeGenerator::localCount += 2;
}

void append_ByteCode(ConstantPool& constantPool,
                     std::vector<char>& result,
                     Graphs::Node_ptr current_node) {
	uint16_t field_stack_idx = BytecodeGenerator::add_field("Main", "stack", "Ljava/util/ArrayDeque;", constantPool);
	uint16_t toString_idx = BytecodeGenerator::add_method("java/lang/Object", "toString", "()Ljava/lang/String;", constantPool);
	uint16_t builder_toString_idx = BytecodeGenerator::add_method("java/lang/StringBuilder", "toString", "()Ljava/lang/String;", constantPool);

	uint16_t append_idx = BytecodeGenerator::add_method("java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;", constantPool);
	uint16_t valueOf_idx = BytecodeGenerator::add_method("java/lang/String", "valueOf", "(Ljava/lang/Object;)Ljava/lang/String;", constantPool);
	uint16_t builder_init_idx = BytecodeGenerator::add_method("java/lang/StringBuilder", "<init>", "(Ljava/lang/String;)V", constantPool);


	globalstack_pop(constantPool,result);
	result.push_back(BytecodeGenerator::ASTORE_1);
	BytecodeGenerator::add_static_field(field_stack_idx, constantPool, result);
	result.push_back(BytecodeGenerator::NEW);
	//TODO: umrechnen auf 2 bytes
	result.push_back('\x00');
	result.push_back(constantPool.addClassRef(constantPool.addString("java/lang/StringBuilder")));
	result.push_back(BytecodeGenerator::DUP);
	globalstack_pop(constantPool,result);
	BytecodeGenerator::add_invoke_virtual(toString_idx, constantPool, result);
    BytecodeGenerator::add_invoke_static(valueOf_idx, constantPool,result);
    BytecodeGenerator::add_invoke_method(BytecodeGenerator::INVOKE_SPECIAL, builder_init_idx, constantPool, result);
    result.push_back(BytecodeGenerator::ALOAD_1);
    BytecodeGenerator::add_invoke_virtual(toString_idx, constantPool, result);
    BytecodeGenerator::add_invoke_virtual(append_idx, constantPool, result);
    BytecodeGenerator::add_invoke_virtual(builder_toString_idx, constantPool, result);
    globalstack_push(constantPool, result);


	/*getstatic 15 [Fieldref: HelloWorld.stack, type = java.util.ArrayDeque]
	opcode [B6] - 0019: invokevirtual 32 [Methodref: java.util.ArrayDeque.pop, parameter = (), returns = java.lang.Object]
	opcode [4C] - 0022: astore_1
	opcode [B2] - 0023: getstatic 15 [Fieldref: HelloWorld.stack, type = java.util.ArrayDeque]
	opcode [BB] - 0026: name_new 36 [Class: java.lang.StringBuilder]
	opcode [59] - 0029: dup
	opcode [B2] - 0030: getstatic 15 [Fieldref: HelloWorld.stack, type = java.util.ArrayDeque]
	opcode [B6] - 0033: invokevirtual 32 [Methodref: java.util.ArrayDeque.pop, parameter = (), returns = java.lang.Object]
	opcode [B6] - 0036: invokevirtual 38 [Methodref: java.lang.Object.toString, parameter = (), returns = java.lang.String]
	opcode [B8] - 0039: invokestatic 42 [Methodref: java.lang.String.valueOf, parameter = (java.lang.Object), returns = java.lang.String]
	opcode [B7] - 0042: invokespecial 48 [Methodref: java.lang.StringBuilder.<init>, parameter = (java.lang.String), returns = void]
	opcode [2B] - 0045: aload_1
	opcode [B6] - 0046: invokevirtual 38 [Methodref: java.lang.Object.toString, parameter = (), returns = java.lang.String]
	opcode [B6] - 0049: invokevirtual 51 [Methodref: java.lang.StringBuilder.append, parameter = (java.lang.String), returns = java.lang.StringBuilder]
	opcode [B6] - 0052: invokevirtual 55 [Methodref: java.lang.StringBuilder.toString, parameter = (), returns = java.lang.String]
	opcode [B6] - 0055: invokevirtual 26 [Methodref: java.util.ArrayDeque.push, parameter = (java.lang.Object), returns = void]
*/


 /* // initial situation: the two strings are on the stacks
  // astore_1 to store the first string
  globalstack_pop(constantPool, result);
  result.push_back(BytecodeGenerator::ASTORE_1);
  // astore_2 to store the second string
  globalstack_pop(constantPool, result);
  result.push_back(BytecodeGenerator::ASTORE_2);

  // create new object of class java/lang/StringBuilder
  BytecodeGenerator::add_new_object(BytecodeGenerator::add_class("java/lang/StringBuilder", constantPool),
                                    constantPool, result);
  // duplicate object
  result.push_back(BytecodeGenerator::DUP);

  // init StringBuilder Close Label

  uint16_t meth_idx = BytecodeGenerator::add_method("java/lang/StringBuilder", "'<init>'", "()V", constantPool);
  BytecodeGenerator::add_invoke_virtual(meth_idx,
                                        constantPool, result);
  // load first string
  result.push_back(BytecodeGenerator::ALOAD_1);

  // invokevirtual <Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder>
  meth_idx = BytecodeGenerator::add_method("java/lang/StringBuilder", "append", "(Ljava/lang/String;)Ljava/lang/StringBuilder;", constantPool);
  BytecodeGenerator::add_invoke_virtual(meth_idx,
                                        constantPool, result);

  // load second string
  result.push_back(BytecodeGenerator::ALOAD_2);

  // invokevirtual <Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder>
  BytecodeGenerator::add_invoke_virtual(meth_idx,
                                        constantPool, result);

  // invokevirtual <Method java/lang/StringBuilder.toString:()Ljava/lang/String>
  meth_idx = BytecodeGenerator::add_method("java/lang/StringBuilder", "toString", "()Ljava/lang/String;", constantPool);
  BytecodeGenerator::add_invoke_virtual(meth_idx,
                                        constantPool, result);
  globalstack_push(constantPool, result);*/
  BytecodeGenerator::localCount += 2;
}

void size_ByteCode(ConstantPool& constantPool, std::vector<char>& result,
                   Graphs::Node_ptr current_node)
{
  globalstack_pop(constantPool, result);
  // invokevirtual <Method java/lang/String.length:()I>
  uint16_t meth_idx = constantPool.str_idx.length_idx;
  if (meth_idx == 0 )
    meth_idx = BytecodeGenerator::add_method("java/lang/String", "length", "()I", constantPool);
  BytecodeGenerator::add_invoke_virtual(meth_idx,
                                        constantPool, result);

  ///TODO use wrapper class!! also wrap int size with valueOf to Integer (Author Zelldon)
}


// CALL
void call_ByteCode(ConstantPool& pool, std::vector<char>& code,
                   Graphs::Node_ptr current_node) {
}

// LIST OPERATIONS
void null_ByteCode(ConstantPool& pool, std::vector<char>& code,
                   Graphs::Node_ptr current_node) {
}
void list_push_ByteCode(ConstantPool& pool, std::vector<char>& code,
                        Graphs::Node_ptr current_node) {
}

void list_pop_ByteCode(ConstantPool& pool, std::vector<char>& code,
                       Graphs::Node_ptr current_node) {
}

// BOOLEAN ARITHMETIC
void greater_ByteCode(ConstantPool& pool, std::vector<char>& result,
                      Graphs::Node_ptr current_node) {
  // store the two integers and load them to get the right order
  globalstack_pop(pool, result);
  result.push_back(BytecodeGenerator::ISTORE_1);
  globalstack_pop(pool, result);
  result.push_back(BytecodeGenerator::ISTORE_2);
  result.push_back(BytecodeGenerator::ILOAD_1);
  result.push_back(BytecodeGenerator::ILOAD_2);

  std::vector<char> if_body;
  std::vector<char> goto_body;
  // represents the branch from 'goto' to the end
  std::vector<char> else_branch;

  goto_body.push_back(BytecodeGenerator::ICONST_0);
  BytecodeGenerator::add_conditional_with_instruction(BytecodeGenerator::GOTO,
                                                      &goto_body[0], else_branch);

  // it is necessary to push ICONST_1 before the goto-branch
  if_body.push_back(BytecodeGenerator::ICONST_1);
  if_body.insert(if_body.end(), else_branch.begin(), else_branch.end());

  BytecodeGenerator::add_conditional_with_instruction(BytecodeGenerator::IF_ICMPLE,
                                                      &if_body[0], result);

  BytecodeGenerator::localCount += 3;
}

void equal_ByteCode(ConstantPool& pool, std::vector<char>& result,
                    Graphs::Node_ptr current_node) {
  /*
   * TODO: "jump-bytes" hard coded, should be replaced by Chris' if-function later
   *        but actually not necessary, because in this case it is static
   * The bytecode represents the following Java code: boolean c = (a == b);
   */

  // store the two integers and load them to get the right order
  globalstack_pop(pool, result);
  result.push_back(BytecodeGenerator::ISTORE_1);
  globalstack_pop(pool, result);
  result.push_back(BytecodeGenerator::ISTORE_2);
  result.push_back(BytecodeGenerator::ILOAD_1);
  result.push_back(BytecodeGenerator::ILOAD_2);

  std::vector<char> if_body;
  std::vector<char> goto_body;
  // represents the branch from 'goto' to the end
  std::vector<char> else_branch;

  goto_body.push_back(BytecodeGenerator::ICONST_0);
  BytecodeGenerator::add_conditional_with_instruction(BytecodeGenerator::GOTO,
                                                      &goto_body[0], else_branch);

  // it is necessary to push ICONST_1 before the goto-branch
  if_body.push_back(BytecodeGenerator::ICONST_1);
  if_body.insert(if_body.end(), else_branch.begin(), else_branch.end());

  BytecodeGenerator::add_conditional_with_instruction(BytecodeGenerator::IF_ICMPNE,
                                                      &if_body[0], result);

  BytecodeGenerator::localCount += 3;
}

void false_ByteCode(ConstantPool& pool, std::vector<char>& code,
                    Graphs::Node_ptr current_node) {
  push_ByteCode(pool, code, current_node);
  //code.push_back(BytecodeGenerator::ICONST_0);
  //globalstack_push(pool, code);

  //BytecodeGenerator::localCount++;
}

void true_ByteCode(ConstantPool& pool, std::vector<char>& code,
                   Graphs::Node_ptr current_node) {
  push_ByteCode(pool, code, current_node);
  //code.push_back(BytecodeGenerator::ICONST_1);
  //globalstack_push(pool, code);
  //BytecodeGenerator::localCount++;
  //TODO localCount noch notwendig?
}

// IO OPERATIONS
void boom_ByteCode(ConstantPool& pool, std::vector<char>& code,
                   Graphs::Node_ptr current_node) {
}

void eof_ByteCode(ConstantPool& pool, std::vector<char>& code,
                  Graphs::Node_ptr current_node) {
}

void input_ByteCode(ConstantPool& pool, std::vector<char>& code,
                    Graphs::Node_ptr current_node) {
}

void underflow_ByteCode(ConstantPool& pool, std::vector<char>& code,
                        Graphs::Node_ptr current_node){
}

void type_ByteCode(ConstantPool& pool, std::vector<char>& code,
                   Graphs::Node_ptr current_node) {
}

//CONTROL STRUCTURE
void if_or_while_ByteCode(ConstantPool& pool, std::vector<char>& code,
                          Graphs::Node_ptr current_node) {
}
//VARIABLES

void pop_Variable(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void push_Variable(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}

std::vector<char> BytecodeGenerator::GenerateCodeFromFunctionGraph(Graphs::Graph_ptr graph,
                                                                   ConstantPool& constantPool) {
  BytecodeGenerator::localCount = 0;
  std::vector<char> result;
  Graphs::Node_ptr current_node(graph->start());
  while (current_node && current_node->command.type != Command::Type::FINISH) {
    func_ptr f = CODE_FUNC_MAPPING.at(current_node->command.type);
    if (f) {
      f(constantPool, result, current_node);
    }
    current_node = current_node->successor1;
  }
  result.push_back(BytecodeGenerator::RETURN);
  return result;
}

uint16_t get_stack_method_ref(ConstantPool& constant_pool, const std::string& method, const std::string& descriptor)
{
  uint16_t stack_class_utf8 = constant_pool.addString("java/util/ArrayDeque");
  uint16_t stack_class = constant_pool.addClassRef(stack_class_utf8);
  uint16_t stack_pop_utf8 = constant_pool.addString(method);
  uint16_t stack_pop_sig_utf8 = constant_pool.addString(descriptor);
  uint16_t stack_pop_name_type = constant_pool.addNameAndType(stack_pop_utf8, stack_pop_sig_utf8);
  return constant_pool.addMethRef(stack_class, stack_pop_name_type);
}

uint16_t get_stack_field_ref(ConstantPool& constant_pool)
{
  uint16_t our_class_utf8 = constant_pool.addString("Main");
  uint16_t our_class = constant_pool.addClassRef(our_class_utf8);
  uint16_t stack_utf8 = constant_pool.addString("stack");
  uint16_t stack_type_utf8 = constant_pool.addString("Ljava/util/ArrayDeque;");
  uint16_t stack_name_type = constant_pool.addNameAndType(stack_utf8, stack_type_utf8);
  return constant_pool.addFieldRef(our_class, stack_name_type);
}



void globalstack_pop(ConstantPool& constant_pool, std::vector<char>& code) {

  uint16_t field_system_idx = BytecodeGenerator::add_field("Main", "stack", "Ljava/util/ArrayDeque;", constant_pool);
  BytecodeGenerator::add_static_field(field_system_idx, constant_pool, code);

  uint16_t pop_idx = BytecodeGenerator::add_method("java/util/ArrayDeque", "pop", "()Ljava/lang/Object;", constant_pool);
  BytecodeGenerator::add_invoke_virtual(pop_idx, constant_pool, code);

}


void globalstack_push(ConstantPool& constant_pool, std::vector<char>& code) {

  /* Method ref in constant pool (stack.push()) */
  uint16_t stack_push = constant_pool.arr_idx.push_idx;
  /* Field ref in constant pool to this.stack */
  uint16_t stack_field = constant_pool.arr_idx.field_idx;

  /* Lazy init ref to stack.push() from constant pool */
  if (stack_push == 0)
    stack_push = get_stack_method_ref(constant_pool, "push", "(Ljava/lang/Object;)V");

  /* Lazy init ref to our stack field */
  if (stack_field == 0)
    stack_field = get_stack_field_ref(constant_pool);

  uint16_t push_idx = BytecodeGenerator::add_method("java/util/ArrayDeque", "push", "(Ljava/lang/Object;)V", constant_pool);
   BytecodeGenerator::add_invoke_virtual(push_idx, constant_pool, code);
}


