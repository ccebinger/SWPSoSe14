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
#include <iostream>

////////////////////////////////////////////////////////////////////////
/// default constructor does nothing, bud is needed because we have other
//  constructor
////////////////////////////////////////////////////////////////////////
Item::Item() {
  type = NONE;
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
  switch (i.type) {
    case UTF8:
    case CLASS:
    case STR:
      return i.strVal == strVal;
    case LONG:
      return i.longVal == longVal;
    case INT: {
      std::cout << "i.int:"<< i.intVal<< "intval:"<< intVal <<"\n";
      return i.intVal == intVal;
    }
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
ConstantPool::ConstantPool() {
  items.reserve(256);
  // Java method reference for java/lang/Object
  addMethRef("java/lang/Object");
  // Java field reference for java/lang/System
  addFieldRef("java/lang/System");
  // Java method reference for Java.io.printStream.println
  addMethRef("java/io/PrintStream");
  // Java class reference Main
  addString("main");
  // add Code attribute to CP
  addString("Code");
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
    std::cout << "new int\n";
    putByte(INT);
    index = put(i);
    putInt(value);
  } else {
    std::cout << "known int\n";
    index = get(i).index;
  }
  return index;
}

////////////////////////////////////////////////////////////////////////
/// DRAFT VERSION - NEEDS TO BE DISCUSSED / REVIEWED (because of my lack of
///                                                   knowledge of constant_pool.cc)
/// method to put a CONSTANT_NameAndType into the pool
/// \param UTF8_name_index the index of the name in the constant pool
/// \param UTF8_descriptor_index the index of the descriptor in the constant pool
/// \return index of the CONSTANT_NameAndType in the pool
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addNameAndType(int32_t UTF8_name_index, int32_t UTF8_descriptor_index) {
  Item i;
  size_t index = 0;
  i.set(UTF8_name_index); // eindeutig?
  if (!check(i)) {
    putByte(NAME_AND_TYPE);
    putInt(UTF8_name_index);
    putInt(UTF8_descriptor_index);
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
size_t ConstantPool::addString(const std::string &value) {
  Item i;
  size_t index = 0;
  i.set(STR, value);
  if (!check(i)) {
    putByte(UTF8);
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
    putByte(CLASS);
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
    putByte(FIELD);
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
    putByte(METHOD);
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
    putByte(IMETHOD);
    putUTF8(value);
    index = put(i);
  } else {
    index = get(i).index;
  }
  return index;
}

/*
////////////////////////////////////////////////////////////////////////
/// ACHTUNG DRAFT !!!!!!!!!!!
/// Das ist eine Beispielumsetzung, zum Hinzufügen eines xRef (in dem Fall x = Method) in den CP
/// Kann so (noch) nicht umgesetzt werden, weil einige Signaturen im CP angepasst werden müssen
/// \param value Name der Methoden-Referenz (z.B. 'java/lang/Object')
/// \param name Name der eigentlichen Methode (z.B. '<init>')
/// \param descriptor Repräsentiert den Type der Methode (z.B. '()V')
/// \return index of the string in pool
///
/// Der Aufbau im CP sieht (beispielhaft) wie folgt aus und wird durch nachfolgenden Code repräsentiert
/// [1] CONSTANT_Methodref class_index=6; name_and_type_index=15 0A 00 06 00 0F
/// [6] CONSTANT_Class name_index=22				 07 00 16
/// [7] CONSTANT_Utf8 length=6; bytes="<init>"			 01 00 06 3C 69 6E 69 74 3E
/// [8] CONSTANT_Utf8 length=3; bytes="()V"			 01 00 03 28 29 56
/// [15] CONSTANT_NameAndType name_index=7; descriptor_index=8	 0C 00 07 00 08
/// [22] CONSTANT_Utf8 length=16; bytes="java/lang/Object"	 01 00 10 6A 61 76 61 2F 6C 61 6E 67 2F 4F 62 6A
////////////////////////////////////////////////////////////////////////
size_t ConstantPool::addIMethRef(const std::string &value, const std::string &name, const std::string &descriptor) {
	Item i;
	size_t methodRef_index = 0;
	size_t UTF8_index = 0;
	size_t UTF8_name_index addString= 0;
	size_t UTF8_descriptor_index = 0;
	size_t class_index = 0;
	size_t name_and_type_index = 0;
	i.set(MEHTOD, value);
	if (!check(i)) {
	  UTF8_index = putUTF8(value);
	  UTF8_name_index = putUTF8(name);
	  UTF8_descriptor_index = putUTF8(type);
	  class_index = addClass(indexUTF8) // u1 tag + u2 name_index
	  name_and_type_index = addNameAndType(UTF8_name_index, UTF8_descriptor_index) // u1 tag + u2 name_index + u2 descriptor_index
	  // letzter Schritt: MethodRef zusammenbauen
	  // u1 tag + u2 class_index + u2 name_and_type_index
	  put2(METHOD);
	  putInt(class_index);
	  putInt(name_and_type_index);
	  index = put(i);
	} else {
	  index = get(i).index;
	}
	  return index;
}
*/

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
