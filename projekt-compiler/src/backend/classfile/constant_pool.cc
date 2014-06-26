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
  type = NONE;
  intVal = 0;
  longVal = 0;

  method_idx = 0;
  descriptor_idx = 0;
  name_idx = 0;
  class_idx = 0;
  name_type_idx = 0;
  index = 0;
}

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

  method_idx = i.method_idx;
  descriptor_idx = i.descriptor_idx;
  name_idx = i.name_idx;
  class_idx = i.class_idx;
  name_type_idx = i.name_type_idx;
}

////////////////////////////////////////////////////////////////////////
/// compare operator implementation
/// \param i copmared item
////////////////////////////////////////////////////////////////////////
bool Item::operator==(const Item& i)const {
  if (type != i.type)
    return false;

  switch (type) {
    case UTF8:
    case STR:
      return i.strVal1 == strVal1;
    case LONG:
      return i.longVal == longVal;
    case INT: {
      return i.intVal == intVal;
    }
    case FIELD:
    case METHOD:
      return class_idx == i.class_idx && name_type_idx == i.name_type_idx;
    case CLASS:
      return name_idx == i.name_idx;
    case NAME_AND_TYPE:
      return i.descriptor_idx == descriptor_idx && method_idx == i.method_idx;
      // case IMETHOD:
    default:
      return i.strVal1 == strVal1;
  }
}

////////////////////////////////////////////////////////////////////////
/// copy operator
/// \param i item to be copied
////////////////////////////////////////////////////////////////////////
Item& Item::operator=(const Item& i) {
  if (this != &i) {
    index = i.index;
    type = i.type;
    intVal = i.intVal;
    longVal = i.longVal;
    strVal1 = i.strVal1;
    strVal2 = i.strVal2;
    strVal3 = i.strVal3;

    method_idx = i.method_idx;
    descriptor_idx = i.descriptor_idx;
    name_idx = i.name_idx;
    class_idx = i.class_idx;
    name_type_idx = i.name_type_idx;
  }
  return *this;
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
/// set item to string value
/// \param _type type of the item
/// \param _strVal1  first part of the value of this item.
/// \param _strVal2  first part of the value of this item.
/// \param _strVal3  first part of the value of this item.
////////////////////////////////////////////////////////////////////////
void Item::set(ItemType _type, const std::string &_strVal1,
               const std::string &_strVal2, const std::string &_strVal3) {
  type = _type;
  strVal1 = _strVal1;
  strVal2 = _strVal2;
  strVal3 = _strVal3;
}

////////////////////////////////////////////////////////////////////////
/// set item to string value
/// \param type type of the item
/// \param class_idx first part of the value of this item.
/// \param name_type_idx first part of the value of this item.
////////////////////////////////////////////////////////////////////////
void Item::set(ItemType type, uint16_t class_idx, uint16_t name_type_idx) {
  this->type = type;
  this->class_idx = class_idx;
  this->name_type_idx = name_type_idx;
}

////////////////////////////////////////////////////////////////////////
/// set item to string value
/// \param type type of the item
/// \param name_idx first part of the value of this item.
////////////////////////////////////////////////////////////////////////
void Item::set(ItemType type, uint16_t name_idx) {
  this->type = type;
  this->name_idx = name_idx;
}

////////////////////////////////////////////////////////////////////////
/// set item to string value
/// \param method_idx type of the item
/// \param descriptor_idx  first part of the value of this item.
////////////////////////////////////////////////////////////////////////
void Item::set_name_type(uint16_t method_idx, uint16_t descriptor_idx) {
  this->type = ItemType::NAME_AND_TYPE;
  this->method_idx = method_idx;
  this->descriptor_idx = descriptor_idx;
}

////////////////////////////////////////////////////////////////////////
/// default constructor
////////////////////////////////////////////////////////////////////////
ConstantPool::ConstantPool() {
  items.reserve(256);
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
    putByte(INT);
    index = put(&i);
    putInt(value);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// DRAFT VERSION - NEEDS TO BE DISCUSSED / REVIEWED (because of my lack of
/// knowledge of constant_pool.cc)
/// method to put a CONSTANT_NameAndType into the pool
/// \param UTF8_name_index the index of the name in the constant pool
/// \param UTF8_descriptor_index the index of the descriptor in the
/// constant pool
/// \return index of the CONSTANT_NameAndType in the pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addNameAndType(uint16_t UTF8_name_index,
                                    uint16_t UTF8_descriptor_index) {
  Item i;
  size_t index = 0;
  i.set_name_type(UTF8_name_index, UTF8_descriptor_index);
  if (!check(i)) {
    putByte(NAME_AND_TYPE);
    putShort(UTF8_name_index);
    putShort(UTF8_descriptor_index);
    index = put(&i);
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
  i.set(UTF8, value, "", "");
  if (!check(i)) {
    putByte(UTF8);
    putUTF8(value);
    index = put(&i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a string index into the pool
/// \param string_idx value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addConstString(uint16_t &string_idx) {
  Item i;
  size_t index = 0;
  i.set(STR, string_idx);
  if (!check(i)) {
    putByte(STR);
    putShort(string_idx);
    index = put(&i);
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
size_t ConstantPool::addClassRef(uint16_t name_idx) {
  Item i;
  size_t index = 0;
  i.set(CLASS, name_idx);
  if (!check(i)) {
    putByte(CLASS);
    putShort(name_idx);
    index = put(&i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a string field into the pool
/// \param class_idx value to add to pool
/// \param value name_type_idx to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addFieldRef(uint16_t class_idx, uint16_t name_type_idx) {
  Item i;
  size_t index = 0;
  i.set(FIELD, class_idx, name_type_idx);
  if (!check(i)) {
    putByte(FIELD);
    putShort(class_idx);
    putShort(name_type_idx);
    index = put(&i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to put a string into the pool
/// \param class_idx value to add to pool
/// \param name_type_idx value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addMethRef(uint16_t class_idx, uint16_t name_type_idx) {
  Item i;
  size_t index = 0;
  i.set(METHOD, class_idx, name_type_idx);
  if (!check(i)) {
    putByte(METHOD);
    putShort(class_idx);
    putShort(name_type_idx);
    index = put(&i);
  } else {
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// method to count numbers of items with specified type
/// \param type type that be counted
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
  auto i = (std::find(items.begin(), items.end(), key));
  return *i;
}

////////////////////////////////////////////////////////////////////////
/// return if item is in constant pool
/// \param i item to find in pool
/// \return true if in pool else false
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
    if ((s[pos] >= 0x01) && (s[pos] <= 0xB1)) {
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
size_t ConstantPool::put(Item *i) {
  if (!check(*i)) {
    // constantpool counts from 1 to constant pool size -1
    // (constant pool size is size +1)
    i->index = items.size() +1;
    items.push_back(*i);
    return i->index;
  } else {
    return get(*i).index;
  }
}
