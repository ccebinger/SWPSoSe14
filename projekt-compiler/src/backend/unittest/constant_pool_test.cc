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

bool testConstructor() {
  ConstantPool cp;
  auto listInit = cp.getByteArray();

  cp.addClassRef("java/lang/system");
  cp.addFieldRef("java.lang.system.out");
  cp.addMethRef("Java.io.printStream.println");
  cp.addMethRef("Main.java");

  auto listChange = cp.getByteArray();

  cout << "test add constant: "<< listInit.size()
       << "/" << listChange.size() <<endl;

  cout << "ClassRef: " << cp.countItemType(CLASS) << endl;
  cout << "MethodRef: : " << cp.countItemType(METHOD) << endl;

  return (listChange.size() == listInit.size() && listInit.size() != 0);
}

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
  std::string str;
  auto list = cp.getByteArray();
  iter = list.begin()+offset;

  bool passed = true;

  bakval = static_cast<char>(*iter++);;
  for (; iter!= (list.begin()+offset+255); iter++) {
    val = static_cast<char>(*iter);
    str += val;
    if (!(val > bakval)) {
      std::cout << val;
      passed = false;
    }
    bakval = val;
  }
} */

  /*
   *  2)	Integer
   *
   */
//  for(int i=0; i<256;i++) {
//	  cp.addInt(i);
//  }
//  	cp.addInt(0x10);
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
  if (!testConstructor()) {
    cout << "testConstructor failed all system off, everything falls apart ... boom\n";
  }

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
