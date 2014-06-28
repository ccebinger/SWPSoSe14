#include <frontend/Graphs.h>

Graphs::Graphs()
{
  //ctor
}

Graphs::~Graphs()
{
  for (Graph_map::iterator it = graphs.begin(); it != graphs.end(); it++)
  {
    it->second.reset();
  }
  //dtor
}



bool Graphs::put(Graphs::str key, Graphs::Graph_ptr graph) {
  graphs.insert(std::pair<std::string, Graphs::Graph_ptr>(key, graph));
  return true;
}

Graphs::Graph_ptr Graphs::find(Graphs::str key)
{
  Graph_map::iterator it = graphs.find(key);
  if (it != graphs.end())
    return it->second;
  else
    return nullptr;
}

size_t Graphs::size()
{
  return graphs.size();
}

Graphs::Graph_map::iterator Graphs::begin()
{
  return graphs.begin();
}

Graphs::Graph_map::iterator Graphs::end()
{
  return graphs.end();
}

void Graphs::marshall(Graphs::str file, char delimiter) {

	// [function name]
	// id ; cmd ; adj1 (true,default) ; adj2 (false, not present)
	if(Env::verbose()) {
		std::cout << "Serializing to " << file << std::endl;
	}


	std::ofstream ofh(file);
	if(!ofh) {
		throw EnvException(ASG_SERIALIZE, "Cannot write file " + file);
	}


	std::map<std::string, Graph_ptr>::iterator it;

	// Function
	for(it = this->graphs.begin(); it != this->graphs.end(); ++it) {
		Graph_ptr gp = it->second;
		std::size_t count = gp->nodeCount();

		if(Env::verbose()) {
			std::cout << "\t'" << it->first << "': " << count << " nodes" << std::endl;
		}

		// Function name
		ofh << "[" << it->first <<"]" << std::endl;

		// Node
		for(std::size_t i = 0; i<=count; ++i) {
			std::shared_ptr<Node> node = gp->find(i);

			if(node == NULL) {
				continue;
			}

			// id ; Command
			ofh << node->id << delimiter << node->command.arg << delimiter;

			// Adjacency list
			if(node->successor1) {
				ofh << node->successor1->id;
			}
			else {
				// Error state for Haskell-Group
				ofh << "0";
			}
			ofh << ",";
			if(node->successor2) {
				ofh << node->successor2->id;
			}
			else {
				// Error state for Haskell-Group
				ofh << "0";
			}

			ofh << std::endl;
		}

	}


	ofh.close();

	if(Env::verbose()) {
		std::cout << "\tResult:" << std::endl;
		std::ifstream ifh(file);
		std::string line;
		while(std::getline(ifh, line)) {
			std::cout << "\t\t" << line << std::endl;
		}
		ifh.close();
		std::cout << "done..." << std::endl;
	}


}

void Graphs::unmarshall(Graphs::str file, char delimiter) {
	if(Env::verbose()) {
		std::cout << "Deserializing " << file << std::endl;
	}

  std::ifstream infile(file);
  std::string line;
  std::getline(infile, line); //must contain function name
  try
  {
    if (!containsFunctionName(line))//if (!std::regex_match(line.begin(), line.end(), regexs[0]))
      throw EnvException(ASG_DESERIALIZE, "Invalid format: " + line);

    while (containsFunctionName(line))//(std::regex_match(line.begin(), line.end(), regexs[0]))
    {
      if (containsBeginAndEndChar(line, '[', ']'))
      {
        line.erase(0,1);
        line.erase(line.length()-1, 1);
      }
      const std::string key = line;
      put(key, unmarshall_Function(infile, line, delimiter));
      skip_empty_lines(infile, line);
    }
    infile.close();
  }
  catch (...)
  {
    infile.close();
    throw;
  }

  if(Env::verbose()) {
	  std::cout << "done..." << std::endl;
  }
}

bool Graphs::containsFunctionName(str line)
{
  if (line.find_first_of("[") == 0)
  {
    if (line.find_last_of("]") == line.length() - 1)
    	return true;
  }
  return false;
}

void Graphs::skip_empty_lines(std::ifstream& infile, std::string& line)
{
  while (std::getline(infile, line) && line.empty()) ;
}

Graphs::Graph_ptr Graphs::unmarshall_Function(std::ifstream& infile, std::string& line, char delimiter)
{
  Graphs::Graph_ptr adj(new Adjacency_list(line));
  if(Env::verbose()) {
	  std::cout << "\tGraph: " << adj->name() << std::endl;
  }
  while(std::getline(infile, line) && !line.empty() && !containsFunctionName(line))
  {
    std::shared_ptr<Node> n(unmarshall_line(adj, line, delimiter));
    adj->addNode(n);
  }
  return adj;
}

