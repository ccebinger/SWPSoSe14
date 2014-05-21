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
  strVal1 = i.strVal1;
  strVal2 = i.strVal2;
  strVal3 = i.strVal3;
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
  strVal1 = i.strVal1;
  strVal2 = i.strVal2;
  strVal3 = i.strVal3;
}

////////////////////////////////////////////////////////////////////////
/// compare operator implementation
/// \param i copmared item
////////////////////////////////////////////////////////////////////////
bool Item::operator==(const Item& i)const {
  switch (type) {
    case STR:
      return i.strVal1 == strVal1;
    case LONG:
      return i.longVal == longVal;
    case INT:
      return i.intVal == intVal;
    default:
      return i.strVal1 == strVal1 && i.strVal2 == strVal2
          && i.strVal3 == strVal3;
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
  strVal1 = i.strVal1;
  strVal2 = i.strVal2;
  strVal3 = i.strVal3;
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
/// \param _strVal1  first part of the value of this item.
/// \param _strVal2  second part of the value of this item.
/// \param _strVal3  third part of the value of this item.
////////////////////////////////////////////////////////////////////////
void Item::set(int32_t  _type,
               const std::string &_strVal1,
               const std::string &_strVal2,
               const std::string &_strVal3) {
  type = _type;
  strVal1 = _strVal1;
  strVal2 = _strVal2;
  strVal3 = _strVal3;
}

////////////////////////////////////////////////////////////////////////
/// default constructor
////////////////////////////////////////////////////////////////////////
ConstantPool::ConstantPool(): items(256) {
  //FIXME: push constants
}

////////////////////////////////////////////////////////////////////////
/// method to put a string into the pool
/// \param value value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addString(const std::string &value) {
  Item key;
  key.set(STR, value, "", "");
  Item result = get(key);
  // if (result == NULL) {
  //   pool.put12(STR, newUTF8(value));
  //   result = new Item(index++, key2);
  //   put(result);
  // }
  // return index;
  return key.index;
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
  int i = (int32_t) (l >> 32);
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
  pool.push_back((uint8_t) (s.size() >> 8));
  pool.push_back((uint8_t) s.size());
  for (int i = 0; i < s.size(); ++i) {
    char c = s[i];
    if (c >= '\001' && c <= '\177') {
      pool.push_back((uint8_t) c);
    // } else {
    //   length = len;
    //   return encodeUTF8(s, i, 65535);
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
/// \param maxByteLength the maximum byte length of the encoded string,
///        including the already encoded characters.
/// \return this byte vector.
////////////////////////////////////////////////////////////////////////
void ConstantPool::encodeUTF8(std::string s, int32_t i,
                                  int32_t maxByteLength) {
  // int charLength = s.length();
  // int byteLength = i;
  // char c;
  // for (int j = i; j < s.size(); ++j) {
  //   c = s[j];
  //   if (c >= '\001' && c <= '\177') {
  //     byteLength++;
  //   } else if (c > '\u07FF') {
  //     byteLength += 3;
  //   } else {
  //     byteLength += 2;
  //   }
  // }
  // if (byteLength > maxByteLength) {
  //   throw std::invalid_argument("encodeUTF8 IllegalArgumentException string is bigger then constant pool");
  // }
  // int start = pool.size() - i - 2;
  // if (start >= 0) {
  //   pool[start] = (uint8_t) (byteLength >> 8);
  //   pool[start + 1] = (uint8_t) byteLength;
  // }
  // int len = pool.size();
  // for (int j = i; j < charLength; ++j) {
  //   c = s[j];
  //   if (c >= "\001" && c <= "\177") {
  //     pool.push_back((uint8_t) c);
  //   } else if (c > "\u07FF") {
  //     pool.push_back((uint8_t) (0xE0 | c >> 12 & 0xF));
  //     pool.push_back((uint8_t) (0x80 | c >> 6 & 0x3F));
  //     pool.push_back((uint8_t) (0x80 | c & 0x3F));
  //   } else {
  //     pool.push_back((uint8_t) (0xC0 | c >> 6 & 0x1F));
  //     pool.push_back((uint8_t) (0x80 | c & 0x3F));
  //   }
  // }
}

////////////////////////////////////////////////////////////////////////
/// method to put a integer into the pool
/// \param value value to add to pool
/// \return index of the integer in the pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addInt(int32_t value) {
  Item key(value);
  Item result = get(key);
  // if (result) {
  pool.push_back(uint8_t(value>>24));
  pool.push_back(uint8_t(value>>16));
  pool.push_back(uint8_t(value>>8));
  pool.push_back(uint8_t(value));
  //result = new Item(index++, key);
  key.index = items.size() + 1;
  put(key);
  // }
  return items.size();
}

////////////////////////////////////////////////////////////////////////
/// method to put a long int into the pool
/// \param value value to add to pool
/// \return index of the long intenger in the pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addLong(int64_t value) {
  Item key;
  key.set(value);
  Item result = get(key);
  // if (result == NULL) {
  //   pool.putByte(LONG).putLong(value);
  //   result = new Item(index, key);
  //   index += 2;
  //   put(result);
  // }
  return items.size();
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
/// \param i item to find in pool
/// \return index of item or zero if not in pool
////////////////////////////////////////////////////////////////////////
const Item& ConstantPool::get(const Item &key) const {
  auto i = std::find(items.begin(), items.end(), key);
  return *i;
}

////////////////////////////////////////////////////////////////////////
/// put item into pool
/// \param i value
////////////////////////////////////////////////////////////////////////
void ConstantPool::put(Item i) {
  // FIXME: check if item is in items
  get(i);

  i.index = items.size();
  items.push_back(i);
}

////////////////////////////////////////////////////////////////////////
/// Puts two bytes into this byte vector. The byte vector is automatically
/// enlarged if necessary.
/// \param b1 first byte.
/// \param b2 second byte.
/// \return this byte vector.
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
/// \return this byte vector.
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
