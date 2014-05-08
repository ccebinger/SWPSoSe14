#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>
#include "../common/ast/ast.h"


class Adjacency_list : public Graph {
 private:
  typedef std::vector<Node*> NODES;
  typedef const std::string& str;
  str graph_name;
  NODES nodes;
 public:
  Adjacency_list(str name);
  Adjacency_list(str name, Node* start);

  virtual ~Adjacency_list();
  virtual void addNode(Node *node);
  virtual void addEdge(Node *source, Node *dist, bool path);
  virtual void serialize(std::ostream &out);
  virtual void deserialize(str file, char delimiter);
  virtual NODES::iterator begin();
  virtual Node* start();
  virtual std::size_t nodeCount() const;
  virtual Node* find(int id);
	virtual std::string name() const;
 private:
  //Command getCommand(str cmd);
};

#endif /* ADJACENCY_LIST_H */
