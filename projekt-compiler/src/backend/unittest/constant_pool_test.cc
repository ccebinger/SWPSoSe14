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

#if TESTS

#include <backend/classfile/constant_pool.h>
#include <backend/classfile/classfile_writer.h>
#include <sstream>
#include <iostream>

int main(int argc, char** argv) {
  ConstantPool cp;
  std::ostringstream os;

  cp.addInt(32);
  std::vector<uint8_t> list = cp.getByteArray();
  std::string str;

  auto iter = list.begin();
  for (; iter!= list.end(); iter++) {
    str+=static_cast<char>(*iter);
  }

  std::cout << "test: " << str << ", length: " << list.size() << std::endl;
}

#endif
