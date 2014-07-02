#include <backend/classfile/Lambda_classfile_writer.h>

Lambda_classfile_writer::Lambda_classfile_writer(ClassfileVersion version, ConstantPool* constantPool,
                    Graphs& graphs,
                    const std::map<std::string, codegen::Bytecode&> codeFunctions,
                    std::ostream* out) : ClassfileWriter(version, constantPool, graphs, codeFunctions, out)
{
  //constant_pool_ = std::make_shared<ConstantPool*>(new ConstantPool());
}

Lambda_classfile_writer::~Lambda_classfile_writer()
{

}


void Lambda_classfile_writer::WriteAccessFlags()
{

}

void Lambda_classfile_writer::WriteAttributes(const std::string& key)
{
  (void) key;

}

void Lambda_classfile_writer::WriteClassName()
{

}

void Lambda_classfile_writer::WriteClInitMethod()
{
}

void Lambda_classfile_writer::WriteConstantPool()
{

}

void Lambda_classfile_writer::WriteFields()
{

}

void Lambda_classfile_writer::WriteInitMethod()
{

}

void Lambda_classfile_writer::WriteMethods()
{

}
