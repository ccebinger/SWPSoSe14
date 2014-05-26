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
  std::cout << "test1: " << cp.addString("main.java") << std::endl;
  std::vector<uint8_t> list = cp.getByteArray();
  int offset  = list.size();
  //  cp.addInt(0x22);
//  cp.addInt(0x20);
//  cp.addInt(0x10);

  /*
   *  1)	Byte
   *
   */
  for (int i = 0; i< 256; i++) {
    cp.addByte(i);
  }

  /*
   *  2)	Integer
   *
   */
//  for(int i=0; i<256;i++) {
//	  cp.addInt(i);
//  }
//  	cp.addInt(0x10);

  /*
   *  3)	Integer
   *
   */

  /*
   *  4)	String
   *
   */
  //cp.addString("Hallo");

  std::string str;
  list = cp.getByteArray();
  auto iter = list.begin();

  for (; iter!= list.end(); iter++) {
    std::cout << *iter;
  }
  std::cout << str << std::endl;
}

#endif
