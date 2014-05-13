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

ConstantPool::ConstantPool() {
}

ConstantPool::~ConstantPool() {
}

uint16_t ConstantPool::addString(std::string str) {
  (void) str;
  return 0;
}

uint16_t ConstantPool::addInt(int32_t i) {
  (void) i;
  return 0;
}

uint16_t ConstantPool::addLong(int64_t l) {
  (void) l;
  return 0;
}

uint16_t ConstantPool::addFloat(float f) {
  (void) f;
  return 0;
}

uint16_t ConstantPool::addDouble(double d) {
  (void) d;
  return 0;
}

// Commented out, needed? --jdettlaff
//uint16_t ConstantPool::addConstant() {
//  return 0;
//}

std::vector<uint8_t> ConstantPool::getByteArray() {
  std::vector<uint8_t> cp;
  return cp;
}
