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

  std::vector<char> mainCode = BytecodeGenerator::GenerateCodeFromFunctionGraph(mainFunction,
                                                                                constantPool);
  std::map<std::string, std::vector<char>&> codeMap{{"main", mainCode}};

  ClassfileWriter writer(ClassfileWriter::JAVA_7, &constantPool, graphs, codeMap, codeOut);
  writer.WriteClassfile();
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
