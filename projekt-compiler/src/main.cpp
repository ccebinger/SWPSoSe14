#include <iostream>
//#include <string.h>


#include <common/Env.h>
#include <frontend/lexer/Lexer.h>
#include <frontend/parser/Parser.h>
#include <frontend/Graphs.h>
#include <backend/backend.h>

using namespace std;

int main(int argc, char *argv[]) {

	Env::initTimer();

	try {
		Env::parseParams(argc, argv);
		Env::initIoDirectory();
		Env::showStatus();

		Graphs graphs;


		// ------------------------------------------------------------------------
		// FRONTEND
		// ------------------------------------------------------------------------

		if(Env::hasSrcFile()) {

			Env::printCaption("Frontend - Lexer");

			// Lexer
			Lexer lexer;
			lexer.lex(Env::getSrcFile());
			Env::showStatus();
			if(!lexer.hasFunctions()) {
				throw EnvException(FRONTEND_LEXER, "No rail functions found in " + Env::getSrcFile());
			}

			// Parser
			Env::printCaption("Frontend - Parser");
			for(auto it = lexer.functions.begin(); it < lexer.functions.end(); ++it) {
				Parser p(*it);
				shared_ptr<Adjacency_list> asg = p.parseGraph();
				if(asg == NULL) {
					throw EnvException(FRONTEND_PARSER, "No ASG to return");
				}
				graphs.put((*it)->getName(), asg);
			}
			Env::showStatus();
		}
		else if(Env::hasSrcDeserialize()) {
			// Deserialize
			Env::printCaption("ASG - Deserialize");
			graphs.unmarshall(Env::getSrcDeserialize(), ';');
			Env::showStatus();
		}
		else {
			// handled within Env.h
		}



		// ------------------------------------------------------------------------
		// ASG
		// ------------------------------------------------------------------------

		// Serialize
		if(Env::hasDstSerialize()) {
			Env::printCaption("ASG - Serialize");
			graphs.marshall(Env::getDstSerialize(), ';');
		}
		Env::showStatus();



		// GraphViz
		if(Env::hasDstGraphviz()) {
			Env::printCaption("ASG - GraphViz");
			graphs.writeGraphViz(Env::getDstGraphviz());
		}
		Env::showStatus();




		// ------------------------------------------------------------------------
		// BACKEND
		// ------------------------------------------------------------------------
		if(Env::hasDstClassfile()) {
			Env::printCaption("Backend");

			// TODO #118
			ofstream outFile(Env::getDstClassfile(), std::ofstream::binary);
			Backend::Generate(graphs, &outFile);
			Env::showStatus();
			if(Env::verbose()) {
				std::cout << "done..." << std::endl;
			}
		}



		Env::printCaption("Finished");

		Env::printBuildStatus(!Env::hasErrors());
		return Env::hasErrors();

	}
	catch(EnvException &ee) {
		Env::showStatus();
		ee.showMessage();
		Env::printBuildStatus(false);
		return 1;
	}
	catch(...) {
		Env::addError(UNKNOWN, "An unhandled exception occurred");
		Env::showStatus();
		Env::printBuildStatus(false);
		return 1;
	}

}
