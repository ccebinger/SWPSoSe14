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

#ifndef PROJEKT_COMPILER_SRC_BACKEND_BACKEND_H_
#define PROJEKT_COMPILER_SRC_BACKEND_BACKEND_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

#include <frontend/Graphs.h>
#include <backend/classfile/constant_pool.h>
#include <backend/codegen/Bytecode.h>
#include <backend/classfile/Lambda_classfile_writer.h>

/**
 * Die Klasse stellt statische Methoden für das Übersetzen des Graphen in
 * Target-Code zur Verfügung.
 */
class Backend {
 public:
  /**
   * Die möglichen 'Exit Codes' des Backends.
   */
  enum Status {
    /**
     * Erfolgreiche Übersetzung.
     */
    SUCCESS
    // TODO other statuses...
  };

  /**
   * Übersetzt den serialisierten Graphen aus graphIn (Dateiname) in
   * Target-Code, der auf codeOut geschrieben wird. Gibt je nach Ergebnis einen
   * Backend::Status zurück.
   */
  static Backend::Status Generate(const std::string& filename,
                                  std::ostream* codeOut);

  /**
   * Übersetzt den Graphen aus graph in Target-Code, der auf codeOut
   * geschrieben wird. Gibt je nach Ergebnis einen Backend::Status zurück.
   */
  static Backend::Status Generate(Graphs& graphs,
                                  std::ostream* codeOut);

  /**
   * Gibt eine beschreibende Fehlernachricht zu einem Backend::Status
   * zurück. Ist der status Backend::Status::SUCCESS, wird ein leerer String
   * zurückgegeben.
   * Typische Nutzung dieser Methode:
   * if (status != Backend::Status::SUCCESS) {
   * 		std::cerr << Backend::ErrorMessage(status) << std::endl;
   * }
   */
  static std::string ErrorMessage(Backend::Status status);

 private:
  /**
   * Nur statische Methoden.
   */
  Backend();
  static std::string get_lambda_class_name(const std::string& name, bool anonymous);
  //static void write_lambda_classes(Graphs::Graph_ptr graph, ConstantPool& constantPool);
  static void write_lambda_interface();
  static void write_lambda_anonymous_classes(Graphs& graphs, ConstantPool& constantPool, codegen::Bytecode* code);
  static void write_lambda_anonymous_class(Graphs::Graph_ptr graph, ConstantPool& constantPool, std::string& name, LocalVariableStash& locals);
  static void add_entries_to_constantpool(ConstantPool& constantPool);
  static void add_functions_to_constantpool(ConstantPool& constantPool, std::vector<std::string>& keyset);
  static void write_lambda_default_constantpool(ConstantPool& constant_pool, const std::string& lambda_class_name);
};

#endif  // PROJEKT_COMPILER_SRC_BACKEND_BACKEND_H_
