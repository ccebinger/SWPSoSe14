#include "backend/codegen/bytecode_generator.h"

#include <cstdint>

std::vector<char> BytecodeGenerator::GenerateCodeFromFunctionGraph(Graphs::Graph_ptr graph,
                               ConstantPool& constantPool) {
  std::vector<char> result;
  std::shared_ptr<Node> currentNode(graph->start());
  while(currentNode && currentNode->command.type != Command::Type::FINISH) {
    uint16_t indexInPool;

    switch(currentNode->command.type) {
    case Command::Type::PUSH_CONST:
      // Emit
      // ldc indexInPool
      indexInPool = constantPool.addString(currentNode->command.arg);
      result.push_back('\x12');
      result.push_back((indexInPool & 0xFF00U) >> 8);
      result.push_back(indexInPool & 0x00FFU);
    break;
    case Command::Type::OUTPUT:
      // Emit
      // astore_1
      result.push_back('\x4c');

      // getstatic <Field java/lang/System.out:Ljava/io/PrintStream;>
      indexInPool = constantPool.addFieldRef("java/lang/System.out:Ljava/io/PrintStream;");
      result.push_back('\xb2');
      result.push_back((indexInPool & 0xFF00U) >> 8);
      result.push_back(indexInPool & 0x00FFU);

      // aload_1
      result.push_back('\x2b');

      // invokevirtual <Method java/io/PrintStream.print:(Ljava/lang/String;)V>
      indexInPool = constantPool.addMethRef("java/io/PrintStream.print:(Ljava/lang/String;)V");
      result.push_back('\xb6');
      result.push_back((indexInPool & 0xFF00U) >> 8);
      result.push_back(indexInPool & 0x00FFU);
    break;
    case Command::Type::ADD:
      result.push_back('\x60');  //iadd
      break;
    case Command::Type::SUB:
		  result.push_back('\x64');  //isub
      break;
    case Command::Type::MULT:
		  result.push_back('\x68');  //imul
		break;
    case Command::Type::DIV:
      result.push_back('/x6c');  //idiv
		break;
    case Command::Type::MOD:
      result.push_back('/x70'); //irem
		break;
    case Command::Type::CUT:
		  // Emit
		  //
		// TODO: MS2
		break;
    case Command::Type::APPEND:
		  // Emit
		  //
		// TODO: MS2
		break;
    case Command::Type::SIZE:
		  // Emit
		  //
		// TODO: MS2
		break;
    default:
    // TODO errors etc.
    break;
    }

    currentNode = currentNode->successor1;
  }
  // Emit
  // return
  result.push_back('\xb1');
  return result;
}
