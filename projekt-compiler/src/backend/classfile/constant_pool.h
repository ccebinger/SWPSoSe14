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

static const int STR = 8;
static const int INT = 3;
static const int LONG = 5;

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
  void set(int32_t _type, const std::string &_strVal1,
           const std::string &_strVal2,
           const std::string &_strVal3);

  size_t index;  //!< index of item
  int32_t type;  //!< item type
  int32_t intVal;  //!< if type integer the value is stored here
  int64_t longVal;  //!< if type long values is stored here
  std::string strVal1;  //!< if type string value is stored here
  std::string strVal2;  //!< if type string value is stored here
  std::string strVal3;  //!< if type string value is stored here
  Item *next;  //!< pointer to next item in list
};

////////////////////////////////////////////////////////////////////////
/// ! ConstantPool
////////////////////////////////////////////////////////////////////////

class ConstantPool {
 public:
  ConstantPool();

  size_t addByte(uint8_t value);
  size_t addInt(int32_t value);
  size_t addLong(int64_t value);
  size_t addString(const std::string &value);
  std::vector<uint8_t> getByteArray();
  bool check(const Item& i) const;   //!< check if item in list
  const Item& get(const Item &key)const;

 private:
  void encodeUTF8(std::string s, uint32_t pos);

  void putByte(uint8_t b);
  void putShort(uint16_t s);
  void putInt(int32_t i);
  void putLong(int64_t l);
  void putUTF8(std::string s);

  size_t put(Item i);
  void put11(int32_t b1, int32_t b2);
  void put12(int32_t b, int32_t s);
  void put122(int32_t b, int32_t s1, int32_t s2);
  void put112(int32_t b1, int32_t b2, int32_t s);

  std::vector<Item> items;  //!< item list
  std::vector<uint8_t> pool;  //!< byte constant list
};

#endif  // PROJEKT_COMPILER_SRC_BACKEND_CLASSFILE_CONSTANT_POOL_H_
