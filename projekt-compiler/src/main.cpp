#include <iostream>
//#include <string.h>

#include <common/Env.h>
#include <frontend/lexer/Lexer.h>
#include <frontend/parser/Parser.h>
#include <frontend/Graphs.h>
#include <backend/backend.h>

using namespace std;

int main(int argc, char *argv[]) {
	try {

		Env::initIoDirectory();
		Env::parseParams(argc, argv);
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
			throw EnvException(ENVIRONMENT, "No source specified. Use either -i <file> or -d <file>.");
		}



		// ------------------------------------------------------------------------
		// ASG
		// ------------------------------------------------------------------------

		// Serialize
		if(Env::getDstSerialize() != "") {
			Env::printCaption("ASG - Serialize");
			graphs.marshall(Env::getDstSerialize(), ';');
		}
		Env::showStatus();



		// GraphViz
		if(Env::getDstGraphviz() != "") {
			Env::printCaption("ASG - GraphViz");
			graphs.writeGraphViz(Env::getDstGraphviz());
		}
		Env::showStatus();


		// ------------------------------------------------------------------------
		// BACKEND
		// ------------------------------------------------------------------------
		Env::printCaption("Backend");

		// TODO #118
		ofstream outFile(Env::getDstClassfile(), std::ofstream::binary);
		Backend::Generate(graphs, &outFile);
		Env::showStatus();
		if(Env::verbose()) {
			std::cout << "done..." << std::endl;
		}



		if(Env::hasErrors()) {
			if(Env::verbose()) {
				std::cout << "Build error(s) occurred" << std::endl;
			}
			return 1;
		}
		else {
			if(Env::verbose()) {
				std::cout << "Build successfull" << std::endl;
			}
			return 0;
		}


	}
	catch(EnvException &ee) {
		ee.showMessage();
		std::cout << "Build error(s) occurred" << std::endl;
		return 1;
	}
	catch(...) {
		Env::addError(UNKNOWN, "An unhandled exception occurred");
		Env::showStatus();
		std::cerr << "Build error(s) occurred" << std::endl;
		return 1;
	}

}
