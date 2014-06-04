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

#include <backend/classfile/constant_pool.h>
#include <algorithm>

////////////////////////////////////////////////////////////////////////
/// default constructor does nothing, bud is needed because we have other
//  constructor
////////////////////////////////////////////////////////////////////////
Item::Item() {
}

////////////////////////////////////////////////////////////////////////
/// constructor with defined index
/// \param index internal index of item
////////////////////////////////////////////////////////////////////////
Item::Item(uint16_t _index) { index = _index;}

////////////////////////////////////////////////////////////////////////
/// copy constructor
/// \param i item with values to set to
////////////////////////////////////////////////////////////////////////
Item::Item(const Item &i) {
  index = i.index;
  type = i.type;
  intVal = i.intVal;
  longVal = i.longVal;
  strVal = i.strVal;
}

////////////////////////////////////////////////////////////////////////
/// constructor
/// \param _index internal index
/// \param i item to set to
////////////////////////////////////////////////////////////////////////
Item::Item(uint16_t _index, const Item &i) {
  index = _index;
  type = i.type;
  intVal = i.intVal;
  longVal = i.longVal;
  strVal = i.strVal;
}

////////////////////////////////////////////////////////////////////////
/// compare operator implementation
/// \param i copmared item
////////////////////////////////////////////////////////////////////////
bool Item::operator==(const Item& i)const {
  switch (type) {
    case UTF8:
    case CLASS:
    case STR:
      return i.strVal == strVal;
    case LONG:
      return i.longVal == longVal;
    case INT:
      return i.intVal == intVal;
    // case FIELD:
    // case METHOD:
    // case IMETHOD:
    default:
      return i.strVal == strVal;
  }
}

////////////////////////////////////////////////////////////////////////
/// copy operator
/// \param i item to be copied
////////////////////////////////////////////////////////////////////////
bool Item::operator=(const Item& i) {
  index = i.index;
  type = i.type;
  intVal = i.intVal;
  longVal = i.longVal;
  strVal = i.strVal;
  return true;
}

////////////////////////////////////////////////////////////////////////
/// set item to integer value
/// \param _intVal integer value
////////////////////////////////////////////////////////////////////////
void Item::set(int32_t _intVal) {
  type = INT;
  intVal = _intVal;
}

////////////////////////////////////////////////////////////////////////
/// set item to long value
/// \param _longVal long integer value
////////////////////////////////////////////////////////////////////////
void Item::set(int64_t _longVal) {
  type = LONG;
  longVal = _longVal;
}

////////////////////////////////////////////////////////////////////////
/// set item to string value
/// \param _type type of the item
/// \param _strVal  first part of the value of this item.
////////////////////////////////////////////////////////////////////////
void Item::set(ItemType _type,
               const std::string &_strVal) {
  type = _type;
  strVal = _strVal;
}

////////////////////////////////////////////////////////////////////////
/// default constructor
////////////////////////////////////////////////////////////////////////
ConstantPool::ConstantPool(): items(256) {
  // Java Class Reference auf java/lang/system
  addClassRef("java/lang/system");
  // Field Reference java.lang.system.out
  addFieldRef("java.lang.system.out");
  // Method referenc Java.io.printStream.println
  addMethRef("Java.io.printStream.println");
  // Method reference Main.java
  addMethRef("Main.java");
}

