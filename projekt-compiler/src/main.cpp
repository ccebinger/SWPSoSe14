#include <iostream>
#include <string.h>

// main(): frontend
//#include "frontend/frontend.cpp"

// main(): classfile_writer_test.cc
//#include "backend/classfile/constant_pool.h"
//#include "backend/classfile/classfile_writer.h"
//#include <sstream>

// main(): parseFunctions
//#include "common/parse/parseFunctions.cpp"

// main(): constant pool test
//#include "backend/unittest/constant_pool_test.cc"

// main(): backend
//#include "backend/main.cc"

using namespace std;


int main(int argc, char *argv[]) {

	bool runFrontend = false;
	bool runBackend = false;
	bool runCwt = false;
	bool runParseFunctions = false;
	bool runConstantPoolTest = false;


	for(int i=0; i<argc; i++) {
		if(strcmp(argv[i], "-f") == 0) {
			runFrontend = true;
		}
		else if(strcmp(argv[i], "-b") == 0) {
			runBackend = true;
		}
		else if(strcmp(argv[i], "-cwt") == 0) {
			runCwt = true;
		}
		else if(strcmp(argv[i], "-pf") == 0) {
			runParseFunctions = true;
		}
		else if(strcmp(argv[i], "-cpt") == 0) {
			runConstantPoolTest = true;
		}


		cout << argv[i] << endl;
	}


	cout << "-------------------" << endl;
	cout << "-f: " << runFrontend << endl;
	cout << "-b: " << runBackend << endl;


	if(runFrontend) {
		// Alte main() aus frontend
//		unmarshallGraph("src/frontend/test_ast.csv", ';');
//		std::cout << std::endl << std::endl;
//		unmarshallGraph("src/frontend/test2.csv", ';');
//		return 0;
	}


	if(runCwt) {
		// Alte main() aus classfile_writer_test.cc
//		ConstantPool cp;
//		std::ostringstream os;
//		ClassfileWriter cs(ClassfileWriter::ClassfileVersion::JAVA_7, cp, os);
	}

	if(runParseFunctions) {
		//oldParseFunctionMain(argc, argv);
	}

	if(runConstantPoolTest) {
//		oldMainConstantPoolTest(argc, argv);
	}


	if(runBackend) {
//		oldBackendMain();
	}



	return 0;
}
