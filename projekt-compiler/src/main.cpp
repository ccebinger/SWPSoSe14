/*[--**--]
Copyright (C) 2014  SWPSoSe14Cpp Group

This program is free software; you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this
program; if not, see <http://www.gnu.org/licenses/>.*/

#include <iostream>
//#include <string.h>

#include <common/Env.h>
#include <frontend/lexer/Lexer.h>
#include <frontend/parser/Parser.h>
#include <frontend/Graphs.h>
#include <backend/backend.h>

using namespace std;

int main(int argc, char *argv[]) {
  Env::initIoDirectory();
  Env::parseParams(argc, argv);

  Graphs graphs;

  // ------------------------------------------------------------------------
  // FRONTEND
  // ------------------------------------------------------------------------
  if (Env::hasSrcFile()) {
    Env::printCaption("Frontend - Lexer");

    // Lexer
    Lexer lexer;
    lexer.lex(Env::getSrcFile());

    if (!lexer.hasFunctions()) {
      throw EnvException(FRONTEND_LEXER, "No rail functions found in " + Env::getSrcFile());
    }
    Env::showWarnings();

    // Parser
    Env::printCaption("Frontend - Parser");
    for(auto it = lexer.functions.begin(); it < lexer.functions.end(); ++it) {
      Parser p(*it);
      shared_ptr<Adjacency_list> asg = p.parseGraph();
      if(asg == NULL) {
        throw EnvException(FRONTEND_PARSER, "No Asg present. Parser report: " + p.errorMessage);
      }
      Env::showWarnings();
      graphs.put((*it)->getName(), asg);
    }
    Env::showWarnings();
  } else if (Env::hasSrcDeserialize()) {
    // Deserialize
    Env::printCaption("ASG - Deserialize");
    graphs.unmarshall(Env::getSrcDeserialize(), ';');
    Env::showWarnings();
  } else {
    throw EnvException(ENVIRONMENT, "No source specified. Use either -i <file> or -d <file>.");
  }

  // ------------------------------------------------------------------------
  // ASG
  // ------------------------------------------------------------------------

  // Serialize
  if (Env::getDstSerialize() != "") {
    Env::printCaption("ASG - Serialize");
    graphs.marshall(Env::getDstSerialize(), ';');
    Env::showWarnings();
  }

  // GraphViz
  if (Env::getDstGraphviz() != "") {
    Env::printCaption("ASG - GraphViz");
    graphs.writeGraphViz(Env::getDstGraphviz());
    Env::showWarnings();
  }

  // ------------------------------------------------------------------------
  // BACKEND
  // ------------------------------------------------------------------------
  Env::printCaption("Backend");

  ofstream outFile(Env::getDstClassfile(), std::ofstream::binary);
  Backend::Generate(graphs, &outFile);

  Env::showWarnings();
  return 0;
}
