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

int main()
{
  Csv_io csv;
  std::vector<std::string> result = csv.deserialize("src/frontend/test_ast.csv", ';');
  checkVec(result);
  Graphs graphs;
  graphs.unmarshall("src/frontend/test_ast.csv", ';');
  std::cout << "Size: " << graphs.size() << std::endl;

  for (Graphs::Graph_map::iterator it = graphs.begin(); it != graphs.end(); it++)
  {
    std::cout << it->first << it->second << std::endl;
  }

  std::shared_ptr<Graph> g = graphs.find("[Hello]");
  if (g == nullptr)
    return -9;

  std::cout << "Name: " << g->name() << "Size: " << g->nodeCount() << std::endl;
  std::shared_ptr<Node> n = g->start();
  printNode(n);
  csv.serialize(result, "test2.csv", ',', 3);
  return 0;
}

