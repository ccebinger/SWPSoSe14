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
unsigned int cp_offset = 0;

bool testConstructor() {
  ConstantPool cp;
  auto listInit = cp.getByteArray();
  cp_offset = listInit.size();
  cp.addClassRef("java/lang/system");
  cp.addFieldRef("java.lang.system.out");
  cp.addMethRef("Java.io.printStream.println");
  cp.addMethRef("Main.java");

  auto listChange = cp.getByteArray();

  cout << "[INFO] " << "test add constant: "<< listInit.size()
       << "/" << listChange.size() << endl;

  cout << "ClassRef: " << cp.countItemType(CLASS) << endl;
  cout << "MethodRef: : " << cp.countItemType(METHOD) << endl;
  return (listChange.size() == listInit.size() && listInit.size() > 0);
}

bool testAddClassReference() {
  /*
    dont know, what to test here 
  */
  ConstantPool cp;
  return false;
}

/*
<<<<<<< HEAD
 *  1)	Byte
 *			got obsolete, due addByte is not implemented anymore…
 *  2)
 */
bool testAddInt() {
  /*
  confused, addbyte has no effects anymore
  */
  ConstantPool cp;
  
	cp.addInt(0xcafebabe);
  cp.addInt(0xdeadbeef);

	auto list = cp.getByteArray();
  auto iter = list.begin();

  bool passed = true;

  string str;
  char bakval = static_cast<char>(*iter++);;
  for (; iter != list.end(); iter++) {
    char val = static_cast<char>(*iter);
    str += val;
    if (!(val > bakval)) {
      //cout << val;
      passed = false;
    }
    bakval = val;
  }
  cout << "[INFO] constant pool string " << str << endl;
  return passed;
}

=======
 *  2)	Integer
 *
 */
bool testAddInt() {
  ConstantPool cp;

  cp.addInt(32);
  auto list = cp.getByteArray();
  int offset  = list.size();
  cp.addInt(0x22);
  cp.addInt(0x20);
  cp.addInt(0x10);

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
>>>>>>> ddcb2ed3ecd111ac67b4a3850b683486297125e7

/*
 *  4)	String
 *
 */
bool testAddString() {
  ConstantPool cp;

}

int main(int argc, char** argv) {
  if (!testConstructor()) {
    cerr << "[ERROR] " << "testConstructor failed all system off, everything falls apart ... boom" << endl;
  }

/*  if (!testAddClassReference()) {
    cerr << "[ERROR] " << "testAddClassReference failed all system off, everything falls apart ... boom" << endl;
  } */

<<<<<<< HEAD
  if (!testAddInt()) {
=======
/*  if (!testAddInt()) {
>>>>>>> ddcb2ed3ecd111ac67b4a3850b683486297125e7
    cerr << "[ERROR] " << "testAddInt failed all system off, everything falls apart ... boom" << endl;
	}

/*  if (!testAddString()) {
    cout << "[ERROR] " << "testAddString failed all system off, everything falls apart ... boom" << endl;
  }*/
}

#endif
