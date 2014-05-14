#include <iostream>
#include <string.h>

#include <frontend/parse/lexer.h>
#include <frontend/Parser.h>
#include <frontend/Graphs.h>
#include <frontend/Parse_Exception.h>
#include <unistd.h>

using namespace std;


int main(int argc, char *argv[]) {

	// ------------------------------------------------------------------------
	// FRONTEND
	// ------------------------------------------------------------------------

	string srcFile = "";
	string srcDeserialize = "";
	string dstSerialize = "";
	string dstGraphviz = "";

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
		if(access(dstSerialize.c_str(), F_OK) == -1) {
			cerr << "Csv-File not accessble: " << dstSerialize << endl;
			return -1;
		}

		graphs.unmarshall("out.csv", ';');
	}
	else if(srcFile != "") {
		if(access(srcFile.c_str(), F_OK) == -1) {
			cerr << "Sourcefile not accessble: " << srcFile << endl;
			return -1;
		}

		// Lexer
		Lexer lexer;
		lexer.lex(srcFile);
		RailFunction func = lexer.functions.at(0); //FIXME hardcoded number of functions


		// "Parser"
		BoardContainer board{func.code, MAX_CHARS_PER_LINE, MAX_LINES_PER_FUNCTION};
		Parser p(board, func.getName());
		shared_ptr<Adjacency_list> asg = p.parseGraph();
		if(asg == NULL) {
		Parse_Exception pe;
		pe.set_msg(p.errorMessage);
			throw pe;
		}

		// Create Graphs
		graphs.put(func.getName(), asg);
	}
	else {
		cerr << "No source specified. Use either -i <file> or -d <file>." << endl;
	}






	// Serialize
	if(dstSerialize != "") {
		if(access(dstSerialize.c_str(), F_OK) == -1) {
			cerr << "File for serialization not accessable, skipping: " << dstSerialize << endl;
		}
		else {
			graphs.marshall(dstSerialize);
		}
	}


	// GraphViz
	if(dstGraphviz != "") {
		if(access(dstGraphviz.c_str(), F_OK) == -1) {
			cerr << "GraphViz-file not accessable, skipping: " << dstSerialize << endl;
		}
		else {
			graphs.writeGraphViz(dstGraphviz);
		}

	}

	// ------------------------------------------------------------------------
	// BACKEND
	// ------------------------------------------------------------------------





	return 0;
}
