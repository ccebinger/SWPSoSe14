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
  {Command::Type::SOUTHJUNC, &if_or_while_ByteCode}
};

  int BytecodeGenerator::localCount = 0;

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

void BytecodeGenerator::add_invoke_virtual(const std::string& method,
                                           ConstantPool& constantPool,
                                           std::vector<char>& result) {
  add_invoke_method(BytecodeGenerator::INVOKE_VIRTUAL, method,
                    constantPool, result);
}

void BytecodeGenerator::add_invoke_static(const std::string& method,
                                          ConstantPool& constantPool,
                                          std::vector<char>& result) {
  add_invoke_method(BytecodeGenerator::INVOKE_STATIC, method,
                    constantPool, result);
}

void BytecodeGenerator::add_invoke_method(BytecodeGenerator::MNEMONIC opcode,
                                          const std::string& method,
                                          ConstantPool& constantPool,
                                          std::vector<char>& result) {
  result.push_back(opcode);
  // add_index(constantPool.addMethRef(method), result);
}

void BytecodeGenerator::add_index(uint16_t indexInPool,
                                  std::vector<char>& result) {
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
}

void BytecodeGenerator::add_static_field(const std::string& field,
                                         ConstantPool& constantPool,
                                         std::vector<char>& result) {
  result.push_back(BytecodeGenerator::GET_STATIC);
  // add_index(constantPool.addFieldRef(field), result);
}

void BytecodeGenerator::add_new_object(const std::string& class_name,
                                       ConstantPool& constantPool,
                                       std::vector<char>& result) {
  result.push_back(BytecodeGenerator::NEW);
  //  add_index(constantPool.addClassRef(class_name), result);
}

void BytecodeGenerator::add_class(const std::string& class_name,
                                  ConstantPool& constantPool,
                                  std::vector<char>& result) {
  // add_index(constantPool.addClassRef(class_name), result);
}

void BytecodeGenerator::add_instance_of(const std::string& class_name,
                                        ConstantPool& constantPool,
                                        std::vector<char>& result) {
  result.push_back(BytecodeGenerator::INSTANCE_OF);
  add_class(class_name, constantPool, result);
}

void BytecodeGenerator::add_type_check(const std::string& class_name,
                                       ConstantPool& constantPool,
                                       std::vector<char>& result) {
  add_instance_of(class_name, constantPool, result);

  std::vector<char> body;
  add_throw_exception("java/lang/IllegalArgumentException", constantPool, body);

  add_conditional_with_instruction(BytecodeGenerator::IFNE, &body[0], result);
}

void BytecodeGenerator::add_throw_exception(const std::string& class_name,
                                            ConstantPool& constantPool,
                                            std::vector<char>& result) {
  add_new_object(class_name, constantPool, result);
  result.push_back(BytecodeGenerator::ATHROW);
}

void output_ByteCode(ConstantPool& constantPool,
                     std::vector<char>& result,
                     Graphs::Node_ptr current_node) {
  // astore_1
  result.push_back(BytecodeGenerator::ASTORE_1);

  // get <Field java/lang/System.out:Ljava/io/PrintStream;>
  BytecodeGenerator::add_static_field("java/lang/System.out:Ljava/io/PrintStream;",
                                      constantPool, result);

  // aload_1
  result.push_back(BytecodeGenerator::ALOAD_1);

  // invokevirtual <Method java/io/PrintStream.print:(Ljava/lang/String;)V>
  BytecodeGenerator::add_invoke_virtual("java/io/PrintStream.print:(Ljava/lang/String;)V",
                                        constantPool, result);

  BytecodeGenerator::localCount++;
}

void push_ByteCode(ConstantPool& constantPool,
                   std::vector<char>& result,
                   Graphs::Node_ptr current_node) {
  // ldc indexInPool
  result.push_back(BytecodeGenerator::LDC);
  std::string value = current_node->command.arg;
  try {
    int int_val = std::stoi(value);
    BytecodeGenerator::add_index(constantPool.addInt(int_val), result);
  }
  // the value is a string
  catch (const std::invalid_argument& ia) {
    uint16_t string_idx = constantPool.addString(current_node->command.arg);
    uint16_t const_idx = constantPool.addConstString(string_idx);
    // BytecodeGenerator::add_index(string_idx, result);
  }
}

