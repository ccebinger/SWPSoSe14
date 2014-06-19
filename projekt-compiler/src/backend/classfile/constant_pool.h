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

////////////////////////////////////////////////////////////////////////
// ! \file constant_pool.h
/// definition
////////////////////////////////////////////////////////////////////////

#ifndef PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_CONSTANT_POOL_H_
#define PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_CONSTANT_POOL_H_

#include <backend/classfile/Bytecode_writer.h>
#include <cstdint>
#include <vector>
#include <string>
#include <functional>
#include <sstream>
#include <ios>
#include <memory>

enum ItemType{
  NONE = 0x00,
  CLASS = 0x07,
  FIELD = 0x09,
  METHOD = 0x0A,
  IMETHOD = 0x0B,
  UTF8 = 0x01,
  STR = 0x08,
  INT = 0x03,
  LONG = 0x05,
  NAME_AND_TYPE = 0x0C
};

struct IntegerIndexes {
  uint16_t class_idx = 0;
  uint16_t value_of_idx = 0;
  uint16_t int_value_idx = 0;
  uint16_t compare_idx = 0;
};

struct StringIndexes {
  uint16_t class_idx = 0;
  uint16_t concat_idx = 0;
  uint16_t substring_2param_idx = 0;
  uint16_t substring_idx = 0;
  uint16_t length_idx = 0;
};


struct ArrayDequeIndexes {
  uint16_t class_idx = 0;
  uint16_t field_idx = 0;
  uint16_t pop_idx = 0;
  uint16_t push_idx = 0;
  uint16_t size = 0;
};

struct ObjectIndexes {
  uint16_t class_idx = 0;
  uint16_t toString = 0;
  uint16_t equals = 0;
};
////////////////////////////////////////////////////////////////////////
/// ! Item
////////////////////////////////////////////////////////////////////////

class Item {
 public:
  Item();
  Item(const Item &i);

  bool operator==(const Item& i)const;
  Item& operator=(const Item& i);

  void set(int32_t intVal);
  void set(int64_t longVal);
  void set(ItemType _type, const std::string &_strVal1,
           const std::string &_strVal2, const std::string &_strVal3);
  void set(ItemType type, uint16_t class_idx, uint16_t name_type_idx);
  void set(ItemType type, uint16_t name_idx);
  void set_name_type(uint16_t method_idx, uint16_t descriptor_idx);

  size_t index;  //!< index of item
  ItemType type;  //!< item type
  int32_t intVal;  //!< if type integer the value is stored here
  int64_t longVal;  //!< if type long values is stored here
  std::string strVal1;  //!< if method, class or string value stored here
  std::string strVal2;  //!< if method or class, method name
  std::string strVal3;  //!< if method or class, method descriptor
  std::shared_ptr<Item> next;  //!< pointer to next item in list

  uint16_t method_idx;  //!< index of method string in constantpool
  uint16_t descriptor_idx;  //!< index of descriptor string in constantpool
  uint16_t name_idx;  //!< index of name/string in constantpool
  uint16_t class_idx;  //!< index of class in constantpool
  uint16_t name_type_idx;  //!< index of name and type in constantpool

  std::vector<unsigned char> getHexRepresentation(Bytecode_writer* writer) {
    std::vector<unsigned char> result;
    std::stringstream sstream;
    writer->writeU8(type);
    if (type == ItemType::INT) {
      sstream << std::hex << intVal;
    } else {
      if (type == ItemType::UTF8)
        sstream << std::hex << strVal1.size();
      sstream << std::hex << strVal1;
    }

    std::string str = sstream.str();
    int len = str.length();
    for (int i = 0; i < len; i++)
      result.push_back(str.at(i));
    return result;
  }
};

////////////////////////////////////////////////////////////////////////
/// ! ConstantPool
////////////////////////////////////////////////////////////////////////

class ConstantPool {
 public:
  IntegerIndexes int_idx;
  StringIndexes str_idx;
  ArrayDequeIndexes arr_idx;
  ObjectIndexes obj_idx;

  ConstantPool();

  size_t addInt(int32_t value);
  size_t addNameAndType(uint16_t UTF8_name_index,
                        uint16_t UTF8_descriptor_index);
  size_t addString(const std::string &value);
  size_t addConstString(uint16_t &string_idx);
  size_t addClassRef(uint16_t name_idx);
  size_t addFieldRef(uint16_t class_idx, uint16_t name_type_idx);
  size_t addMethRef(uint16_t class_idx, uint16_t name_type_idx);

  std::vector<uint8_t> getByteArray();
  bool check(const Item& i) const;   //!< check if item in list
  const Item& get(const Item &key)const;
  size_t countItemType(ItemType type);
  size_t size() {return items.size();}

  std::vector<Item> getItems() {return items;}

 protected:
  void putByte(uint8_t b);
  void putShort(uint16_t s);
  void putInt(int32_t i);
  void putUTF8(std::string s);

  size_t put(Item *i);

  void encodeUTF8(std::string s, uint32_t pos);

  std::vector<Item> items;  //!< item list
  std::vector<uint8_t> pool;  //!< byte constant list
};

#endif  // PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_CONSTANT_POOL_H_
