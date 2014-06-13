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
#include <iostream>

enum ItemType{
  NONE,
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

////////////////////////////////////////////////////////////////////////
/// ! Item
////////////////////////////////////////////////////////////////////////

class Item {
 public:
  Item();
  Item(const Item &i);
  explicit Item(uint16_t _index);
  Item(uint16_t _index, const Item &i);

  bool operator==(const Item& i)const;
  bool operator=(const Item& i);

  void set(int32_t intVal);
  void set(int64_t longVal);
  void set(ItemType _type, const std::string &_strVal);

  size_t index;  //!< index of item
  ItemType type;  //!< item type
  int32_t intVal;  //!< if type integer the value is stored here
  int64_t longVal;  //!< if type long values is stored here
  std::string strVal;  //!< if type string value is stored here
  Item *next;  //!< pointer to next item in list
  std::vector<unsigned char> getHexRepresentation(Bytecode_writer& writer)
  {
    std::vector<unsigned char> result;
    std::stringstream sstream;
    //sstream << '0' << type;
    writer.writeU8(type);
    //writer.
    //result.add((char) t);
    //sstream.width(1);
//    sstream.fill(prev);
    if (type == ItemType::INT)
      sstream << std::hex << intVal;
    else
    {
      if (type == ItemType::UTF8)
        sstream << std::hex << strVal.size();
      sstream << std::hex << strVal;
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
  ConstantPool();

  size_t addInt(int32_t value);
  size_t addNameAndType(int32_t UTF8_name_index, int32_t UTF8_descriptor_index);
  size_t addString(const std::string &value);
  size_t addClassRef(const std::string &value);
  size_t addFieldRef(const std::string &value);
  size_t addMethRef(const std::string &value);
  size_t addIMethRef(const std::string &value);

  std::vector<uint8_t> getByteArray();
  bool check(const Item& i) const;   //!< check if item in list
  const Item& get(const Item &key)const;
  size_t countItemType(ItemType type);

  std::vector<Item> getItems() {return items;}

<<<<<<< HEAD
=======
 protected:
>>>>>>> merge
  void putByte(uint8_t b);
  void putShort(uint16_t s);
  void putInt(int32_t i);
  void putUTF8(std::string s);

  size_t put(Item i);
 protected:
  void encodeUTF8(std::string s, uint32_t pos);

  std::vector<Item> items;  //!< item list
  std::vector<uint8_t> pool;  //!< byte constant list
};

#endif  // PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_CONSTANT_POOL_H_
