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
      indexInPool = constantPool.addString(currentNode->command.arg);
      // Emit
      // ldc indexInPool
      result.push_back('\x12');
      result.push_back((indexInPool & 0xFF00U) >> 8);
      result.push_back(indexInPool & 0x00FFU);
    break;
    case Command::Type::OUTPUT:
      // Emit
      //
    // TODO
    break;
    case Command::Type::ADD:
          // Emit
          //
        // TODO: MS2
        break;
    case Command::Type::SUB:
		  // Emit
		  //
		// TODO: MS2
		break;
    case Command::Type::MULT:
		  // Emit
		  //
		// TODO: MS2
		break;
    case Command::Type::DIV:
		  // Emit
		  //
		// TODO: MS2
		break;
    case Command::Type::MOD:
		  // Emit
		  //
		// TODO: MS2
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
  }
  // Emit
  // return
  result.push_back('\xb1');
  return result;
}
