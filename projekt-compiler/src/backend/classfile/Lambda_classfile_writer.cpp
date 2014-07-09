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


Lambda_classfile_writer::Lambda_classfile_writer(std::string& class_name, ClassfileVersion version,
                                 ConstantPool* constantPool,
                                 Graphs& graphs,
                                 const std::map<std::string, codegen::Bytecode&> codeFunctions,
                                 std::ostream* out) : Lambda_interface_writer(version, constantPool, graphs, codeFunctions, out),  class_name(class_name)  {

}

Lambda_classfile_writer::~Lambda_classfile_writer() {
}



void Lambda_classfile_writer::WriteConstantPool() {
  uint16_t idx = constant_pool_->addString(Lambda_interface_writer::lambda_class_name);
  constant_pool_->addClassRef(idx);
  constant_pool_->addString(enclosing_attr); //optional

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

  //TODO CODE ATTRIBUTE
  WriteAttributes(Lambda_interface_writer::method_name);

  /*public void closure();
      flags: ACC_PUBLIC
      Code:
        stack=2, locals=1, args_size=1
           0: getstatic     #2                  // Field java/lang/System.out:Ljava/io/PrintStream;
           3: ldc           #3                  // String Hello
           5: invokevirtual #4                  // Method java/io/PrintStream.print:(Ljava/lang/String;)V
           8: return
        LineNumberTable:
          line 40: 0
          line 41: 8
        LocalVariableTable:
          Start  Length  Slot  Name   Signature
                 0       9     0  this   LLambdaBytecode$1;
        org.netbeans.SourceLevelAnnotations: length = 0x6
         00 01 00 13 00 00 */
  //===============================================================

  // file attributes_count

  writer.writeU16(2);
  //ENCLOSING METHOD
  writer.writeU16(constant_pool_->addString(enclosing_attr));//Enclosing Method
  writer.writeU32(4);//Attr len
  std::string class_ = Env::getDstClassName();
  writer.writeU16(ClassfileWriter::get_class_ref(class_));
  //TODO SHOULD BE CHECKED FOR THE RIGHT METHOD!!!!!!!!
  uint16_t main_idx = constant_pool_->addString("main");
  uint16_t main_descr_idx = constant_pool_->addString("([Ljava/lang/String;)V");
  writer.writeU16(constant_pool_->addNameAndType(main_idx, main_descr_idx)); //ref in which method the lambda was created
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
  ClassfileWriter::WriteInitMethod();
}

void Lambda_classfile_writer::WriteClInitMethod() {
  ClassfileWriter::WriteClInitMethod();
}

void Lambda_classfile_writer::WriteAttributes(const std::string &key) {
  ClassfileWriter::WriteAttributes(key);
}
