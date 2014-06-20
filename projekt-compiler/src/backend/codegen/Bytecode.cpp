#include <backend/codegen/Bytecode.h>

codegen::Bytecode::Bytecode(ConstantPool& p) : pool(p) {}

codegen::Bytecode::~Bytecode() {}


codegen::Bytecode* codegen::Bytecode::build(Graphs::Graph_ptr graph)
{
  return this;
}
