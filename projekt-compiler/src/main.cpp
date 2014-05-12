#include <iostream>
#include <string.h>

#include <common/parse/parseFunctions.cpp>



using namespace std;


int main(int argc, char *argv[]) {

//	bool runFrontend = false;
//	bool runBackend = false;
//	bool runParseFunctions = false;
//
//	for(int i=0; i<argc; i++) {
//		if(strcmp(argv[i], "-f") == 0) {
//			runFrontend = true;
//		}
//		else if(strcmp(argv[i], "-b") == 0) {
//			runBackend = true;
//		}
//		else if(strcmp(argv[i], "-pf") == 0) {
//			runParseFunctions = true;
//		}
//
//		cout << argv[i] << endl;
//	}
//
//
//	cout << "-------------------" << endl;
//	cout << "-f: " << runFrontend << endl;
//	cout << "-b: " << runBackend << endl;
//
//
//	if(runFrontend) {
//		// Alte main() aus frontend
//		unmarshallGraph("src/frontend/test_ast.csv", ';');
//		std::cout << std::endl << std::endl;
//		unmarshallGraph("src/frontend/test2.csv", ';');
//		return 0;
//	}
//
//
//
//	if(runParseFunctions) {
//		//
//	}
//
//
//	if(runBackend) {
////		oldBackendMain();
//	}


	// lex source file
	vector<RailFunction> parsed = oldParseFunctionMain(argc, argv);


	// parse source































	return 0;
}
