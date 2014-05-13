#include <iostream>
#include <frontend/Graphs.h>



int unmarshallGraph(const std::string& file, char delimiter)
{
  Graphs graphs;
  graphs.unmarshall(file, delimiter);
  std::cout << "Graphs: " << graphs.size() << std::endl;

  for (Graphs::Graph_map::iterator it = graphs.begin(); it != graphs.end(); it++)
  {
    std::cout << it->first << it->second << std::endl;
  }

  std::shared_ptr<Graph> g = graphs.find("main");
  if (g == nullptr)
    return -9;

  std::cout << "Name: " << g->name() << "Size: " << g->nodeCount() << std::endl;
  std::shared_ptr<Node>  n = g->start();
  printNode(n);

  g = graphs.find("Hello");
  if (g == nullptr)
    return -9;

  std::cout << "Name: " << g->name() << "Size: " << g->nodeCount() << std::endl;
  n = g->start();
  printNode(n);
  return 0;
}

