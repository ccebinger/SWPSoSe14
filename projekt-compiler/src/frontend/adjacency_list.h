#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>
#include "../common/ast/ast.h"


class Adjacency_list : Graph {
 private:
  typedef std::vector<Node*> NODES;
  typedef const std::string& str;

  NODES nodes;
 public:
  Adjacency_list();
  Adjacency_list(Node* start);
  virtual ~Adjacency_list();
  virtual void addNode(Node *node);
  virtual void addEdge(Node *source, Node *dist, bool path);
  //void serialize(std::ostream &out);

  void deserialize(str file, char delimiter);

  virtual NODES::iterator begin();

  virtual std::size_t nodeCount();
 private:
  Command getCommand(str cmd);
};

#endif /* ADJACENCY_LIST_H */