Graphs::Node_ptr Graphs::unmarshall_line(Graphs::Graph_ptr adj, std::string& line, char delimiter)
{
  std::stringstream lineStream(line);
  std::string cell;

  if (!std::getline(lineStream, cell, delimiter)) { // id
    throw EnvException(ASG_DESERIALIZE, "Invalid format found");
  }

  int id = std::stoi(cell);
  std::shared_ptr<Node> n(adj->find(id));
  if (!n) {
    n.reset(new Node());
    n->id = id;
  }

  if (!std::getline(lineStream, cell, delimiter)) { // arg
	 throw EnvException(ASG_DESERIALIZE, "Invalid format found");
  }

  n->command = getCommand(cell);

  if (!std::getline(lineStream, cell, delimiter)) { // adja
    throw EnvException(ASG_DESERIALIZE, "Invalid format found");
  }

  n->successor1 = findNode(adj, cell);
  if (std::getline(lineStream, cell, delimiter))
  {
    n->successor2 = findNode(adj, cell);
  }

  printNode(n);
  return n;
}

Command Graphs::getCommand(std::string& cmd)
{
  Command c;
  int length = cmd.length();
  if (length == 1)
  {
    std::locale loc;
    if (std::isdigit(cmd[0], loc))
      c.type = Command::Type::PUSH_CONST;
    else
      c.type = static_cast<Command::Type>(cmd[0]);
  } else if (containsBeginAndEndChar(cmd, '[', ']') ||
             containsBeginAndEndChar(cmd, '{', '}') ||
             containsBeginAndEndChar(cmd, ']', '[') ||
             containsBeginAndEndChar(cmd, '}', '{') ||
             containsBeginAndEndChar(cmd, '(', ')') ||
             containsBeginAndEndChar(cmd, ')', '('))
  {
    if (cmd[0] == '[' || cmd[0] == ']')
      c.type = Command::Type::PUSH_CONST;
    else if (cmd[0] == '(' || cmd[0] == ')')
    {
      if (cmd[1] == '!')
        c.type = Command::Type::VAR_PUSH;
      else
        c.type = Command::Type::VAR_POP;
    } else
      c.type = Command::Type::CALL;

    cmd.erase(0,1);
    cmd.erase(cmd.length()-1, 1);
  }
  else
    c.type = Command::Type::OUTPUT;

  c.arg = cmd;
  return c;
}

bool Graphs::containsBeginAndEndChar(std::string& cmd, char begin, char end)
{
  return cmd.find_first_of(begin) == 0 && cmd.find_last_of(end) == cmd.length() - 1;
}

std::shared_ptr<Node> Graphs::findNode(Graphs::Graph_ptr adj, std::string id)
{
  int nodeID = std::stoi(id);
  std::shared_ptr<Node> n(adj->find(nodeID));
  if (!n) {
    n .reset(new Node());
    n->id = nodeID;
    adj->addNode(n);
  }
  return n;
}


