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

Lambda_classfile_writer::Lambda_classfile_writer(ClassfileVersion version,
                                 ConstantPool* constantPool,
                                 Graphs& graphs,
                                 const std::map<std::string, codegen::Bytecode&> codeFunctions,
                                 std::ostream* out) : Lambda_interface_writer(version, constantPool, graphs, codeFunctions, out) {

}

Lambda_classfile_writer::~Lambda_classfile_writer() {
}



void Lambda_classfile_writer::WriteConstantPool() {
  uint16_t idx = constant_pool_->addString(Lambda_interface_writer::lambda_class_name);
  constant_pool_->addClassRef(idx);
  constant_pool_->addString("InnerClasses");
  constant_pool_->addString("EnclosingMethod"); //optional


  size_t size = constant_pool_->size() + 1;
  writer.writeU16(size);
  writer.writeVector(constant_pool_->getByteArray());
}

void Lambda_classfile_writer::WriteAccessFlags() {
  //TODO
  //flags: ACC_FINAL, ACC_SUPER
  ClassfileWriter::WriteAccessFlags();
}

void Lambda_classfile_writer::WriteClassName() {
  writer.writeU16(constant_pool_->addClassRef(constant_pool_->addString(Env::getDstClassName())));
}


void Lambda_classfile_writer::WriteInterfaces() {
  //LAMBDA INTERFACE!!
}


void Lambda_classfile_writer::WriteMethods() {
  std::vector<std::string> keys = this->graphs_.keyset();
  size_t size = keys.size();
  writer.writeU16(size+2);
  WriteInitMethod();
  WriteClInitMethod();

  //===============================================================

  //TODO
  //CLOSURE METHOD
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
//    out_->write(ClassfileWriter::kNotRequired, sizeof (ClassfileWriter::kNotRequired));
  writer.writeU8(0);
}

void Lambda_classfile_writer::WriteInitMethod() {
  ClassfileWriter::WriteInitMethod();
}

void Lambda_classfile_writer::WriteAttributes(const std::string &key) {
  //TODO
  /*  EnclosingMethod: #23.#24                // LambdaBytecode.main
  InnerClasses:
       static #5; //class LambdaBytecode$1
       */
  ClassfileWriter::WriteAttributes(key);
}
