#include <iostream>
#include <string.h>
//#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#include <frontend/lexer/Lexer.h>
#include <frontend/parser/Parser.h>
#include <frontend/Graphs.h>
#include <frontend/Parse_Exception.h>
#include <backend/backend.h>



using namespace std;


int main(int argc, char *argv[]) {

	// ------------------------------------------------------------------------
	// FRONTEND
	// ------------------------------------------------------------------------


	// Ensure existence of folder io
	if(access("io", F_OK) == -1) {
#ifdef _WIN32
		int rMkdir = mkdir("io");
#else
		int rMkdir = mkdir("io", 0777);
#endif
		if(rMkdir == -1) {
			IO_Exception ie;
			ie.set_file("io");
			throw ie;
		}
	}


	string srcFile = "";
	string srcDeserialize = "";
	string dstSerialize = "";
	string dstGraphviz = "";


	if(argc <= 1) {
		srcFile = "test-cases/helloworld.txt";
		dstSerialize = "io/serialized.csv";
		dstGraphviz = "io/graphviz.dot";
		cout << "No parameters set, using defaults:" << endl;
		cout << " -i " << srcFile << endl;
		cout << " -s " << dstSerialize << endl;
		cout << " -g " << dstGraphviz << endl;
	}

	// Parse parameters
	for(int i=0; i<argc; ++i) {

		if(strcmp(argv[i], "-i") == 0) {
			srcFile = argv[++i];
		}
		else if(strcmp(argv[i], "-d") == 0) {
			srcDeserialize = argv[++i];
		}
		else if(strcmp(argv[i], "-s") == 0) {
			dstSerialize = argv[++i];
		}
		else if(strcmp(argv[i], "-g") == 0) {
			dstGraphviz = argv[++i];
		}
		else if(strcmp(argv[i], "-h") == 0) {
			cout << "Command line help:" << endl;
			cout << " -i <file> specifies input sourcefile" << endl;
			cout << " -s <file> serializes graph to <file>" << endl;
			cout << " -d <file> deserialize csv to graph" << endl;
			cout << " -g <file> create graphViz" << endl;
			cout << " -h help" << endl;
			cout << "General:" << endl;
			cout << " -d > -i" << endl;
		}
	}


	cout << endl << "Starting..." << endl;



	Graphs graphs;


	// Deserialize
	// csv-src file & accessable
	if(srcDeserialize != "") {
		if(access(srcDeserialize.c_str(), F_OK) == -1) {
			cerr << "Csv-File not accessble: " << srcDeserialize << endl;
			return -1;
		}
		graphs.unmarshall(srcDeserialize, ';');
	}
	else if(srcFile != "") {
		if(access(srcFile.c_str(), F_OK) == -1) {
			cerr << "Sourcefile not accessble: " << srcFile << endl;
			return -1;
		}
		cout << "Reading Sourcefile " << srcFile << endl;

		// Lexer
		Lexer lexer;
		lexer.lex(srcFile);
		if(lexer.functions.size() == 0) {
			//FIXME error handling -> Exception
			std::cout << "No Rail Functions found in " << srcFile << std::endl;
			return -1;
		}

		// Parser
		/*
		std::shared_ptr<RailFunction> func = lexer.functions.at(0); //FIXME hardcoded number of functions
		Parser p(func);
		shared_ptr<Adjacency_list> asg = p.parseGraph();
		if(asg == NULL) {
			Parse_Exception pe;
			pe.set_msg(p.errorMessage);
			throw pe;
		}
		graphs.put(func->getName(), asg);
		*/


		// Parser
		for(auto it=lexer.functions.begin(); it < lexer.functions.end(); ++it) {
			Parser p(*it);
			shared_ptr<Adjacency_list> asg = p.parseGraph();
			if(asg == NULL) {
				//FIXME error handling!
				Parse_Exception pe;
				pe.set_msg(p.errorMessage);
				throw pe;
			}
			graphs.put((*it)->getName(), asg);
		}


	}
	else {
		cerr << "No source specified. Use either -i <file> or -d <file>." << endl;
	}






	// Serialize
	if(dstSerialize != "") {
		graphs.marshall(dstSerialize, ';');
	}


	// GraphViz
	if(dstGraphviz != "") {
		graphs.writeGraphViz(dstGraphviz);
	}

	// ------------------------------------------------------------------------
	// BACKEND
	// ------------------------------------------------------------------------

	// TODO this is just a mockup...
	cout << "--- Begin Backend ------------------------------------------------" << endl;
	ofstream outFile;
	outFile.open("io/out.class");
	Backend::Generate(graphs, outFile);

	return 0;
}
