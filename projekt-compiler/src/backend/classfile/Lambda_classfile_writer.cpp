#include <backend/classfile/Lambda_classfile_writer.h>

//ACC_PUBLIC 00 01
//ACC_INTERFACE 0200
//ACC_ABSTRACT 0400
const uint8_t Lambda_classfile_writer::access_flags[] = {0x06, 0x01};
const std::string Lambda_classfile_writer::lambda_class_name = "Lambda";

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
  size_t len = sizeof(access_flags)/sizeof(access_flags[0]);
  for (size_t i = 0; i < len; i++)
    writer.writeU8(access_flags[i]);
}

void Lambda_classfile_writer::WriteAttributes(const std::string& key)
{
  (void) key;

}

void Lambda_classfile_writer::WriteClassName()
{
   writer.writeU16(constant_pool_->addClassRef(constant_pool_->addString(lambda_class_name)));
}

void Lambda_classfile_writer::WriteClInitMethod()
{
  writer.writeU16(0);
}

void Lambda_classfile_writer::WriteConstantPool()
{
  //STRINGS
  constant_pool_->obj_idx.class_idx = constant_pool_->addString("java/lang/Object");
  size_t idx = constant_pool_->addString(lambda_class_name);
  constant_pool_->addString(lambda_class_name + ".java");
  constant_pool_->addString("()V");
  constant_pool_->addString("closure");
  constant_pool_->addClassRef(idx); //Lambda class
  constant_pool_->addClassRef(constant_pool_->obj_idx.class_idx);


  size_t size = constant_pool_->size() + 1;
  writer.writeU16(size);
  writer.writeVector(constant_pool_->getByteArray());
}

void Lambda_classfile_writer::WriteFields()
{
  writer.writeU16(0);
}

void Lambda_classfile_writer::WriteInitMethod()
{
  writer.writeU16(0);
}

void Lambda_classfile_writer::WriteMethods()
{

}
