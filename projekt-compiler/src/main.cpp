#include <iostream>
#include <string.h>

// main(): frontend
#include <frontend/frontend.h>

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
	bool runParseFunctions = false;

	for(int i=0; i<argc; i++) {
		if(strcmp(argv[i], "-f") == 0) {
			runFrontend = true;
		}
		else if(strcmp(argv[i], "-b") == 0) {
			runBackend = true;
		}
		else if(strcmp(argv[i], "-pf") == 0) {
			runParseFunctions = true;
		}

		cout << argv[i] << endl;
	}


	cout << "-------------------" << endl;
	cout << "-f: " << runFrontend << endl;
	cout << "-b: " << runBackend << endl;





	if(runFrontend) {
		// Alte main() aus frontend
		unmarshallGraph("src/frontend/test_ast.csv", ';');
		std::cout << std::endl << std::endl;
		unmarshallGraph("src/frontend/test2.csv", ';');
		return 0;
	}



	if(runParseFunctions) {
		//oldParseFunctionMain(argc, argv);
	}


	if(runBackend) {
//		oldBackendMain();
	}



	return 0;
}
