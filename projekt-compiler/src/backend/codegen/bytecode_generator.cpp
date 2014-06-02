#include <backend/codegen/bytecode_generator.h>
#include <cstdint>



const char BytecodeGenerator::ILOAD_0 = '\x1a';
const char BytecodeGenerator::ICONST_0 = '\x03';
const char BytecodeGenerator::ISTORE_0 = '\x3b';
const char BytecodeGenerator::ALOAD_1 = '\x2b';
const char BytecodeGenerator::ALOAD_2 = '\x2c';
const char BytecodeGenerator::ASTORE_1 = '\x4c';
const char BytecodeGenerator::ASTORE_2 = '\x4d';
const char BytecodeGenerator::NEW = '\xbb';
const char BytecodeGenerator::INVOKE_VIRTUAL = '\xb6';
const char BytecodeGenerator::GET_STATIC = '\xb2';
const char BytecodeGenerator::RETURN = '\xb1';
const char BytecodeGenerator::LDC = '\x12';
const char BytecodeGenerator::DUP = '\x59';
const char BytecodeGenerator::IADD = '\x60';
const char BytecodeGenerator::ISUB = '\x64';
const char BytecodeGenerator::IMULT = '\x68';
const char BytecodeGenerator::IDIV = '\x6c';
const char BytecodeGenerator::IREM = '\x70';

const std::map<Command::Type, BytecodeGenerator::func_ptr> BytecodeGenerator::CODE_FUNC_MAPPING =
{
  {Command::Type::OUTPUT, &output_ByteCode},
  {Command::Type::PUSH_CONST, &push_ByteCode},
  {Command::Type::ADD, &add_ByteCode},
  {Command::Type::SUB, &sub_ByteCode},
  {Command::Type::MULT, &mult_ByteCode},
  {Command::Type::DIV, &div_ByteCode},
  {Command::Type::MOD, &mod_ByteCode},
  {Command::Type::CUT, &cut_ByteCode},
  {Command::Type::APPEND, &append_ByteCode},
  {Command::Type::SIZE, &size_ByteCode}
};

void output_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{

  // astore_1
  result.push_back(BytecodeGenerator::ASTORE_1);

  // get <Field java/lang/System.out:Ljava/io/PrintStream;>
  uint16_t indexInPool = constantPool.addFieldRef("java/lang/System.out:Ljava/io/PrintStream;");
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

void push_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  // ldc indexInPool
  uint16_t indexInPool = constantPool.addString(current_node->command.arg);
  result.push_back(BytecodeGenerator::LDC);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
}
void add_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::IADD);  //iadd
}
void sub_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::ISUB);  //isub
}
void mult_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::IMULT);  //imul
}
void div_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::IDIV);  //idiv
}
void mod_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::IREM); //irem
}
void cut_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  result.push_back(BytecodeGenerator::ISTORE_0); //istore_0 to store the index for the cut
  result.push_back(BytecodeGenerator::ASTORE_1); //astore_1 to store the begin string
  result.push_back(BytecodeGenerator::ALOAD_1); //load begin string
  result.push_back(BytecodeGenerator::ICONST_0); //iconst_0 for the begin of the string
  result.push_back(BytecodeGenerator::ILOAD_0); //iload_0 to add the index until the cut should happend

  uint16_t indexInPool = constantPool.addMethRef("java/lang/String.substring:(II)Ljava/lang/String;"); //import substring method
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL); //invokevirtual String.substring(from, to)
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);

  result.push_back(BytecodeGenerator::ALOAD_1); //to get begin string
  result.push_back(BytecodeGenerator::ILOAD_0); //index for cut

  indexInPool = constantPool.addIMethRef("java/lang/String.substring:(I)Ljava/lang/String;");
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL); //invokevirtual String.substring(from);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);

}

void append_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  // initial situation: the two strings are on the stacks
  result.push_back(BytecodeGenerator::ASTORE_1); //astore_1 to store the first string
  result.push_back(BytecodeGenerator::ASTORE_2); //astore_2 to store the second string

  // create new object of class java/lang/StringBuilder
  uint16_t indexInPool = constantPool.addClassRef("java/lang/StringBuilder");
  result.push_back(BytecodeGenerator::NEW);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);

  // duplicate object
  result.push_back(BytecodeGenerator::DUP);

  // init StringBuilder
  indexInPool = constantPool.addMethRef("java/lang/StringBuilder.'<init>':()V");
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);

  // load first string
  result.push_back(BytecodeGenerator::ALOAD_1);

  // invokevirtual <Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder>
  indexInPool = constantPool.addMethRef("java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;");
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);

  // load second string
  result.push_back(BytecodeGenerator::ALOAD_2);

  // invokevirtual <Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder>
  indexInPool = constantPool.addMethRef("java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;");
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);

  // invokevirtual <Method java/lang/StringBuilder.toString:()Ljava/lang/String>
  indexInPool = constantPool.addMethRef("java/lang/StringBuilder.toString:()Ljava/lang/String;");
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
}

void size_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  // invokevirtual <Method java/lang/String.length:()I>
  uint16_t indexInPool 	= constantPool.addMethRef("java/lang/String.length:()I");
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL);
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
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
