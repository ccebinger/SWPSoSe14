#include <backend/classfile/Lambda_interface_writer.h>

//ACC_PUBLIC 00 01
//ACC_INTERFACE 0200
//ACC_ABSTRACT 0400
const unsigned char Lambda_interface_writer::interface_access_flags[] = {'\x06', '\x01'};
const unsigned char Lambda_interface_writer::method_access_flags[] = {'\x04', '\x01'};
const std::string Lambda_interface_writer::lambda_class_name = "Lambda";
const std::string Lambda_interface_writer::method_name = "closure";
const std::string Lambda_interface_writer::method_descriptor = "()V";

Lambda_interface_writer::Lambda_interface_writer(ClassfileVersion version, ConstantPool* constantPool,
                    Graphs& graphs,
                    const std::map<std::string, codegen::Bytecode&> codeFunctions,
                    std::ostream* out) : ClassfileWriter(version, constantPool, graphs, codeFunctions, out)
{
 lambda_file_name = Env::getDstClassfile() + Lambda_interface_writer::lambda_class_name + ".class";
  //constant_pool_ = std::make_shared<ConstantPool*>(new ConstantPool());
}

Lambda_interface_writer::~Lambda_interface_writer()
{

}


void Lambda_interface_writer::WriteAccessFlags()
{
  writer.write_array(sizeof interface_access_flags / sizeof interface_access_flags[0], interface_access_flags);
}

void Lambda_interface_writer::WriteAttributes(const std::string& key)
{
  (void) key;
  // attributes_count
  writer.writeU16(0);
}

void Lambda_interface_writer::WriteClassName()
{
   writer.writeU16(get_class_ref());
}

size_t Lambda_interface_writer::get_class_ref()
{
  return ClassfileWriter::get_class_ref(lambda_class_name);
}


void Lambda_interface_writer::WriteClInitMethod()
{
  writer.writeU16(0);
}

void Lambda_interface_writer::WriteFields()
{
  writer.writeU16(0);
}

void Lambda_interface_writer::WriteInitMethod()
{
  writer.writeU16(0);
}

void Lambda_interface_writer::WriteMethods()
{
  writer.writeU16(1);//method count
  writer.write_array(sizeof method_access_flags / sizeof method_access_flags[0] ,method_access_flags);
  writer.writeU16(constant_pool_->addString(method_name));
  writer.writeU16(constant_pool_->addString(method_descriptor));
  writer.writeU16(0);//method attr count
  writer.writeU16(0);//global attr count
}

