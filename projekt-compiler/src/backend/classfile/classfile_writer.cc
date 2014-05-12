#include <backend/classfile/classfile_writer.h>

#include <array>
#include <iostream>
#include <map>
#include <string>

const char ClassfileWriter::kMagicNumber[] { '\xCA', '\xFE',
      '\xBA', '\xBE' };
const char ClassfileWriter::kNotRequired[] { '\x00', '\x00' };
std::map<std::string, int> collection = {{"bear", 4},
                                         {"cassowary", 2},
                                         {"tiger", 7}};

std::map<ClassfileWriter::ClassfileVersion, const std::array<const char, 4>>
    ClassfileWriter::kVersionNumbers {
  { ClassfileWriter::ClassfileVersion::JAVA_7, {'\x00', '\x00', '\x00', '\x33'}}
};

ClassfileWriter::ClassfileWriter(ClassfileVersion version,
                                 const ConstantPool& constantPool,
                                 std::ostream& out) :
    out_(out), version_(version), constant_pool_(constantPool) {
}

ClassfileWriter::~ClassfileWriter() {
}

void ClassfileWriter::WriteClassfile() {
  WriteMagicNumber();
  WriteVersionNumber();
  WriteConstantPool();
  WriteAccessFlags();
  WriteClassName();
  WriteSuperClassName();
  WriteInterfaces();
  WriteFields();
  WriteMethods();
  WriteAttributes();
}

void ClassfileWriter::WriteMagicNumber() {
  out_.write(kMagicNumber, sizeof(kMagicNumber));
}

void ClassfileWriter::WriteVersionNumber() {
  out_.write(kVersionNumbers[version_].data(),
             sizeof(kVersionNumbers[version_].data()));
}

void ClassfileWriter::WriteConstantPool() {
  // TODO
}

void ClassfileWriter::WriteAccessFlags() {
  // TODO
}

void ClassfileWriter::WriteClassName() {
  // TODO
}

void ClassfileWriter::WriteSuperClassName() {
  // TODO
}

void ClassfileWriter::WriteInterfaces() {
  out_.write(kNotRequired, sizeof(kNotRequired));
}

void ClassfileWriter::WriteFields() {
  out_.write(kNotRequired, sizeof(kNotRequired));
}

void ClassfileWriter::WriteMethods() {
  // TODO
}

void ClassfileWriter::WriteAttributes() {
  // TODO
}
