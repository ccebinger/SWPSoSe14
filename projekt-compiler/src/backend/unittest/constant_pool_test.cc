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
 *  1)	Byte
 *			got obsolete, due addByte is not implemented anymore…
 *  2)
 */
struct ConstantPoolTest:public ConstantPool{
  bool testAddInt() {
    addInt(0xcafebabe);
    addInt(0xdeadbeef);

    auto list = getByteArray();
    auto iter = list.begin();

    for (auto i = items.begin(); i != items.end(); i++) {
    }
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
};
/*
 *  4)	String
 *
 */
bool testAddString() {
  // initialisiere cases
  std::vector<string> cases = {"Clemens", "Maurice", "Paul", "Till",
                               "Leon", "Miro", "Jonas", "Sandra",
                               "Christopher", "Sascha" ,"Vincent", "ACME",
                               "TertiumNonDatur", "Kellerspeicher", "UniverseOfDiscourse", "SeparationOfConcerns",
                               "kontextfrei", "links-regulaer", "comma-separated-values", "\r\n\t\t\tFUB"};

  ConstantPool cp;
  auto list = cp.getByteArray();
  int off = list.size();

  for(auto zeiger=cases.begin(); zeiger!=cases.end(); zeiger++) {
    cp.addString((*zeiger));
  }
  list = cp.getByteArray();
  auto iter = list.begin()+off;

  bool passed = true;

  string str;
  //for (; iter != list.end(); iter++) {
  //    char val = static_cast<char>(*iter);
  //  str += val;
  //}

  for (auto zeiger = cases.begin(); zeiger != cases.end(); zeiger++) {
    string in =  *zeiger;
    string out;
    int l = (*zeiger).length();
    int i = 0;
    for (; i < l; i++) {
      char val = static_cast<char>(*(iter++));
      out += val;
    }
    cout << in << "vs." << out << " [" << i << " / " << l << "] " << endl;
    str+=out;
  }
  cout << "[INFO] constant pool string " << str << endl;

  return passed;
}

int main(int argc, char** argv) {
  if (!testConstructor()) {
    cerr << "[ERROR] " << "testConstructor failed all system off, everything falls apart ... boom" << endl;
  }

  /*  if (!testAddClassReference()) {
      cerr << "[ERROR] " << "testAddClassReference failed all system off, everything falls apart ... boom" << endl;
      } */

  ConstantPoolTest intTest;
  if (!intTest.testAddInt()) {
    cerr << "[ERROR] " << "testAddInt failed all system off, everything falls apart ... boom" << endl;
  }

  if (!testAddString()) {
    cout << "[ERROR] " << "testAddString failed all system off, everything falls apart ... boom" << endl;
  }
}

#endif
