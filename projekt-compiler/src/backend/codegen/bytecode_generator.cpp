#include "backend/codegen/bytecode_generator.h"

#include <cstdint>


const std::map<Command::Type, BytecodeGenerator::func_ptr> BytecodeGenerator::CODE_FUNC_MAPPING =
{
  {Command::Type::OUTPUT, &output_ByteCode},
  {Command::Type::PUSH_CONST, &push_ByteCode},
  {Command::Type::ADD, &add_ByteCode},
  {Command::Type::SUB, &sub_ByteCode},
  {Command::Type::MULT, &mult_ByteCode},
  {Command::Type::DIV, &div_ByteCode},
  {Command::Type::MOD, &mod_ByteCode},
  {Command::Type::SIZE, &size_ByteCode}
};


void push_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  uint16_t indexInPool;
  // ldc indexInPool
  indexInPool = constantPool.addString(current_node->command.arg);
  result.push_back(BytecodeGenerator::LDC);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
}
void add_ByteCode(ConstantPool& pool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::IADD);  //iadd
}
void sub_ByteCode(ConstantPool& pool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::ISUB);  //isub
}
void mult_ByteCode(ConstantPool& pool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::IMULT);  //imul
}
void div_ByteCode(ConstantPool& pool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::IDIV);  //idiv
}
void mod_ByteCode(ConstantPool& pool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::IREM); //irem
}

void output_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  uint16_t indexInPool;
  // astore_1
  result.push_back(BytecodeGenerator::ASTORE_1);

  // getstatic <Field java/lang/System.out:Ljava/io/PrintStream;>
  indexInPool = constantPool.addFieldRef("java/lang/System.out:Ljava/io/PrintStream;");
  result.push_back(BytecodeGenerator::GET_STATIC);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);

  // aload_1
  result.push_back(BytecodeGenerator::ALOAD_1);

  // invokevirtual <Method java/io/PrintStream.print:(Ljava/lang/String;)V>
  indexInPool = constantPool.addMethRef("java/io/PrintStream.print:(Ljava/lang/String;)V");
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
}


void size_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  uint16_t indexInPool;

  // invokevirtual <Method java/lang/String.length:()I>
  indexInPool 	= constantPool.addMethRef("java/lang/String.length:()I");
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
}

void cut_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{
  code.push_back(BytecodeGenerator::ISTORE_0); //istore_0 to store the index for the cut
  code.push_back(BytecodeGenerator::ICONST_0); //iconst_0 for the begin of the string
  code.push_back(BytecodeGenerator::ILOAD_0); //iload_0 to add the index until the cut should happend

  uint16_t index = pool.addMethRef("java/util/String.substring:(II)Ljava/lang/String"); //import substring method
  code.push_back(BytecodeGenerator::INVOKE_VIRTUAL); //invokevirtual
  code.push_back((index & 0xFF00U) >> 8);
  code.push_back(index & 0x00FFU);
}

std::vector<char> BytecodeGenerator::GenerateCodeFromFunctionGraph(Graphs::Graph_ptr graph,
                               ConstantPool& constantPool) {
  std::vector<char> result;
  Graphs::Node_ptr current_node(graph->start());
  while(current_node && current_node->command.type != Command::Type::FINISH) {
    uint16_t indexInPool;
    func_ptr f = CODE_FUNC_MAPPING.at(current_node->command.type);
    if (f) {
      f(constantPool, result, current_node);
    }
    current_node = current_node->successor1;
  }
  // Emit
  // return
  result.push_back(BytecodeGenerator::RETURN);
  return result;
}
