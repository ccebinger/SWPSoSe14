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

using namespace std;

bool testAddClassReference() {
  ConstantPool cp;
  return false;
}

/*
 *  1)	Byte
 *
 */
bool testAddByte() {
  ConstantPool cp;
  for (int i = 0; i< 256; i++) {
    cp.addByte(i);
  }
  return false;
}

/*
 *  2)	Integer
 *
 */
bool testAddInt() {
  ConstantPool cp;

  cp.addInt(32);
  cout << "test1: " << cp.addString("main.java") << endl;
  auto list = cp.getByteArray();
  int offset  = list.size();
  //  cp.addInt(0x22);
  //  cp.addInt(0x20);
  //  cp.addInt(0x10);

  //  for(int i=0; i<256;i++) {
  //	  cp.addInt(i);
  //  }
  //  	cp.addInt(0x10);

  /*
   *  3)	Integer
   *
   */
  return false;
}

/*
 *  4)	String
 *
 */
bool testAddString() {
  ConstantPool cp;

  //cp.addString("Hallo");

  string str;
  auto list = cp.getByteArray();
  auto iter = list.begin();

  for (; iter!= list.end(); iter++) {
    cout << *iter;
  }
  cout << str << endl;
  return false;
}

int main(int argc, char** argv) {
  if (!testAddClassReference()) {
    cout << "testAddClassReference failed all system off, everything falls apart ... boom\n";
  }

  if (!testAddByte()) {
    cout << "testAddByte failed all system off, everything falls apart ... boom\n";
  }

  if (!testAddInt()) {
    cout << "testAddInt failed all system off, everything falls apart ... boom\n";
  }

  if (!testAddString()) {
    cout << "testAddString failed all system off, everything falls apart ... boom\n";
  }
}

#endif
