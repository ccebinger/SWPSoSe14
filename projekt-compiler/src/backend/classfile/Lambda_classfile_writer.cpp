/*[--**--]
 * Copyright (C) 2014  SWPSoSe14Cpp Group
 *
 * This program is free software; you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this
 * program; if not, see <http://www.gnu.org/licenses/>.
 *
 */
 #include <backend/classfile/Lambda_classfile_writer.h>


const unsigned char Lambda_classfile_writer::anonymous_class_access_flags[] = {'\x00', '\x30'};


Lambda_classfile_writer::Lambda_classfile_writer(std::string& class_name, LocalVariableStash locals, ClassfileVersion version,
                                 ConstantPool* constantPool,
                                 Graphs& graphs,
                                 const std::map<std::string, codegen::Bytecode&> codeFunctions,
                                 std::ostream* out) : Lambda_interface_writer(version, constantPool, graphs, codeFunctions, out),  class_name(class_name), fields(locals)  {

}

Lambda_classfile_writer::~Lambda_classfile_writer() {
}



void Lambda_classfile_writer::WriteConstantPool() {
  uint16_t idx = constant_pool_->addString(Lambda_interface_writer::lambda_class_name);
  constant_pool_->addClassRef(idx);

  uint16_t main_idx = constant_pool_->addString("main");
  uint16_t main_descr_idx = constant_pool_->addString("([Ljava/lang/String;)V");
  constant_pool_->addNameAndType(main_idx, main_descr_idx); //ref in which method the lambda was created

  size_t size = constant_pool_->size() + 1;
  writer.writeU16(size);
  writer.writeVector(constant_pool_->getByteArray());
}

void Lambda_classfile_writer::WriteAccessFlags() {
  writer.write_array(2, anonymous_class_access_flags);
}

void Lambda_classfile_writer::WriteClassName() {
  writer.writeU16(constant_pool_->addClassRef(constant_pool_->addString(class_name)));
}


void Lambda_classfile_writer::WriteInterfaces() {
  //LAMBDA INTERFACE!!
  writer.writeU16(1); //interface count 1
  writer.writeU16(get_class_ref()); //lambda class idx
}


void Lambda_classfile_writer::WriteFields()
{
  //TODO add variables as fields !!!!


    ClassfileWriter::WriteFields();
}

void Lambda_classfile_writer::WriteMethods() {
  writer.writeU16(3); //closure + <init> + <cinit>
  WriteInitMethod();
  WriteClInitMethod();
  //===============================================================
  //CLOSURE METHOD
  writer.writeU16(1); //public
  writer.writeU16(constant_pool_->addString(Lambda_interface_writer::method_name)); //closure name idx
  writer.writeU16(constant_pool_->addString(Lambda_interface_writer::method_descriptor));//()V idx
  //CODE ATTRIBUTE
  WriteAttributes(Lambda_interface_writer::method_name);
  //===============================================================

  // file attributes_count
  writer.writeU16(1);
  //INNER CLASSES
  writer.writeU16(constant_pool_->addString(inner_classes_attr)); //innerclass
  writer.writeU32(10); //0000 000a attr length
  writer.writeU16(1); //0001 nr of classes
  writer.writeU16(ClassfileWriter::get_class_ref(class_name));
  writer.writeU16(0); //0000 outer class info idx
  writer.writeU16(0); //inner name idx
  writer.write_array(2, inner_class_flag);
}

void Lambda_classfile_writer::WriteInitMethod() {
  writer.writeU16(1);
  uint16_t obj_idx = constant_pool_->obj_idx.class_idx;
  uint16_t init_name_type_idx = constant_pool_->addNameAndType(constant_pool_->addString("<init>"), constant_pool_->addString("()V"));
  writer.writeU16(constant_pool_->addString("<init>"));
  writer.writeU16(constant_pool_->addString("()V"));
  /* WriteAttributes */
  // attribute_count=1
  writer.writeU16(1);
  writer.writeU16(constant_pool_->addString("Code"));
  uint32_t code_len = 5;

  if (fields.current_var_count() > 0)
    code_len += 5 * fields.current_var_count() + 1;
  //attribute length
  writer.writeU32(12 + code_len);
  // max_stack=1
  writer.writeU16(1);
  // max_locals=1
  writer.writeU16(1);
  // code_length=5
  writer.writeU32(code_len);
  //code source

  //TODO add fields from local variable stash
  //alods until field size
  std::map<std::string, uint8_t> map = fields.get_variable_to_index_map();
  if (map.size() > 0) {
    writer.writeU8(codegen::MNEMONIC::ALOAD_0);
    uint8_t idx = 1;
    for (std::map<std::string, uint8_t>::iterator it = map.begin(); it != map.end(); it++) {
      uint16_t field_name_idx = constant_pool_->addString(it->first);
      uint16_t field_idx = constant_pool_->addNameAndType(obj_idx, field_name_idx);
      writer.writeU8(codegen::MNEMONIC::ALOAD);
      writer.writeU8(idx++);
      writer.writeU8(codegen::MNEMONIC::PUTFIELD);
      writer.writeU16(field_idx);
    }
  }
  //
  writer.writeU8(codegen::MNEMONIC::ALOAD_0);
  writer.writeU8(codegen::MNEMONIC::INVOKE_SPECIAL);
  writer.writeU16(constant_pool_->addMethRef(constant_pool_->addClassRef(obj_idx), init_name_type_idx));
  writer.writeU8(codegen::RETURN);
  // exception_table_length=0
  writer.writeU16(0);
  // attributes_count
  writer.writeU16(0);
}

void Lambda_classfile_writer::WriteClInitMethod() {
  ClassfileWriter::WriteClInitMethod();
}

void Lambda_classfile_writer::WriteAttributes(const std::string &key) {
  ClassfileWriter::WriteAttributes(key);
}
