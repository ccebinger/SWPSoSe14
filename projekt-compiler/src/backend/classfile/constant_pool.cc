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

unsigned ConstantPool::addString(std::string str) {
  return 0;
}

unsigned ConstantPool::addInt(int32 i) {
  return 0;
}

unsigned ConstantPool::addLong(int64 l) {
  return 0;
}

unsigned ConstantPool::addFloat(float f) {
  return 0;
}

unsigned ConstantPool::addDouble(double d) {
  return 0;
}

unsigned ConstantPool::addConstant() {
  return 0;
}

std::vector<char> ConstantPool::getByteArray() {
  std::vector<char> cp;
  return cp;
}
