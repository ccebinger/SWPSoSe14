#include "adjacency_list.h"

Adjacency_list::Adjacency_list() {}
Adjacency_list::Adjacency_list(Node* start)
{
  nodes.push_back(start);
}
Adjacency_list::~Adjacency_list()
{
  for (size_t i = 0; i < nodes.size(); i++)
    delete nodes.at(i);
  nodes.clear();
}
void Adjacency_list::addNode(Node *node)
{
  nodes.push_back(node);
}
void Adjacency_list::addEdge(Node *source, Node *dist, bool path) {
  for (size_t i = 0; i < nodes.size(); i++) {
    if ((*nodes.at(i)) == (*source))
    {
      if (path)
      {
        nodes.at(i)->successor1 = dist;
      }
      else
      {
        nodes.at(i)->successor2 = dist;
      }
    }
  }
}

//void serialize(std::ostream &out);

void Adjacency_list::deserialize(Adjacency_list::str file, char delimiter)
{
  std::ifstream infile(file);
  std::string line;
  std::getline(infile, line); //TABLE HEADER
  while(std::getline(infile, line))
  {
    Node n;
    std::stringstream lineStream(line);
    std::string cell;
    if (std::getline(lineStream, cell, delimiter)) // id
    n.id = std::stoi(cell);
  // if (std::getline(lineStream, cell, delimiter)) // arg
    // n.command = getCommand(cell);
  // if (std::getline(lineStream, cell, delimiter)) // adj
    //todo
  }
  infile.close();
}



Adjacency_list::NODES::iterator Adjacency_list::begin()
{
  return nodes.begin();
}

std::size_t Adjacency_list::nodeCount()
{
  return nodes.size();
}
