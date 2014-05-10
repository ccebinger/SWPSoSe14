#include <iostream>
#include "Csv_io.h"
#include "Graphs.h"


void checkVec(std::vector<std::string> result)
{
  std::regex reg("\[[a-zA-z0-9]+\]"); //THE FUNCTION NAMES

  for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
  {
    if (std::regex_match((*it).begin(), (*it).end(), reg))
    {
      std::cout << "FUNCTION: " << *it << std::endl;
    }
  }
}

int unmarshallGraph(const std::string& file, char delimiter)
{
  Graphs graphs;
  graphs.unmarshall(file, delimiter);
  std::cout << "Size: " << graphs.size() << std::endl;

  for (Graphs::Graph_map::iterator it = graphs.begin(); it != graphs.end(); it++)
  {
    std::cout << it->first << it->second << std::endl;
  }

  std::shared_ptr<Graph> g = graphs.find("[main]");
  if (g == nullptr)
    return -9;

  std::cout << "Name: " << g->name() << "Size: " << g->nodeCount() << std::endl;
  std::shared_ptr<Node>  n = g->start();
  printNode(n);

  g = graphs.find("[Hello]");
  if (g == nullptr)
    return -9;

  std::cout << "Name: " << g->name() << "Size: " << g->nodeCount() << std::endl;
  n = g->start();
  printNode(n);
}

int main()
{
  unmarshallGraph("src/frontend/test_ast.csv", ';');
  std::cout << std::endl << std::endl;
  unmarshallGraph("src/frontend/test2.csv", ';');
  return 0;
}

