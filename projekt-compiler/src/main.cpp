#include <iostream>
#include <string.h>

#include <frontend/parse/lexer.h>
#include <frontend/Parser.h>
#include <frontend/Graphs.h>
#include <frontend/Parse_Exception.h>

using namespace std;


int main(int argc, char *argv[]) {
	// ------------------------------------------------------------------------
	// FRONTEND
	// ------------------------------------------------------------------------

	// Lexer
	//FIXME hardcoded. must be provided by commandline
	Lexer lexer;
	lexer.lex("test-cases/helloworld.txt");
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
	Graphs graphs;
	graphs.put(func.getName(), asg);


	// Serialize
	graphs.marshall("out.csv");


	// Deserialize
	Graphs sndGraphs;
	sndGraphs.unmarshall("out.csv", ';'); //FIXME fix delimiter



	// ------------------------------------------------------------------------
	// BACKEND
	// ------------------------------------------------------------------------





	return 0;
}
