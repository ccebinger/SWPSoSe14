#include <backend/classfile/classfile_writer.h>
#include <backend/classfile/constant_pool.h>

#include <array>
#include <iostream>
#include <map>
#include <string>

const char ClassfileWriter::kMagicNumber[] { '\xCA', '\xFE',
      '\xBA', '\xBE' };
const char ClassfileWriter::kNotRequired[] { '\x00', '\x00' };
const char ClassfileWriter::kPublicAccessFlag[] { '\x00', '\x00','\x00', '\x01' };

std::map<ClassfileWriter::ClassfileVersion, const std::array<const char, 4>>
    ClassfileWriter::kVersionNumbers {
  { ClassfileWriter::ClassfileVersion::JAVA_7,
        std::array<const char, 4>{'\x00', '\x00', '\x00', '\x33'}}
};

ClassfileWriter::ClassfileWriter(ClassfileVersion version,
                                 ConstantPool& constantPool,
                                 const std::map<std::string,
                                   std::vector<char>&> codeFunctions,
                                 std::ostream& out) :
    out_(out), version_(version), constant_pool_(constantPool),
    code_functions_(codeFunctions) {
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
             kVersionNumbers[version_].size());
}

void ClassfileWriter::WriteConstantPool() {
  // TODO
}

void ClassfileWriter::WriteAccessFlags() {
  out_.write(kPublicAccessFlag, sizeof(kPublicAccessFlag));
}

void ClassfileWriter::WriteClassName() {
	uint16_t indexInPool = (constant_pool_.addString("java/lang/Object"))-1;
	out_ << ((unsigned char) indexInPool & 0xFF00U >> 8);
	out_ << ((unsigned char) indexInPool & 0x00FFU);
}

void ClassfileWriter::WriteSuperClassName() {
  uint16_t indexInPool = constant_pool_.addString("java/lang/Object");
  out_ << ((unsigned char) indexInPool & 0xFF00U >> 8);
  out_ << ((unsigned char) indexInPool & 0x00FFU);
}

void ClassfileWriter::WriteInterfaces() {
  out_.write(kNotRequired, sizeof(kNotRequired));
}

void ClassfileWriter::WriteFields() {
  out_.write(kNotRequired, sizeof(kNotRequired));
}

void ClassfileWriter::WriteMethods() {
	/**
	 * TODO: insert method_count dynamically (minimum 2 for init & main method for MS1)
	 * TODO: use u2 (e.g. 2 methods = \x0002)
	 */

	/**
	 * Inserts the bytecode for the method init.
	 * Should be the same in every class-file.
	 */
	char methodInit[] { '\x00', '\x01',	/* access_flag=1 */
						// TODO: insert reference from constant pool here
						'\x00', '\x07',	/* <inti> */
						'\x00', '\x08',	/* ()V */
						'\x00', '\x01',	/* u2 attributes_count=1 */
						'\x00', '\x09',	/* u2 attribute_name_index=9 */
						'\x00', '\x00', '\x00', '\x1D',	/* u4 attribute_length=29 */
						'\x00', '\x01', /* u2 max_stack=1 */
						'\x00', '\x01', /* u2 max_locals=1 */
						'\x00', '\x00', '\x00', '\x05',	/* u4 code_length=5 */
						'\x2A', '\xB7', '\x00', '\x01', '\xB1',
						'\x00', '\x00',	/* u2 exception_table_length=0 */
						'\x00', '\x01',	/* u2 attributes_count=1 */
						// TODO: insert reference from constant pool here
						'\x00', '\x0A',	/* u2 attribute_name_index=10, length=15; bytes="LineNumberTable" */
						'\x00', '\x00', '\x00', '\x06',	/* u4 attribute_length=6 */
						'\x00', '\x01', '\x00', '\x00', '\x00', '\x01'	/* Attribute bytes: */
		};
	out_.write(methodInit, sizeof(methodInit));

	/**
	 * Inserts the bytecode for the method main.
	 * Should be the same in every class-file.
	 * TODO: for MS2: May be replaced by a function
	 */
	char methodMain[] { '\x00', '\x09',	/* access_flag=9 */
						// TODO: insert reference from constant pool here
						'\x00', '\x0B',	/* main */
						'\x00', '\x0C',	/* ([Ljava/lang/String;)V */
						'\x00', '\x01',	/* u2 attributes_count=1 */
						'\x00', '\x09',	/* u2 attribute_name_index=9 */
						'\x00', '\x00', '\x00', '\x25',	/* u4 attribute_length=37 */
						'\x00', '\x02', /* u2 max_stack=2 */
						'\x00', '\x01', /* u2 max_locals=1 */
						'\x00', '\x00', '\x00', '\x09',	/* u4 code_length=9 */
						'\xB2', '\x00', '\x02', '\x12', '\x03', '\xB6', '\x00', '\x04', '\xB1',
						'\x00', '\x00',	/* u2 exception_table_length=0 */
						'\x00', '\x01',	/* u2 attributes_count=1 */
						// TODO: insert reference from constant pool here
						'\x00', '\x0A',	/* u2 attribute_name_index=10, length=15; bytes="LineNumberTable" */
						'\x00', '\x00', '\x00', '\x0A',	/* u4 attribute_length=10 */
						'\x00', '\x02', '\x00', '\x00', '\x00', '\x03', '\x00', '\x08', '\x00', '\x04'	/* Attribute bytes: */
		};
	out_.write(methodMain, sizeof(methodMain));
}

void ClassfileWriter::WriteAttributes() {
  // TODO
}
