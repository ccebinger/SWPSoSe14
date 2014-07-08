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



  size_t size = constant_pool_->size() + 1;
  writer.writeU16(size);
  writer.writeVector(constant_pool_->getByteArray());
}

void Lambda_classfile_writer::WriteAccessFlags() {
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

  //===============================================================


  // file attributes_count
//    out_->write(ClassfileWriter::kNotRequired, sizeof (ClassfileWriter::kNotRequired));
  writer.writeU8(0);
}

void Lambda_classfile_writer::WriteInitMethod() {
  ClassfileWriter::WriteInitMethod();
}

void Lambda_classfile_writer::WriteAttributes(const std::string &key) {
  ClassfileWriter::WriteAttributes(key);
}
