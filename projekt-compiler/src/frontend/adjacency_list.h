#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <iterator>
#include <iostream>
#include <map>

#include <common/ast/ast.h>
#include <common/Env.h>



/**
 * Represents the implementation of the ASG interface as adjacency list.
 * See documentation Graph.
 *
 * @author Christopher Zell <Zelldon91@googlemail.com>
 */
class Adjacency_list : public Graph {
 private:
 /**
  * Represents a type of vector from type shared_ptr of Node.
  * Type is named NODES.
  */
  typedef std::vector<std::shared_ptr<Node>> NODES;
 /**
  * Represents a type of const std::string, named str.
  */
  typedef const std::string str;

 /**
  * Represents the type of map with std::string as key and Grahp::Variable_Type as value.
  */
  typedef std::map<std::string, Graph::Variable_Type> Variable_Map;
 /**
  * The name of the ASG, respectively of the Rail function.
  */
  str graph_name;
 /**
  * The nodes of the ASG, respectively the commands of the corresponding Rail function.
  */
  NODES nodes;

 /**
  * The symbol table which contains the variable declarations for the current graph (Rail function).
  */
  Variable_Map symbol_table;
 public:
 /**
  * The ctor of the adjacency list, which gets the name of the Rail function.
  * @param name       the name of the Rail function (ASG)
  */
  Adjacency_list(str name);
 /**
  * The ctor of the adjacency list, which gets the name of the Rail function and the first node.
  * @param name       the name of the Rail function (ASG)
  * @param start      the start node of the Rail function
  */
  Adjacency_list(str name, std::shared_ptr<Node> start);

 /**
  * The destructor of the adjacency list.
  * Cleans up the nodes vector and resets the shared_ptr's.
  */
  virtual ~Adjacency_list();
 /**
  * The implementation only adds the node if the node not already exists in the list.
  * If the node exists, the node will be replaced by the new one.
  * <br/>
  * Inherited DOC: <br/>
  * @copydoc Graph::addNode()
  */
  virtual void addNode(std::shared_ptr<Node> node);
  virtual void addEdge(std::shared_ptr<Node> source, std::shared_ptr<Node> dist, bool path);
 /**
  * Returns the begin iterator of the nodes vector.
  * @return     the begin iterator of the nodes vector
  */
  virtual NODES::iterator begin();
  virtual std::shared_ptr<Node> start();
  virtual std::size_t nodeCount() const;
  virtual std::shared_ptr<Node> find(int id) const;
	virtual std::string name() const;
	virtual void putVariable(const std::string& identifier);
	virtual void putVariable(const std::string& identifier, Variable_Type type);
	virtual Variable_Type getVariable(const std::string& identifier);
};

#endif /* ADJACENCY_LIST_H */
