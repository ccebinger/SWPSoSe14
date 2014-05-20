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
ConstantPool::ConstantPool():index(1), key(0), items(256) {
}

////////////////////////////////////////////////////////////////////////
/// method to put a string into the pool
/// \param value value to add to pool
/// \return index of the string in pool
////////////////////////////////////////////////////////////////////////
uint16_t ConstantPool::addString(const std::string &value) {
  key.set(STR, value, "", "");
  // Item result = get(key2);
  // if (result == NULL) {
  //   //pool.put12(STR, newUTF8(value));
  //   result = new Item(index++, key2);
  //   put(result);
  // }
  // return index;
  return 0;
}

////////////////////////////////////////////////////////////////////////
/// method to put a integer into the pool
/// \param value value to add to pool
/// \return index of the integer in the pool
////////////////////////////////////////////////////////////////////////
uint16_t ConstantPool::addInt(int32_t value) {
  key.set(value);
  // Item result = get(key);
  // if (result == NULL) {
  pool.push_back(uint8_t(value>>24));
  pool.push_back(uint8_t(value>>16));
  pool.push_back(uint8_t(value>>8));
  pool.push_back(uint8_t(value));
  //result = new Item(index++, key);
  key.index = index++;
  put(key);
  // }
  // return index;
  return 0;
}

////////////////////////////////////////////////////////////////////////
/// method to put a long int into the pool
/// \param value value to add to pool
/// \return index of the long intenger in the pool
////////////////////////////////////////////////////////////////////////
uint16_t ConstantPool::addLong(int64_t value) {
  key.set(value);
  // Item result = get(key);
  // if (result == NULL) {
  //   pool.putByte(LONG).putLong(value);
  //   result = new Item(index, key);
  //   index += 2;
  //   put(result);
  // }
  // return index;
  return 0;
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
  while (i != items.end() && (i->type != key.type || !(key == *i))) {
    //(*i).set(i->next);
  }
  return i->index;
}

////////////////////////////////////////////////////////////////////////
/// put item into pool
/// \param i value
////////////////////////////////////////////////////////////////////////
void ConstantPool::put(const Item &i) {
  Item item(i);
  //if (index > threshold) {
    int ll = items.size();
    int nl = ll * 2 + 1;
    std::vector<Item> newItems(nl);
    for (int l = ll - 1; l >= 0; --l) {
      Item *j = &items[l];
      while (j != nullptr) {
        int index = j->index;
        Item *k = j->next;
        j->next = &newItems[index];
        //newItems[index] = &j;
        j = k;
      }
    }
    items = newItems;
    //threshold = (int) (nl * 0.75);
    //}
  item.next = &items[index++];
  items[index] = item;
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
