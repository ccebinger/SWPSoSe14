#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <iterator>
#include <iostream>
#include "../common/ast/ast.h"

class Adjacency_list : public Graph {
 private:
  typedef std::vector<std::shared_ptr<Node>> NODES;
  typedef const std::string str;
  str graph_name;
  NODES nodes;
 public:
  Adjacency_list(str name);
  Adjacency_list(str name, std::shared_ptr<Node> start);

  virtual ~Adjacency_list();
  virtual void addNode(std::shared_ptr<Node> node);
  virtual void addEdge(std::shared_ptr<Node> source, std::shared_ptr<Node> dist, bool path);
  virtual void serialize(std::ostream &out);
  virtual NODES::iterator begin();
  virtual std::shared_ptr<Node> start();
  virtual std::size_t nodeCount() const;
  virtual std::shared_ptr<Node> find(int id) const;
	virtual std::string name() const;
 private:
  //Command getCommand(str cmd);
};

#endif /* ADJACENCY_LIST_H */
