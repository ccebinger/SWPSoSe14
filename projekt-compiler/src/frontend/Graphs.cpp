#include <frontend/Graphs.h>


const std::regex Graphs::regexs[] = {std::regex("\\[[a-zA-z0-9]+\\]")};


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
	
	//FIXME Miro ;) #2


	//FIXME delete file


	std::ofstream fh(file);
	if(!fh) {
		throw "Serialize: can't open file handle for " + file;
	}


	std::map<std::string, Graph_ptr>::iterator it;

	// Function
	for(it = this->graphs.begin(); it != this->graphs.end(); ++it) {
		Graph_ptr gp = it->second;
		std::size_t count = gp->nodeCount();
		
		// Function name
		fh << "[" << it->first <<"]" << std::endl;
		
		// Node
		for(std::size_t i = 0; i<count; ++i) {
			
			
			
			std::shared_ptr<Node> node = gp->find(i);
			
			
			// id ; Command
			fh << node->id << ";" << node->command.arg;
			
			
			// Adjacency list
			if(node->successor1) {
				fh << ";" << node->successor1->id;
			}
			else {
				// Error state for Haskell-Group
				fh << ";0";
			}
			
			if(node->successor2) {
				fh << ";" << node->successor2->id;
			}
			else {
				// Error state for Haskell-Group
				fh << ";0";
			}
			
			fh << std::endl << std::endl;
		}

	}


	fh.close();
}

void Graphs::unmarshall(Graphs::str file, char delimiter)
{
  std::ifstream infile(file);
  std::string line;
  std::getline(infile, line); //must contain function name
  try
  {
    if (!std::regex_match(line.begin(), line.end(), regexs[0]))
      throw Invalid_Format_Exception();

    while (std::regex_match(line.begin(), line.end(), regexs[0]))
    {
      const std::string key = line;
      put(key, unmarshall_Function(infile, line, delimiter));
      skip_empty_lines(infile, line);
    }
    infile.close();
  }
  catch (Invalid_Format_Exception ife)
  {
    infile.close();
    throw ife;
  }

}

void Graphs::skip_empty_lines(std::ifstream& infile, std::string& line)
{
  while (std::getline(infile, line) && line.empty()) ;
}

Graphs::Graph_ptr Graphs::unmarshall_Function(std::ifstream& infile, std::string& line, char delimiter)
{
  Graphs::Graph_ptr adj(new Adjacency_list(line));
  std::cout << "Graph: " << adj->name() << std::endl;
  while(std::getline(infile, line) && !line.empty() && !std::regex_match(line.begin(), line.end(), regexs[0]))
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

  if (std::getline(lineStream, cell, ',')) {
    n->successor1 = findNode(adj, cell);
    if (std::getline(lineStream, cell, ','))
    {
      n->successor2 = findNode(adj, cell);
    }
  }
  else
    n->successor1 = findNode(adj, cell);

  printNode(n);
  return n;
}

Command Graphs::getCommand(std::string& cmd)
{
  Command c;
  std::regex regex_push("[0-9]|\\[[0-9]\\]|\\['[a-zA-Z0-9]+'\\]");
  std::regex regex_add("a");
  std::regex regex_call("[{][a-zA-Z0-9]+[}]");
  if (std::regex_match(cmd.begin(), cmd.end(), regex_push))
  {
    c.type = Command::Type::PUSH_CONST;
    //std::cout << "PUSH CONST: " << Command::Type::PUSH_CONST;
  }
  else if (std::regex_match(cmd.begin(), cmd.end(), regex_add))
  {
    c.type = Command::Type::ADD;
  }
  else if (std::regex_match(cmd.begin(), cmd.end(), regex_call))
  {
    c.type = Command::Type::CALL;
  }
  else
  {
    c.type = Command::Type::OUTPUT;
  }

  size_t pos = cmd.find("[");
  if (pos != std::string::npos)
  {
    cmd.erase(pos,1); //remove [
    pos = cmd.find("]");
    cmd.erase(pos, 1); //remove ]
  }
  c.arg = cmd;
  return c;
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


void printNode(std::shared_ptr<Node> n)
{
  std::cout << "Node: " << n->id
            << " Cmd Type:" << n->command.type << " Arg: " << n->command.arg
            << " Succ 1: " << n->successor1->id << std::endl;
}

