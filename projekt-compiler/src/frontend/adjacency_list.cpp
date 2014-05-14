#include <frontend/adjacency_list.h>

Adjacency_list::Adjacency_list(Adjacency_list::str name) : graph_name(name)
{
}

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
  //std::cout << "Push: " << node->id << std::endl;
  std::shared_ptr<Node> n = find(node->id);
  if (!n)
    nodes.push_back(node);
  else
    n = node;
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


std::shared_ptr<Node> Adjacency_list::find(int id) const
{
  std::shared_ptr<Node> node;
  for (size_t i = 0; i < nodeCount(); i++) {
    if (nodes.at(i)->id == id) {
      node = nodes.at(i);
      break;
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
    return find(1);
  else
    throw -1;
}
