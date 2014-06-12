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

#include <cstdint>
#include <vector>
#include <string>
#include <functional>

enum ItemType{
  CLASS = 7,
  FIELD = 9,
  METHOD = 10,
  IMETHOD = 11,
  UTF8 = 1,
  STR = 8,
  INT = 3,
  LONG = 5
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
};

////////////////////////////////////////////////////////////////////////
/// ! ConstantPool
////////////////////////////////////////////////////////////////////////

class ConstantPool {
 public:
  ConstantPool();

  size_t addInt(int32_t value);
  size_t addLong(int64_t value);
  size_t addString(const std::string &value);
  size_t addClassRef(const std::string &value);
  size_t addFieldRef(const std::string &value);
  size_t addMethRef(const std::string &value);
  size_t addIMethRef(const std::string &value);

  std::vector<uint8_t> getByteArray();
  bool check(const Item& i) const;   //!< check if item in list
  const Item& get(const Item &key)const;
  size_t countItemType(ItemType type);

 protected:
  void putByte(uint8_t b);
  void putShort(uint16_t s);
  void putInt(int32_t i);
  void putLong(int64_t l);
  void putUTF8(std::string s);
  void encodeUTF8(std::string s, uint32_t pos);

  size_t put(Item i);

  std::vector<Item> items;  //!< item list
  std::vector<uint8_t> pool;  //!< byte constant list
};

#endif  // PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_CONSTANT_POOL_H_
