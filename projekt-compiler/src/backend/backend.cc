/*[--**--]
Copyright (C) 2014  SWPSoSe14Cpp Group

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this
program; if not, see <http://www.gnu.org/licenses/>.*/

#include <backend/backend.h>
#include <common/Env.h>

/*!
 * \mainpage backend.cc
 * \author Backend group & friends
 * \date SoSe 2014
 *
 * This class generates the target code from an input graph.
 *
 */

/*!
 * \brief Generates target code from a serialized graph
 * \param graphIn The serialized graph
 * \param codeOut The target code on the output stream
 *
 * \returns Generated class-file
 */
Backend::Status Backend::Generate(const std::string& graphIn,
                                  std::ostream* codeOut) {
  Graphs graphs;
  graphs.unmarshall(graphIn, ';');
  Backend::Status ret = Backend::Generate(graphs, codeOut);
  return ret;
}
/*!
 * \brief Generates target code from a graph
 * \param graphs The graph
 * \param codeOut The target code on the output stream
 *
 * Start in the graph at the main function. Went on with generating the constant pool
 * to write it and the other member as java class-file.
 * Is called after the serialized graph was unmarshalled.
 *
 * \returns Status of the created file
 */
Backend::Status Backend::Generate(Graphs& graphs,
                                  std::ostream* codeOut) {
  std::string entryFunctionName("main");
  Graphs::Graph_ptr mainFunction = graphs.find(entryFunctionName);
  ConstantPool constantPool;

  /// Add strings
  uint16_t obj_cls_idx = constantPool.addString("java/lang/Object");
  uint16_t system_idx = constantPool.addString("java/lang/System");
  uint16_t print_idx = constantPool.addString("java/io/PrintStream");
  uint16_t system_in_class_name_idx = constantPool.addString("java/io/InputStream");
  uint16_t integer_idx = constantPool.addString("java/lang/Integer");
  uint16_t string_idx = constantPool.addString("java/lang/String");
  uint16_t void_descriptor_idx = constantPool.addString("()V");
  uint16_t stack_init_idx = constantPool.addString("<clinit>");
  constantPool.addString("Code");
  uint16_t object_name_idx = constantPool.addString("<init>");
  constantPool.addString("([Ljava/lang/String;)V");
  uint16_t print_type_idx = constantPool.addString("(Ljava/lang/String;)V");
  uint16_t system_name_idx = constantPool.addString("out");
  uint16_t system_in_str_idx = constantPool.addString("in");
  uint16_t system_type_idx = constantPool.addString("Ljava/io/PrintStream;");
  uint16_t system_in_type_str_idx = constantPool.addString("Ljava/io/InputStream;");
  uint16_t print_name_idx = constantPool.addString("println");
  uint16_t system_in_avail_str_idx = constantPool.addString("available");
  uint16_t system_in_read_str_idx = constantPool.addString("read");
  uint16_t valueOf_name_idx = constantPool.addString("valueOf");
  uint16_t valueOf_type_idx = constantPool.addString("(I)Ljava/lang/Integer;");
  uint16_t intValue_name_idx = constantPool.addString("intValue");
  uint16_t intValue_type_idx = constantPool.addString("()I");
  uint16_t concat_name_idx = constantPool.addString("concat");
  uint16_t concat_type_idx = constantPool.addString("(Ljava/lang/String;)Ljava/lang/String;");
  uint16_t substring_name_idx = constantPool.addString("substring");
  uint16_t substring_type_idx = constantPool.addString("(II)Ljava/lang/String;");
  uint16_t substring_type_single_idx = constantPool.addString("(I)Ljava/lang/String;");
  uint16_t length_name_idx = constantPool.addString("length");
  uint16_t stack_class_name_idx = constantPool.addString("java/util/ArrayDeque");
  uint16_t pop_name_idx = constantPool.addString("pop");
  uint16_t pop_type_idx = constantPool.addString("()Ljava/lang/Object;");
  uint16_t push_name_idx = constantPool.addString("push");
  uint16_t push_type_idx = constantPool.addString("(Ljava/lang/Object;)V");
  uint16_t toString_name_idx = constantPool.addString("toString");
  uint16_t intCompare_type_idx = constantPool.addString("(Ljava/lang/Integer;)I");
  uint16_t compare_name_idx = constantPool.addString("compareTo");
  uint16_t boolEquals_type_idx = constantPool.addString("(Ljava/lang/Object;)Z");
  uint16_t equals_name_idx = constantPool.addString("equals");
  uint16_t toString_type_idx = constantPool.addString("()Ljava/lang/String;");
  uint16_t stringbuilder_idx = constantPool.addString("java/lang/StringBuilder");
  uint16_t size_str_idx = constantPool.addString("size");
  uint16_t list_str_idx = constantPool.addString("java/util/ArrayList");
  uint16_t add_str_idx = constantPool.addString("add");
  uint16_t remove_str_idx = constantPool.addString("remove");
  uint16_t remove_type_idx = constantPool.addString("(I)Ljava/lang/Object;");
  //uint16_t add_type_idx = constantPool.addString("(Ljava/lang/Object)Z")  same like bool equals

  ///  Add classes
  constantPool.obj_idx.class_idx = constantPool.addClassRef(obj_cls_idx);
  uint16_t system_class_idx = constantPool.addClassRef(system_idx);
  uint16_t system_in_class_idx = constantPool.addClassRef(system_in_class_name_idx);
  uint16_t print_class_idx = constantPool.addClassRef(print_idx);
  constantPool.int_idx.class_idx = constantPool.addClassRef(integer_idx);
  constantPool.str_idx.class_idx = constantPool.addClassRef(string_idx);
  constantPool.arr_idx.class_idx = constantPool.addClassRef(stack_class_name_idx);
  constantPool.list_idx.class_idx = constantPool.addClassRef(list_str_idx);
  constantPool.addClassRef(stringbuilder_idx);


  ///  Add name and type
  uint16_t object_name_type_idx = constantPool.addNameAndType(object_name_idx, void_descriptor_idx);
  uint16_t system_name_type_idx = constantPool.addNameAndType(system_name_idx, system_type_idx);
  uint16_t system_in_name_type_idx = constantPool.addNameAndType(system_in_str_idx, system_in_type_str_idx);
  uint16_t print_name_type_idx =  constantPool.addNameAndType(print_name_idx, print_type_idx);
  uint16_t system_in_read_name_type_idx = constantPool.addNameAndType(system_in_read_str_idx, intValue_type_idx);
  uint16_t system_in_avail_name_type_idx = constantPool.addNameAndType(system_in_avail_str_idx, intValue_type_idx);
  uint16_t valueOf_name_type_idx = constantPool.addNameAndType(valueOf_name_idx, valueOf_type_idx);
  uint16_t intValue_name_type_idx = constantPool.addNameAndType(intValue_name_idx, intValue_type_idx);
  uint16_t concat_name_type_idx = constantPool.addNameAndType(concat_name_idx, concat_type_idx);
  uint16_t compare_name_type_idx = constantPool.addNameAndType(compare_name_idx, intCompare_type_idx);
  uint16_t equals_name_type_idx = constantPool.addNameAndType(equals_name_idx, boolEquals_type_idx);
  uint16_t substring_name_type_idx = constantPool.addNameAndType(substring_name_idx, substring_type_idx);
  uint16_t substring_name_type_single_idx = constantPool.addNameAndType(substring_name_idx, substring_type_single_idx);
  uint16_t length_name_type_idx = constantPool.addNameAndType(length_name_idx, intValue_type_idx);
  constantPool.addNameAndType(stack_init_idx, void_descriptor_idx);
  uint16_t pop_name_type_idx = constantPool.addNameAndType(pop_name_idx, pop_type_idx);
  uint16_t push_name_type_idx = constantPool.addNameAndType(push_name_idx, push_type_idx);
  uint16_t toString_name_type_idx = constantPool.addNameAndType(toString_name_idx, toString_type_idx);
  uint16_t size_name_type_idx = constantPool.addNameAndType(size_str_idx, intValue_type_idx);
  uint16_t add_name_type_idx = constantPool.addNameAndType(add_str_idx, boolEquals_type_idx);
  uint16_t remove_name_type_idx = constantPool.addNameAndType(remove_str_idx, remove_type_idx);

  ///  Add method refs
  constantPool.addMethRef(constantPool.obj_idx.class_idx , object_name_type_idx);
  constantPool.addMethRef(print_class_idx, print_name_type_idx);
  constantPool.addMethRef(system_in_class_idx, system_in_avail_name_type_idx);
  constantPool.addMethRef(system_in_class_idx, system_in_read_name_type_idx);
  constantPool.int_idx.value_of_idx = constantPool.addMethRef(constantPool.int_idx.class_idx, valueOf_name_type_idx);
  constantPool.int_idx.int_value_idx = constantPool.addMethRef(constantPool.int_idx.class_idx, intValue_name_type_idx);
  constantPool.int_idx.compare_idx = constantPool.addMethRef(constantPool.int_idx.class_idx, compare_name_type_idx);
  constantPool.int_idx.equals_idx = constantPool.addMethRef(constantPool.int_idx.class_idx, equals_name_type_idx);
  constantPool.str_idx.concat_idx = constantPool.addMethRef(constantPool.str_idx.class_idx, concat_name_type_idx);
  constantPool.str_idx.substring_2param_idx = constantPool.addMethRef(constantPool.str_idx.class_idx, substring_name_type_idx);
  constantPool.str_idx.substring_idx = constantPool.addMethRef(constantPool.str_idx.class_idx, substring_name_type_single_idx);
  constantPool.str_idx.length_idx = constantPool.addMethRef(constantPool.str_idx.class_idx, length_name_type_idx);
  constantPool.str_idx.length_idx = constantPool.addMethRef(constantPool.str_idx.class_idx, length_name_type_idx);
  constantPool.addMethRef(constantPool.arr_idx.class_idx, object_name_type_idx);
  constantPool.arr_idx.pop_idx = constantPool.addMethRef(constantPool.arr_idx.class_idx, pop_name_type_idx);
  constantPool.arr_idx.push_idx = constantPool.addMethRef(constantPool.arr_idx.class_idx, push_name_type_idx);
  constantPool.arr_idx.size = constantPool.addMethRef(constantPool.arr_idx.class_idx, size_name_type_idx);
  constantPool.obj_idx.toString = constantPool.addMethRef(constantPool.obj_idx.class_idx, toString_name_type_idx);
  constantPool.list_idx.add_idx = constantPool.addMethRef(constantPool.list_idx.class_idx, add_name_type_idx);
  constantPool.list_idx.remove_idx = constantPool.addMethRef(constantPool.list_idx.class_idx, remove_name_type_idx);
  constantPool.list_idx.init_idx = constantPool.addMethRef(constantPool.list_idx.class_idx, object_name_type_idx);
  ///  Add field refs
  constantPool.addFieldRef(system_class_idx, system_name_type_idx);
  constantPool.addFieldRef(system_class_idx, system_in_name_type_idx);

  uint16_t main_class_str_idx = constantPool.addString(Env::getDstClassName());
  uint16_t main_class_idx = constantPool.addClassRef(main_class_str_idx);
  uint16_t stack_field_name_idx = constantPool.addString("stack");
  uint16_t stack_field_type_idx = constantPool.addString("Ljava/util/ArrayDeque;");
  uint16_t stack_field_name_type_idx = constantPool.addNameAndType(stack_field_name_idx, stack_field_type_idx);
  constantPool.arr_idx.field_idx = constantPool.addFieldRef(main_class_idx, stack_field_name_type_idx);


  ///  Add Rail-Functionnames as Strings
   std::vector<std::string> keyset = graphs.keyset();
   for (auto it = keyset.begin(); it != keyset.end(); it++) {
     constantPool.addString(*it);
   }
  //Create code for each function
   std::map<std::string, codegen::Bytecode&> codeMap;
   for (std::vector<std::string>::iterator it = keyset.begin(); it != keyset.end(); it++) {
      codegen::Bytecode* code = new codegen::Bytecode(constantPool);
 		  code->build(graphs.find(*it));
 		  codeMap.insert(std::pair<std::string, codegen::Bytecode&>(*it, *code));
   }


  ClassfileWriter writer(ClassfileWriter::JAVA_7, &constantPool, graphs, codeMap, codeOut);
  writer.WriteClassfile();

  for (std::map<std::string, codegen::Bytecode&>::iterator it = codeMap.begin(); it != codeMap.end(); it++) {
    delete &it->second;
  }
  return Backend::Status::SUCCESS;
}

/*!
 * \brief Generates status message
 * \param status Status of backend function
 *
 * \returns nothing (TODO)
 */
std::string Backend::ErrorMessage(Backend::Status status) {
  switch (status) {
    case Backend::Status::SUCCESS:
      return "";
      break;
      // TODO handle other cases
    default:
      return "";
  }
}
