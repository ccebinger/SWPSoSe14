#include <backend/codegen/bytecode_generator.h>
#include <cstdint>



const char BytecodeGenerator::ILOAD_0 = '\x1a';
const char BytecodeGenerator::ICONST_0 = '\x03';
const char BytecodeGenerator::ICONST_1 = '\x04';
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

void BytecodeGenerator::add_invoke_virtual(const std::string& method, ConstantPool& constantPool, std::vector<char>& result)
{
  result.push_back(BytecodeGenerator::INVOKE_VIRTUAL);
  add_index(constantPool.addMethRef(method), result);
}

void BytecodeGenerator::add_index(uint16_t indexInPool, std::vector<char>& result)
{
  result.push_back((indexInPool & 0xFF00U) >> 8);
  result.push_back(indexInPool & 0x00FFU);
}

void BytecodeGenerator::add_static_field(const std::string& field, ConstantPool& constantPool, std::vector<char>& result)
{
  result.push_back(BytecodeGenerator::GET_STATIC);
  add_index(constantPool.addFieldRef(field), result);
}
void BytecodeGenerator::add_new_object(const std::string& class_name, ConstantPool& constantPool, std::vector<char>& result)
{
  result.push_back(BytecodeGenerator::NEW);
  add_index(constantPool.addClassRef(class_name), result);
}

void output_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  // astore_1
  result.push_back(BytecodeGenerator::ASTORE_1);

  // get <Field java/lang/System.out:Ljava/io/PrintStream;>
  BytecodeGenerator::add_static_field("java/lang/System.out:Ljava/io/PrintStream;", constantPool, result);

  // aload_1
  result.push_back(BytecodeGenerator::ALOAD_1);

  // invokevirtual <Method java/io/PrintStream.print:(Ljava/lang/String;)V>
  BytecodeGenerator::add_invoke_virtual("java/io/PrintStream.print:(Ljava/lang/String;)V", constantPool, result);
}

void push_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  // ldc indexInPool
  result.push_back(BytecodeGenerator::LDC);
  BytecodeGenerator::add_index(constantPool.addString(current_node->command.arg), result);
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

  BytecodeGenerator::add_invoke_virtual("java/lang/String.substring:(II)Ljava/lang/String;", constantPool, result);

  result.push_back(BytecodeGenerator::ALOAD_1); //to get begin string
  result.push_back(BytecodeGenerator::ILOAD_0); //index for cut

  BytecodeGenerator::add_invoke_virtual("java/lang/String.substring:(I)Ljava/lang/String;", constantPool, result);
}

void append_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  // initial situation: the two strings are on the stacks
  result.push_back(BytecodeGenerator::ASTORE_1); //astore_1 to store the first string
  result.push_back(BytecodeGenerator::ASTORE_2); //astore_2 to store the second string

  // create new object of class java/lang/StringBuilder
  BytecodeGenerator::add_new_object("java/lang/StringBuilder", constantPool, result);
  // duplicate object
  result.push_back(BytecodeGenerator::DUP);

  // init StringBuilder
  BytecodeGenerator::add_invoke_virtual("java/lang/StringBuilder.'<init>':()V", constantPool, result);
  // load first string
  result.push_back(BytecodeGenerator::ALOAD_1);

  // invokevirtual <Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder>
  BytecodeGenerator::add_invoke_virtual("java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;", constantPool, result);

  // load second string
  result.push_back(BytecodeGenerator::ALOAD_2);

  // invokevirtual <Method java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder>
  BytecodeGenerator::add_invoke_virtual("java/lang/StringBuilder.append:(Ljava/lang/String;)Ljava/lang/StringBuilder;", constantPool, result);

  // invokevirtual <Method java/lang/StringBuilder.toString:()Ljava/lang/String>
  BytecodeGenerator::add_invoke_virtual("java/lang/StringBuilder.toString:()Ljava/lang/String;", constantPool, result);
}

void size_ByteCode(ConstantPool& constantPool, std::vector<char>& result, Graphs::Node_ptr current_node)
{
  // invokevirtual <Method java/lang/String.length:()I>
  BytecodeGenerator::add_invoke_virtual("java/lang/String.length:()I", constantPool, result);
}


//CALL
void call_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
// LIST OPERATIONS
void null_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void list_push_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void list_pop_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
//BOOLEAN ARITHMETIC
void false_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{
  code.push_back(BytecodeGenerator::ICONST_0);
}
void greater_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void equal_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void true_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{
  code.push_back(BytecodeGenerator::ICONST_1);
}
//IO OPERATIONS
void boom_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void eof_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void input_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void underflow_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
void type_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

}
//CONTROL STRUCTURE
void if_or_while_ByteCode(ConstantPool& pool, std::vector<char>& code, Graphs::Node_ptr current_node)
{

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
