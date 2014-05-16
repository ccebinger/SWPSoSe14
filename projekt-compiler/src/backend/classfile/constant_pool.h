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

#ifndef CONSTANT_POOL_H_
#define CONSTANT_POOL_H_

#include <cstdint>
#include <vector>
#include <string>
#include <functional>

static const int STR = 8;
static const int INT = 3;
static const int LONG = 5;

class Item {
 public:
  Item();
  explicit Item(const Item &i);
  explicit Item(uint16_t _index);
  Item(uint16_t _index, const Item &i);

  bool operator==(const Item& i)const;
  bool operator=(const Item& i);

  void set(int32_t intVal);
  void set(int64_t longVal);
  void set(int32_t _type, const std::string &_strVal1,
           const std::string &_strVal2,
           const std::string &_strVal3);

  Item *next;
  uint16_t index;
  int32_t type;
  int32_t intVal;
  int64_t longVal;
  std::string strVal1;
  std::string strVal2;
  std::string strVal3;
  int32_t hashCode;

 private:
  std::hash<std::string> hash_fn;
};

class ConstantPool {
 public:
  ConstantPool();

  uint16_t addString(const std::string &value);
  uint16_t addInt(int32_t value);
  uint16_t addLong(int64_t value);
  std::vector<uint8_t> getByteArray();
 private:
  uint16_t get(const Item &key)const;
  void put(const Item &i);
  void put122(int32_t b, int32_t s1, int32_t s2);
  void put112(int32_t b1, int32_t b2, int32_t s);

  int threshold;
  int index;
  Item key;
  Item key2;
  std::vector<Item> items;
  std::vector<uint8_t> pool;
};

#endif /* CONSTANT_POOL_H_ */
