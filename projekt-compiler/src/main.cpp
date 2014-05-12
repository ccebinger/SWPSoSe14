#include <iostream>
#include <string.h>

#include <frontend/parse/lexer.h>
#include <frontend/Parser.h>


using namespace std;


int main(int argc, char *argv[]) {
	// ------------------------------------------------------------------------
	// FRONTEND
	// ------------------------------------------------------------------------

	// Lexer
	//FIXME hardcoded. must be provided by commandline
	Lexer lexer;
	lexer.lex("../src/test-cases/helloworld.txt");
	RailFunction func = lexer.functions.at(0); //FIXME hardcoded number of functions


	// "Parser"
	Graphs graphs;
	BoardContainer board{func.code, MAX_CHARS_PER_LINE, MAX_LINES_PER_FUNCTION};
	Parser p(board, func.getName());
	shared_ptr<Adjacency_list> asg = p.parseGraph();
	if(asg == NULL) {
		throw "Parser error " + p.errorMessage + "";
	}


	// Serialize
	graphs.put(func.getName(), asg);
	graphs.marshall("out.csv");


	// Deserialize
	Graphs sndGraphs;
	sndGraphs.unmarshall("out.csv", ';'); //FIXME fix delimiter





	// ------------------------------------------------------------------------
	// BACKEND
	// ------------------------------------------------------------------------





	return 0;
}
