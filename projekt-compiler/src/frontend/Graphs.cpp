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

void Graphs::marshall(Graphs::str file) {

	// [function name]
	// id ; cmd ; adj1 (true,default) ; adj2 (false, not present)
	std::cout << "Serializing to " << file << std::endl;

	std::ofstream ofh(file);
	if(!ofh) {
		IO_Exception ie;
		ie.set_file(file);
		throw ie;
	}


	std::map<std::string, Graph_ptr>::iterator it;

	// Function
	for(it = this->graphs.begin(); it != this->graphs.end(); ++it) {
		Graph_ptr gp = it->second;
		std::size_t count = gp->nodeCount();
		std::cout << "\t" << count << " nodes" << std::endl;

		// Function name
		ofh << "[" << it->first <<"]" << std::endl;

		// Node
		for(std::size_t i = 0; i<=count; ++i) {
			std::shared_ptr<Node> node = gp->find(i);

			if(node == NULL) {
				continue;
			}

			// id ; Command
			ofh << node->id << ";" << node->command.arg;

			// Adjacency list
			if(node->successor1) {
				ofh << ";" << node->successor1->id;
			}
			else {
				// Error state for Haskell-Group
				ofh << ";0";
			}

			if(node->successor2) {
				ofh << ";" << node->successor2->id;
			}
			else {
				// Error state for Haskell-Group
				ofh << ";0";
			}

			ofh << std::endl;
		}

	}


	ofh.close();


	std::cout << "\tResult:" << std::endl;
	std::ifstream ifh(file);
	std::string line;
	while(std::getline(ifh, line)) {
		std::cout << "\t\t" << line << std::endl;
	}
	ifh.close();


	std::cout << "done..." << std::endl;
}

void Graphs::unmarshall(Graphs::str file, char delimiter)
{
  std::cout << "Deserializing " << file << std::endl;

  std::ifstream infile(file);
  std::string line;
  std::getline(infile, line); //must contain function name
  try
  {
    if (!containsFunctionName(line))//if (!std::regex_match(line.begin(), line.end(), regexs[0]))
      throw Invalid_Format_Exception();

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
  catch (Invalid_Format_Exception& ife)
  {
    infile.close();
    throw ife;
  }

  std::cout << "done..." << std::endl;
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
  std::cout << "\tGraph: " << adj->name() << std::endl;
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

  if (!std::getline(lineStream, cell, delimiter)) // id
    throw Invalid_Format_Exception();

  int id = std::stoi(cell);
  std::shared_ptr<Node> n(adj->find(id));
  if (!n) {
    n.reset(new Node());
    n->id = id;
  }

  if (!std::getline(lineStream, cell, delimiter)) // arg
    throw Invalid_Format_Exception();

  n->command = getCommand(cell);

  if (!std::getline(lineStream, cell, delimiter)) // adja
    throw Invalid_Format_Exception();

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
             containsBeginAndEndChar(cmd, '}', '{'))
  {
    if (cmd[0] == '[' || cmd[0] == ']')
      c.type = Command::Type::PUSH_CONST;
    else
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

	std::cout << "Creating GraphViz dot-file" << std::endl;

	std::ofstream fh(file);
	if(!fh) {
		IO_Exception ie;
		ie.set_file(file);
		throw ie;
	}

	fh << "digraph G {";
	fh << std::endl << "	node [shape=\"circle\",fontname=Courir,fontsize=10,style=filled,penwidth=1,fillcolor=\"#EEEEEE\",color=\"#048ABD\"]";
	fh << std::endl << "	edge [color=\"#000000\", arrowsize=\"0.8\", fontsize=10, decorate=true]";
	fh << std::endl << "	labelloc=\"t\";";
	fh << std::endl << "	label=\"Rail ASG\";";
	fh << std::endl << "	rankdir=\"TL\";";
	fh << std::endl;


	// Function
	std::map<std::string, Graph_ptr>::iterator it;
	for(it = this->graphs.begin(); it != this->graphs.end(); ++it) {
		Graph_ptr gp = it->second;
		std::size_t count = gp->nodeCount();

		// Function name
		fh << std::endl << "\tfunc" << it->first << " [shape=\"invhouse\",fillcolor=\"none\",label=\"Function " << it->first << "\"]";

		// Function -> first node
		fh << std::endl << "\tfunc" << it->first << " -> 1";


		// Nodes
		for(std::size_t i=0; i<=count; ++i) {
			std::shared_ptr<Node> node = gp->find(i);
			if(node == NULL) {
				continue;
			}

			// Node
			//FIXME command-based node shapes
			fh << std::endl << "\t" << node->id << " [label=\"" << node->command.arg << "\"]";

			// Edges
			if(node->successor1) {
				fh << std::endl << "\t" << node->id << " -> " << node->successor1->id;
				fh << (node->successor1 && node->successor2 ? " [label=\"true\"]" : "");
			}
			if(node->successor2) {
				fh << std::endl << "\t" << node->id << " -> " << node->successor2->id;
				fh << (node->successor1 && node->successor2 ? " [label=\"false\"]" : "");
			}
		}
	}

	fh << std::endl << "}";

	fh.close();

	std::cout << "done..." << std::endl;
}



void printNode(std::shared_ptr<Node> n)
{
  std::cout << "\t\tNode: " << n->id
            << " Cmd Type:" << n->command.type << " Arg: " << n->command.arg
            << " Succ 1: " << n->successor1->id << std::endl;
}

