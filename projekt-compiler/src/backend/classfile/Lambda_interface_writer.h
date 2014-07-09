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
 *
 * This header declares the functions for @see classfile_writer.cc.
 * The @see classfile_writer writes the specific classfile we want to create from a rail program.
 * For each function in an rail program (incl. main), we need to produce a method in this classfile.
 * To ensure a clean initialization we need to use the two methods <init> and <clinit>.
 * In addition each classfile contains the executable main function because alle Rail programms must have
 * one.
 *
 */
#ifndef LAMBDA_INTERFACE_WRITER_h_
#define LAMBDA_INTERFACE_WRITER_h_
#include <backend/classfile/classfile_writer.h>

class Lambda_interface_writer :
   public ClassfileWriter
{
public:
    /**
   * The constructor of the Lambda_interface_writer.
   *
   * @param version             The Java version.
   * @param constant pool       The current constant pool of the classfile.
   * @param graphs              The graph we extract the rail commands from.
   * @param codeFunctions       The mapping of the function and appropriate bytecode.
   * @param out                 The stream we write on.
   */
  Lambda_interface_writer(ClassfileVersion version, ConstantPool* constantPool,
                    Graphs& graphs,
                    const std::map<std::string, codegen::Bytecode&> codeFunctions,
                    std::ostream* out);
  virtual ~Lambda_interface_writer();

  static const std::string lambda_class_name;
  std::string lambda_file_name;
  static const std::string method_name;
  static const std::string method_descriptor;
protected:
  static const unsigned char interface_access_flags[];
  static const unsigned char method_access_flags[];

  /**
   * Method to write the access flag of the classfile.
   *
   * @return void
   */
  virtual void WriteAccessFlags();

  /**
   * Method to write the name of the classfile.
   *
   * @return void
   */
  virtual void WriteClassName();



  /**
   * Method to write the fields in the classfile.
   *
   * @return void
   */
  virtual void WriteFields();

  /**
   * Method to write methods in the classfile.
   * The methode code is generated in the class @see Bytecode.cpp
   *
   * @return void
   */
  virtual void WriteMethods();

  /**
   * Method to write the <init> method in the classfile.  WriteMagicNumber();
   * For us it is always the same bytecode.
   *
   * @return void
   */
  virtual void WriteInitMethod();

  /**
   * Method to write the <clinit> method in the classfile.
   * For us it is always the same bytecode
   *
   * @return void
   */
  virtual void WriteClInitMethod();

  /**
   * Method to write the attributes in the classfile, particularly the code attribute.
   *
   * @return void
   */
  virtual void WriteAttributes(const std::string &key);

  void write_array(size_t len, const unsigned char arr[]);
  size_t get_class_ref();
  size_t get_class_ref(const std::string& _class);
};

#endif //  LAMBDA_INTERFACE_WRITER_h_
