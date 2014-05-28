/*!
* \mainpage classfile_writer.cc
* \author Backend group & friends
* \date SoSe 2014
*
* This class writes the specific class-file we want to create from a rail program.
* For each function in an rail program (incl. main), we need to produce one class-file.
*
*/

#include <backend/classfile/classfile_writer.h>
#include <backend/classfile/constant_pool.h>

#include <array>
#include <iostream>
#include <map>
#include <string>

const char ClassfileWriter::kMagicNumber[] { '\xCA', '\xFE','\xBA', '\xBE' };
const char ClassfileWriter::kNotRequired[] { '\x00', '\x00' };
const char ClassfileWriter::kPublicAccessFlag[] { '\x00', '\x00','\x00', '\x01' };

std::map<ClassfileWriter::ClassfileVersion, const std::array<const char, 4>>
    ClassfileWriter::kVersionNumbers {
    {ClassfileWriter::ClassfileVersion::JAVA_7,
    std::array<const char, 4>{'\x00', '\x00', '\x00', '\x33'}}
};

/*!
 * \brief Constructor for ClassfileWriter
 * \param version The java version of the class-file
 * \param constantPool The specific constant pool for the class-file
 * \param codeFunctions Holds the the bytecode via map function name -> bytecode
 * \param out The ouput stream
 */
ClassfileWriter::ClassfileWriter(ClassfileVersion version,
                                 ConstantPool& constantPool,
                                 const std::map<std::string,
                                 	 std::vector<char>&> codeFunctions,
                                 std::ostream& out) :
    out_(out), version_(version), constant_pool_(constantPool),
    code_functions_(codeFunctions) {
}

/*!
 * \brief Deconstructor for ClassFileWriter
 */
ClassfileWriter::~ClassfileWriter() {
}

/*!
 * \brief Calls methods to write into the classfile
 * Each method represents an specific part of the class-file
 */
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
}

/*!
 * \brief Write the magic number
 * The magic number indicates a java class-file
 */
void ClassfileWriter::WriteMagicNumber() {
  out_.write(kMagicNumber, sizeof(kMagicNumber));
}

/*!
 * \brief Write the java version number (e.g. 0x00000033 for v.7)
 */
void ClassfileWriter::WriteVersionNumber() {
  out_.write(kVersionNumbers[version_].data(),
             kVersionNumbers[version_].size());
}

/*!
 * \brief Write the constant pool
 * \sa constant_pool.cc
 */
void ClassfileWriter::WriteConstantPool() {
  out_.write((char*)constant_pool_.getByteArray().data(),
             constant_pool_.getByteArray().size());
}

/*!
 * \brief Write the access flag (e.g. 0x00000001 for public)
 */
void ClassfileWriter::WriteAccessFlags() {
  out_.write(kPublicAccessFlag, sizeof(kPublicAccessFlag));
}

/*!
 * \brief Write the class name
 * We get the class name from the constant pool.
 * The index of the class name is one postition before the
 * 	java/lang/object super class
 */
void ClassfileWriter::WriteClassName() {
	uint16_t indexInPool = (constant_pool_.addString("java/lang/Object"))-1;
	out_ << ((unsigned char) indexInPool & 0xFF00U >> 8);
	out_ << ((unsigned char) indexInPool & 0x00FFU);
}
/*!
 * \brief Write super class name
 * For us we always have the java/lang/object class
 */
void ClassfileWriter::WriteSuperClassName() {
  uint16_t indexInPool = constant_pool_.addString("java/lang/Object");
  out_ << ((unsigned char) indexInPool & 0xFF00U >> 8);
  out_ << ((unsigned char) indexInPool & 0x00FFU);
}

/*!
 * \brief Write the interfaces
 * Not used in Rail programms, thus 0x0000
 */
void ClassfileWriter::WriteInterfaces() {
  out_.write(kNotRequired, sizeof(kNotRequired));
}

/*!
 * \brief Write the fields
 * Not used in Rail programms, thus 0x0000
 */
void ClassfileWriter::WriteFields() {
  out_.write(kNotRequired, sizeof(kNotRequired));
}

/*!
 * \brief Write the methods
 * The methods refer to the constant pool.
 * BUT this reference is again an reference to the actual element.
 * (i.e. method -> reference in constant pool -> actual element in constant pool)
 * FIXME: The init and main is hard coded. Should be replaced later.
 */
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
						// TODO: call WriteAttributes to link method with its specific attributes
						'\x00', '\x01',	/* u2 attributes_count=1 */
						// TODO: insert reference from constant pool
						'\x00', '\x0A',	/* u2 attribute_name_index=10, length=15; bytes="LineNumberTable" */
						'\x00', '\x00', '\x00', '\x06',	/* u4 attribute_length=6 */
						'\x00', '\x01', '\x00', '\x00', '\x00', '\x01'	/* Attribute bytes: */
		};
	out_.write(methodInit, sizeof(methodInit));

	/**
	 * Inserts the bytecode for the method main.
	 * Should be the same in every class-file.
	 * TODO: for MS2: May be replaced by a rail function
	 */
	char methodMain[] { '\x00', '\x09',	/* access_flag=9 */
						// TODO: insert reference from constant pool
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
						// TODO: call WriteAttributes to link method with its specific attributes
						'\x00', '\x01',	/* u2 attributes_count=1 */
						// TODO: insert reference from constant pool
						'\x00', '\x0A',	/* u2 attribute_name_index=10, length=15; bytes="LineNumberTable" */
						'\x00', '\x00', '\x00', '\x0A',	/* u4 attribute_length=10 */
						'\x00', '\x02', '\x00', '\x00', '\x00', '\x03', '\x00', '\x08', '\x00', '\x04'	/* Attribute bytes: */
		};
	out_.write(methodMain, sizeof(methodMain));
}

/*!
 * \brief Writes attributes in class-file
 * Every method calls WritesAttributes
 */
void ClassfileWriter::WriteAttributes() {
/**
 * TODO: 0. insert attribute_count (u2)
 *  Code_attribute:
 * TODO: 1. call constant pool to get reference of attribute (u2)
 * 			-> attribute_name_index
 * TODO: 2. indicate the length of the subsequent information in bytes (u4) (without 6 bytes of attribute_name_index)
 * 			-> attributes_length
 * TODO: 3. maximum depth of the operand stack of this method at any point during execution of the method (u2)
 * 			-> max_stack
 * TODO: 4. number of local variables in the local variable array allocated upon invocation of this method (u2)
 * 			-> max_locals
 * TODO: 5. number of bytes in the code array for this method (u4) = should be: max_stack + max_locals
 * 			-> code_length
 */
	// out_.write(kNotRequired,sizeof(kNotRequired)); //exception_table_length
/**
 *  LineNumberTable:
 * TODO: 1. insert attribute_count (u2)
 * TODO: 2. call constant pool to get reference of attribute (u2) = +1 of code_attribute
 * 			-> attributes_name_index
 * TODO: 3. indicate the length of the subsequent information in bytes (u4) (without 6 bytes of attribute_name_index)
 * 			-> attributes_length
 *
 *  StackMapTable is needed for variables MS2
 */
}
