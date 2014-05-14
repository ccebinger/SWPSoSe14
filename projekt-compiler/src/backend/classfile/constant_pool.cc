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

Item::Item(const Item &i) {
  index = i.index;
  type = i.type;
  intVal = i.intVal;
  longVal = i.longVal;
  strVal1 = i.strVal1;
  strVal2 = i.strVal2;
  strVal3 = i.strVal3;
  hashCode = i.hashCode;
}

Item::Item(uint16_t _index, const Item &i) {
  index = _index;
  type = i.type;
  intVal = i.intVal;
  longVal = i.longVal;
  strVal1 = i.strVal1;
  strVal2 = i.strVal2;
  strVal3 = i.strVal3;
  hashCode = i.hashCode;
}

bool Item::operator==(const Item& i) {
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

bool Item::operator=(const Item& i) {
  index = i.index;
  type = i.type;
  intVal = i.intVal;
  longVal = i.longVal;
  strVal1 = i.strVal1;
  strVal2 = i.strVal2;
  strVal3 = i.strVal3;
  hashCode = i.hashCode;
  return true;
}


void Item::set(int32_t _intVal) {
  type = INT;
  intVal = _intVal;
  hashCode = 0x7FFFFFFF & (type + _intVal);
}

void Item::set(int64_t _longVal) {
  type = LONG;
  longVal = _longVal;
  hashCode = 0x7FFFFFFF & (type + (int32_t) _longVal);
}

void Item::set(int32_t  _type, const std::string &_strVal1, const std::string &_strVal2,
         const std::string &_strVal3) {
  type = _type;
  strVal1 = _strVal1;
  strVal2 = _strVal2;
  strVal3 = _strVal3;
  switch (type) {
    case STR: {
      hashCode = 0x7FFFFFFF & (type + hash_fn(strVal1));
      return;
    }
    default:
      hashCode = 0x7FFFFFFF & (type + hash_fn(strVal1)
                               * hash_fn(strVal2) * hash_fn(strVal3));
  }
}

ConstantPool::ConstantPool():index(1),key(0),key2(0),items(256) {
}

uint16_t ConstantPool::addString(const std::string &value) {
  key2.set(STR, value, "", "");
  // Item result = get(key2);
  // if (result == NULL) {
  //   //pool.put12(STR, newUTF8(value));
  //   result = new Item(index++, key2);
  //   put(result);
  // }
  // return index;
  return 0;
}

uint16_t ConstantPool::addInt(int32_t value) {
  key.set(value);
  // Item result = get(key);
  // if (result == NULL) {
  //   pool.putByte(INT).putInt(value);
  //   result = new Item(index++, key);
  //   put(result);
  // }
  // return index;
  return 0;
}

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

std::vector<uint8_t> ConstantPool::getByteArray() {
  std::vector<uint8_t> cp;
  return cp;
}

const Item& ConstantPool::get(const Item &key) const{
  // Item i = items[key.hashCode % items.size()];
  // while (i != NULL && (i.type != key.type || !(key == i))) {
  //   i = i->next;
  // }
  // return i;
  Item i;
  return i;
}

void ConstantPool::put(Item i) {
  if (index > threshold) {
    int ll = items.size();
    int nl = ll * 2 + 1;
    std::vector<Item> newItems(nl);
    for (int l = ll - 1; l >= 0; --l) {
      Item *j = &items[l];
      while (j != nullptr) {
        int index = j->hashCode % newItems.size();
        Item *k = j->next;
        j->next = &newItems[index];
        //newItems[index] = &j;
        j = k;
      }
    }
    items = newItems;
    threshold = (int) (nl * 0.75);
  }
  int index = i.hashCode % items.size();
  i.next = &items[index];
  items[index] = i;
}

void ConstantPool::put122(int32_t b, int32_t s1, int32_t s2) {
  pool.push_back(b);
  pool.push_back(s1>>8);
  pool.push_back(s1);
  pool.push_back(s2>>8);
  pool.push_back(s2);
}

void ConstantPool::put112(int32_t b1, int32_t b2, int32_t s) {
  pool.push_back(b1);
  pool.push_back(b2);
  pool.push_back(s>>8);
  pool.push_back(s);
}