void add_integer_calculation(BytecodeGenerator::MNEMONIC calculation,
                             ConstantPool& constantPool,
                             std::vector<char>& result) {
  std::string integer_class = "java/lang/Integer";
  std::string integer_class_intValue_method = "java/lang/Integer.intValue:()I";
  std::string integer_class_static_value_of_method =
      "java/lang/Integer.valueOf:(I)Ljava/lang/Integer;";

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
  // istore_0 to store the index for the cut
  result.push_back(BytecodeGenerator::ISTORE_0);
  // astore_1 to store the begin string
  result.push_back(BytecodeGenerator::ASTORE_1);
  // load begin string
  result.push_back(BytecodeGenerator::ALOAD_1);
  // iconst_0 for the begin of the string
  result.push_back(BytecodeGenerator::ICONST_0);
  // iload_0 to add the index until the cut should happend
  result.push_back(BytecodeGenerator::ILOAD_0);

  BytecodeGenerator::add_invoke_virtual("java/lang/String.substring:(II)Ljava/lang/String;",
                                        constantPool, result);
  // to get begin string
  result.push_back(BytecodeGenerator::ALOAD_1);
  // index for cut
  result.push_back(BytecodeGenerator::ILOAD_0);

  BytecodeGenerator::add_invoke_virtual("java/lang/String.substring:(I)Ljava/lang/String;",
                                        constantPool, result);

  BytecodeGenerator::localCount += 2;
}

void append_ByteCode(ConstantPool& constantPool,
                     std::vector<char>& result,
                     Graphs::Node_ptr current_node) {
  // initial situation: the two strings are on the stacks
  // astore_1 to store the first string
  result.push_back(BytecodeGenerator::ASTORE_1);
  // astore_2 to store the second string
  result.push_back(BytecodeGenerator::ASTORE_2);

  // create new object of class java/lang/StringBuilder
  BytecodeGenerator::add_new_object("java/lang/StringBuilder",
                                    constantPool, result);
  // duplicate object
  result.push_back(BytecodeGenerator::DUP);

  // init StringBuilder
  BytecodeGenerator::add_invoke_virtual("java/lang/StringBuilder.'<init>':()V",
                                        constantPool, result);
  // load first string
  result.push_back(BytecodeGenerator::ALOAD_1);

  // invokevirtual <Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder>
  BytecodeGenerator::add_invoke_virtual("java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;",
                                        constantPool, result);

  // load second string
  result.push_back(BytecodeGenerator::ALOAD_2);

  // invokevirtual <Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder>
  BytecodeGenerator::add_invoke_virtual("java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;",
                                        constantPool, result);

  // invokevirtual <Method java/lang/StringBuilder.toString:()Ljava/lang/String>
  BytecodeGenerator::add_invoke_virtual("java/lang/StringBuilder.toString:()Ljava/lang/String;",
                                        constantPool, result);

  BytecodeGenerator::localCount += 2;
}

void size_ByteCode(ConstantPool& constantPool, std::vector<char>& result,
                   Graphs::Node_ptr current_node)
{
  // invokevirtual <Method java/lang/String.length:()I>
  BytecodeGenerator::add_invoke_virtual("java/lang/String.length:()I",
                                        constantPool, result);
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
void false_ByteCode(ConstantPool& pool, std::vector<char>& code,
                    Graphs::Node_ptr current_node) {
  code.push_back(BytecodeGenerator::ICONST_0);

  BytecodeGenerator::localCount++;
}

void greater_ByteCode(ConstantPool& pool, std::vector<char>& result,
                      Graphs::Node_ptr current_node) {
  // store the two integers and load them to get the right order
  result.push_back(BytecodeGenerator::ISTORE_1);
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
  result.push_back(BytecodeGenerator::ISTORE_1);
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

void true_ByteCode(ConstantPool& pool, std::vector<char>& code,
                   Graphs::Node_ptr current_node) {
  code.push_back(BytecodeGenerator::ICONST_1);
  BytecodeGenerator::localCount++;
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
