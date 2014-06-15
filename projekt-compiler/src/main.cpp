#include <iostream>
//#include <string.h>



#include <common/Env.h>
#include <frontend/lexer/Lexer.h>
#include <frontend/parser/Parser.h>
#include <frontend/Graphs.h>
#include <frontend/Parse_Exception.h>
#include <backend/backend.h>

using namespace std;

int main(int argc, char *argv[]) {

	Env::initIoDirectory();
	Env::parseParams(argc, argv);

	Graphs graphs;


	// ------------------------------------------------------------------------
	// FRONTEND
	// ------------------------------------------------------------------------

	if(Env::hasSrcFile()) {


		Env::printCaption("Frontend - Lexer");

		// Lexer
		Lexer lexer;
		lexer.lex(Env::getSrcFile());



		if (lexer.functions.size() == 0) {
			throw EnvException("Lexer: No rail functions found in " + Env::getSrcFile());
		}
		Env::showWarnings();




		// Parser
		Env::printCaption("Frontend - Parser");
		for(auto it = lexer.functions.begin(); it < lexer.functions.end(); ++it) {
			Parser p(*it);
			shared_ptr<Adjacency_list> asg = p.parseGraph();
			if(asg == NULL) {
				throw EnvException("Parser error: no ASG present. Parser report: " + p.errorMessage);
			}
			Env::showWarnings();
			graphs.put((*it)->getName(), asg);
		}
		Env::showWarnings();

	}
	else if(Env::hasSrcDeserialize()) {
		// Deserialize
		Env::printCaption("ASG - Deserialize");
		graphs.unmarshall(Env::getSrcDeserialize(), ';');
		Env::showWarnings();
	}
	else {
		throw EnvException("No source specified. Use either -i <file> or -d <file>.");
	}



	// Serialize
	if(Env::getDstSerialize() != "") {
		Env::printCaption("ASG - Serialize");
		graphs.marshall(Env::getDstSerialize(), ';');
		Env::showWarnings();
	}



	// GraphViz
	if(Env::getDstGraphviz() != "") {
		Env::printCaption("ASG - GraphViz");
		graphs.writeGraphViz(Env::getDstGraphviz());
		Env::showWarnings();
	}

	// ------------------------------------------------------------------------
	// BACKEND
	// ------------------------------------------------------------------------
	Env::printCaption("Backend");

	// TODO #118
	ofstream outFile(Env::getDstClassfile(), std::ofstream::binary);
	Backend::Generate(graphs, outFile);

	Env::showWarnings();
	return 0;
}