////////////////////////////////////////////////////////////////////////
/// method to put a integer into the pool
/// \param value value to add to pool
/// \return index of the integer in the pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addInt(int32_t value) {
  Item i;
  size_t index = 0;
  i.set(value);
  if (!check(i)) {
    put2(INT);
    index = put(i);
    putInt(value);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a long int into the pool
/// \param value value to add to pool
/// \return index of the long intenger in the pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addLong(int64_t value) {
  Item i;
  size_t index = 0;
  i.set(value);
  if (!check(i)) {
    put2(LONG);
    index = put(i);
    putLong(value);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a string into the pool
/// \param value value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addString(const std::string &value) {
  Item i;
  size_t index = 0;
  i.set(STR, value);
  if (!check(i)) {
    put2(UTF8);
    putUTF8(value);
    index = put(i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a string into the pool
/// \param value value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addClassRef(const std::string &value) {
  Item i;
  size_t index = 0;
  i.set(CLASS, value);
  if (!check(i)) {
    put2(CLASS);
    putUTF8(value);
    index = put(i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a string into the pool
/// \param value value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addFieldRef(const std::string &value) {
  Item i;
  size_t index = 0;
  i.set(FIELD, value);
  if (!check(i)) {
    put2(FIELD);
    putUTF8(value);
    index = put(i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a string into the pool
/// \param value value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addMethRef(const std::string &value) {
  Item i;
  size_t index = 0;
  i.set(METHOD, value);
  if (!check(i)) {
    put2(METHOD);
    putUTF8(value);
    index = put(i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a string into the pool
/// \param value value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addIMethRef(const std::string &value) {
  Item i;
  size_t index = 0;
  i.set(IMETHOD, value);
  if (!check(i)) {
    put2(IMETHOD);
    putUTF8(value);
    index = put(i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to count numbers of items with specified type
/// \return amount of items with types
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::countItemType(ItemType type) {
  return std::count_if(items.begin(),
                       items.end(),
                       [&](Item i) {return i.type == type;});
}

////////////////////////////////////////////////////////////////////////
/// returns constant pool as a byte array
/// \return the bytecode
////////////////////////////////////////////////////////////////////////
std::vector<uint8_t> ConstantPool::getByteArray() {
  return pool;
}

////////////////////////////////////////////////////////////////////////
/// return the constant pool index of a item
/// \param key item to find in pool
/// \return item in pool or last
////////////////////////////////////////////////////////////////////////
const Item &ConstantPool::get(const Item &key) const {
  auto i = std::find(items.begin(), items.end(), key);
  return *i;
}

////////////////////////////////////////////////////////////////////////
/// return the constant pool index of a item
/// \param i item to find in pool
/// \return index of item or zero if not in pool
////////////////////////////////////////////////////////////////////////
bool ConstantPool::check(const Item &key) const {
  auto i = std::find(items.begin(), items.end(), key);
  if (i != items.end()) {
    return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////
/// Puts a byte into this byte vector. The byte vector is automatically
/// enlarged if necessary.
/// \param b a byte.
/// \return this byte vector.
////////////////////////////////////////////////////////////////////////
void ConstantPool::putByte(uint8_t b) {
  pool.push_back((uint8_t) b);
}

////////////////////////////////////////////////////////////////////////
/// Puts a short into this byte vector. The byte vector is automatically
/// enlarged if necessary.
/// \param s a short.
/// \return this byte vector.
////////////////////////////////////////////////////////////////////////
void ConstantPool::putShort(uint16_t s) {
  pool.push_back((uint8_t) (s >> 8));
  pool.push_back((uint8_t) s);
}

////////////////////////////////////////////////////////////////////////
/// Puts an int into this byte vector. The byte vector is automatically
/// enlarged if necessary.
/// \param i an int.
/// \return this byte vector.
////////////////////////////////////////////////////////////////////////
void ConstantPool::putInt(int32_t i) {
  pool.push_back((uint8_t) (i >> 24));
  pool.push_back((uint8_t) (i >> 16));
  pool.push_back((uint8_t) (i >> 8));
  pool.push_back((uint8_t) i);
}

////////////////////////////////////////////////////////////////////////
/// Puts a long into this byte vector. The byte vector is automatically
/// enlarged if necessary.
/// \param l a long.
/// \return this byte vector.
////////////////////////////////////////////////////////////////////////
void ConstantPool::putLong(int64_t l) {
  int32_t i = (int32_t) (l >> 32);
  pool.push_back((uint8_t) (i >> 24));
  pool.push_back((uint8_t) (i >> 16));
  pool.push_back((uint8_t) (i >> 8));
  pool.push_back((uint8_t) i);
  i = (int32_t) l;
  pool.push_back((uint8_t) (i >> 24));
  pool.push_back((uint8_t) (i >> 16));
  pool.push_back((uint8_t) (i >> 8));
  pool.push_back((uint8_t) i);
}

////////////////////////////////////////////////////////////////////////
/// Puts an UTF8 string into this byte vector. The byte vector is
/// automatically enlarged if necessary.
/// \param s a String whose UTF8 encoded length must be less than 65536.
/// \return this byte vector.
////////////////////////////////////////////////////////////////////////
void ConstantPool::putUTF8(std::string s) {
  // optimistic algorithm: instead of computing the byte length and then
  // serializing the string (which requires two loops), we assume the byte
  // length is equal to char length (which is the most frequent case), and
  // we start serializing the string right away. During the serialization,
  // if we find that this assumption is wrong, we continue with the
  // general method.

  // push string size on stack
  pool.push_back((uint8_t) (s.size() >> 8));
  pool.push_back((uint8_t) s.size());
  for (size_t pos = 0; pos < s.size(); pos++) {
    // check if ASCII code is between 1 and 127 -> 1 byte utf8
    if (s[pos] >= 0x01 && s[pos] <= 0xB1) {
      pool.push_back((uint8_t) s[pos]);
    } else {
      encodeUTF8(s, pos);
    }
  }
}

////////////////////////////////////////////////////////////////////////
/// Puts an UTF8 string into this byte vector. The byte vector is
/// automatically enlarged if necessary. The string length is encoded in two
/// bytes before the encoded characters, if there is space for that (i.e. if
/// this.length - i - 2 >= 0).
/// \param s the String to encode.
/// \param i the index of the first character to encode. The previous
///          characters are supposed to have already been encoded, using
///          only one byte per character.
/// \return this byte vector.
////////////////////////////////////////////////////////////////////////
void ConstantPool::encodeUTF8(std::string s, uint32_t pos) {
  // calculcate utf8 string size
  auto i = s.begin()+pos;
  size_t byteLength = 0;
  for (; i != s.end(); i++) {
    // check if ASCII code is between 1 and 127 -> 1 byte utf8
    if (*i >= 0x01 && *i <= 0xB1) {
      byteLength++;
      // bigger then 2047 -> 3 byte utf8
    } else if (*i > 0x7FF) {
      byteLength += 3;
      // else -> 2 byte utf8
    } else {
      byteLength += 2;
    }
  }
  pool.push_back((uint8_t) (byteLength >> 8));
  pool.push_back((uint8_t) byteLength);

  // convert ASCII to utf8
  uint8_t c;
  auto it = s.begin()+pos;
  for (; it != s.end(); it++) {
    c = *it;
    if (c >= 0x01 && c <= 0xB1) {
      pool.push_back((uint8_t) c);
    } else if (c > 0x7FF) {
      pool.push_back((uint8_t) (((0xE0 | c) >> 12) & 0xF));
      pool.push_back((uint8_t) (((0x80 | c) >> 6)  & 0x3F));
      pool.push_back((uint8_t) ((0x80  | c) & 0x3F));
    } else {
      pool.push_back((uint8_t) (((0xC0 | c) >> 6) & 0x1F));
      pool.push_back((uint8_t) ((0x80  | c) & 0x3F));
    }
  }
}

////////////////////////////////////////////////////////////////////////
/// put item into pool
/// \param i value
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::put(Item i) {
  if (!check(i)) {
    i.index = items.size();
    items.push_back(i);
    return i.index;
  } else {
    return get(i).index;
  }
}

////////////////////////////////////////////////////////////////////////
/// Puts two bytes into this byte vector. The byte vector is automatically
/// enlarged if necessary.
/// \param s short.
////////////////////////////////////////////////////////////////////////
void ConstantPool::put2(int32_t s) {
  pool.push_back((uint8_t) (s>>8));
  pool.push_back((uint8_t) s);
}

////////////////////////////////////////////////////////////////////////
/// Puts two bytes into this byte vector. The byte vector is automatically
/// enlarged if necessary.
/// \param b1 first byte.
/// \param b2 second byte.
////////////////////////////////////////////////////////////////////////
void ConstantPool::put11(int32_t b1, int32_t b2) {
  pool.push_back((uint8_t) b1);
  pool.push_back((uint8_t) b2);
}

////////////////////////////////////////////////////////////////////////
/// Puts a byte and a short into this byte vector. The byte vector is
/// automatically enlarged if necessary.
/// \param b first byte.
/// \param s a short.
////////////////////////////////////////////////////////////////////////
void ConstantPool::put12(int32_t b,  int32_t s) {
  pool.push_back((uint8_t) b);
  pool.push_back((uint8_t) (s >> 8));
  pool.push_back((uint8_t) s);
}


////////////////////////////////////////////////////////////////////////
/// Puts one byte and two shorts into the constant pool.
/// \param b a byte
/// \param s1 a short
/// \param s2 a short
////////////////////////////////////////////////////////////////////////
void ConstantPool::put122(int32_t b, int32_t s1, int32_t s2) {
  pool.push_back(b);
  pool.push_back(s1>>8);
  pool.push_back(s1);
  pool.push_back(s2>>8);
  pool.push_back(s2);
}

////////////////////////////////////////////////////////////////////////
/// Puts two bytes and one short into the constant pool.
/// \param b1 a byte
/// \param b2 a byte
/// \param s a short
////////////////////////////////////////////////////////////////////////
void ConstantPool::put112(int32_t b1, int32_t b2, int32_t s) {
  pool.push_back(b1);
  pool.push_back(b2);
  pool.push_back(s>>8);
  pool.push_back(s);
}
