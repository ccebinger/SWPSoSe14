#include "adjacency_list.h"

Adjacency_list::Adjacency_list(Adjacency_list::str name) : graph_name(name) {}

Adjacency_list::Adjacency_list(Adjacency_list::str name, std::shared_ptr<Node> start) : graph_name(name)
{
  nodes.push_back(start);
}

Adjacency_list::~Adjacency_list()
{
  for (size_t i = 0; i < nodes.size(); i++)
    nodes.at(i).reset();
  nodes.clear();
}

void Adjacency_list::addNode(std::shared_ptr<Node> node)
{
  nodes.push_back(node);
}

void Adjacency_list::addEdge(std::shared_ptr<Node> source, std::shared_ptr<Node> dist, bool path) {
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

void Adjacency_list::serialize(std::ostream &out)
{
  //TODO
}

void Adjacency_list::deserialize(Adjacency_list::str file, char delimiter)
{
  std::ifstream infile(file);
  std::string line;
  std::getline(infile, line); //TABLE HEADER
  while(std::getline(infile, line))
  {
    //Node n;
    //std::stringstream lineStream(line);
    //std::string cell;
    //if (std::getline(lineStream, cell, delimiter)) // id
    //n.id = std::stoi(cell);
  // if (std::getline(lineStream, cell, delimiter)) // arg
    // n.command = getCommand(cell);
  // if (std::getline(lineStream, cell, delimiter)) // adj
    //todo
  }
  infile.close();
}

std::shared_ptr<Node> Adjacency_list::find(int id) const
{
  bool found = false;
  std::shared_ptr<Node> node;
  for (size_t i = 0; i < nodeCount() && !found; i++) {
    if (nodes.at(i)->id == id) {
      found = true;
      node = nodes.at(i);
    }
  }
  return node;
}


Adjacency_list::NODES::iterator Adjacency_list::begin()
{
  return nodes.begin();
}

std::size_t Adjacency_list::nodeCount() const
{
  return nodes.size();
}

std::string Adjacency_list::name() const
{
  return graph_name;
}

std::shared_ptr<Node> Adjacency_list::start()
{
  if (nodeCount() > 0)
    return nodes.at(0);
  else
    throw -1;
}