void Graphs::writeGraphViz(Graphs::str file) {
	if(Env::verbose()) {
		std::cout << "Creating dot-file" << std::endl;
	}


	std::ofstream fh(file);
	if(!fh) {
		throw EnvException(ASG_GRAPHVIZ, "Cannot open file: " + file);
	}

	fh << "digraph G {";
	fh << std::endl << "	node [shape=\"circle\",fontname=Courir,fontsize=10,style=filled,penwidth=1,fillcolor=\"#EEEEEE\",color=\"#048ABD\"]";
	fh << std::endl << "	edge [color=\"#000000\", arrowsize=\"0.8\", fontsize=10, decorate=true]";
	fh << std::endl << "	labelloc=\"t\";";
	fh << std::endl << "	label=\"Rail ASG\";";
	fh << std::endl << "	rankdir=\"TL\";";
	fh << std::endl;


	// ------------------------------------------------------------------------------------------
	// Function
	// ------------------------------------------------------------------------------------------
	// id + gs = unique id
	uint32_t gs = 0;
	std::map<std::string, Graph_ptr>::iterator it;
	for(it = this->graphs.begin(); it != this->graphs.end(); ++it) {
		Graph_ptr gp = it->second;
		std::size_t count = gp->nodeCount();

		// Function name
		fh << std::endl << "\tfunc_" << it->first << " [shape=\"invhouse\",fillcolor=\"none\",label=\"'" << it->first << "'\"]";

		// Function -> first node

		fh << std::endl << "\tfunc_" << it->first << " -> " << (gs == 0 ? 1 : gs+1);


		// Nodes
		for(std::size_t i=0; i<=count; ++i) {
			std::shared_ptr<Node> node = gp->find(i);
			if(node == NULL) {
				continue;
			}

			// Node
			fh << std::endl << "\t" << node->id+gs << " " << gvGetNodeStyles(node);

			// Function calls: Node -> Function
			if(node->command.type == Command::Type::CALL) {
				std::string funcName = node->command.arg.substr(1, node->command.arg.length()-2);
				fh << std::endl << "\t" << node->id+gs << " -> func_" << funcName << " [label=\"CALL\",style=dotted]";
			}


			// Edges
			if(node->successor1) {
				fh << std::endl << "\t" << node->id+gs << " -> " << node->successor1->id+gs;
				fh << (node->successor1 && node->successor2 ? " [label=\"true\"]" : "");
			}
			if(node->successor2) {
				fh << std::endl << "\t" << node->id+gs << " -> " << node->successor2->id+gs;
				fh << (node->successor1 && node->successor2 ? " [label=\"false\"]" : "");
			}
		}


		gs += count;
	}

	fh << std::endl << "}";
	fh.close();

	if(Env::verbose()) {
		std::cout << "done..." << std::endl;
	}
}

std::vector<std::string> Graphs::keyset()
{
  std::vector<std::string> keyset;
  for (Graph_map::iterator it = graphs.begin(); it != graphs.end(); it++)
  {
    keyset.push_back(it->first);
  }
  return keyset;
}

std::string Graphs::gvGetNodeStyles(std::shared_ptr<Node> node) const {




	//FIXME command-based node style - requires a good testfile
	bool useLabel = true;
	std::string shape = "";
	std::string fillColor = "";
	switch(node->command.type) {

		// Rail
		case Command::Type::START:				shape="plaintext"; break;
		case Command::Type::FINISH:				shape="house"; fillColor="none"; break;
		case Command::Type::BOOM:				useLabel=false; /*shape="proteasesite";*/ break;
		case Command::Type::REFLECTOR:			break; //we can change the arrow direction in Graph ?
		case Command::Type::LAMBDA:				break;
		case Command::Type::CALL:				shape="diamond"; break;


		// Stack
		case Command::Type::PUSH_CONST:			shape="signature"; break;
		case Command::Type::TRUE:				shape="signature"; fillColor="green"; break;
		case Command::Type::FALSE:				shape="signature"; fillColor="red"; break;


		// IO
		case Command::Type::INPUT:				shape="larrow"; break;
		case Command::Type::OUTPUT:				shape="rarrow"; break;

		// List
		case Command::Type::APPEND:				break;
		case Command::Type::CUT:				break;
		case Command::Type::LIST_BREAKUP:		break;
		case Command::Type::LIST_CONS:			break;
		case Command::Type::NIL:				break;
		case Command::Type::SIZE:				break;



		// Math
		case Command::Type::ADD:
		case Command::Type::DIV:
		case Command::Type::MOD:
		case Command::Type::MULT:
		case Command::Type::SUB:				shape="note"; break;

		// Compare
		case Command::Type::EQUAL:				break;
		case Command::Type::GREATER:			break;
		case Command::Type::EOF_CHECK:			shape="assembly"; break;
		case Command::Type::TYPE_CHECK:			break;
		case Command::Type::UNDERFLOW_CHECK:	break;

		// Junction
		case Command::Type::EASTJUNC:
		case Command::Type::NORTHJUNC:
		case Command::Type::SOUTHJUNC:
		case Command::Type::WESTJUNC:			shape="triangle"; break;

		default: break;
	}



	std::string style = "[";

	style += "label=\"" + (useLabel ? node->command.arg : "") + "\"";

	if(shape != "") {
		style += ",shape=" + shape;
	}

	if(fillColor != "") {
		style += ",fillcolor=" + fillColor;
	}


	style += "]";

//	if(Env::verbose()) {
//		std::cout << "NODE STYLE: " << style << std::endl;
//	}
	return style;
}




void printNode(std::shared_ptr<Node> n) {
	if(Env::verbose()) {
		std::cout << "\t\tNode: " << n->id
			<< " Cmd Type:" << n->command.type << " Arg: " << n->command.arg
			<< " Succ 1: " << n->successor1->id << std::endl;
	}
}

