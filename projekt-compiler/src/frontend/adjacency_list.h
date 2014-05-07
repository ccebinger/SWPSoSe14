#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <string>
#include <vector>

enum Command_Type
{
   push, pop, add, sub, cond
};

struct Command {
	std::string arg;
	Command_Type type;
};

struct Node {
	int id;
	Command command;
	Node *successor1; //true Pfad
	Node *successor2; //false Pfad
	bool operator ==(Node n) {
		return id == n.id;
	}
};

class Adjacency_list {
private:
	typedef std::vector<Node*> NODES;
	NODES nodes;
public:

	Adjacency_list(Node* start) 
	{
		nodes.push_back(start);
	}
	virtual ~Adjacency_list()
	{
		for (int i = 0; i < nodes.size(); i++)
			delete nodes.at(i);
		nodes.clear();
	}
	virtual void addNode(Node *node) 
	{
		nodes.push_back(node);
	}
	virtual void addEdge(Node *source, Node *dist, bool path) {
		for (int i = 0; i < nodes.size(); i++) {
			if ((*nodes.at(i)) == (*source))
				if (path)
					nodes.at(i)->successor1 = dist;
				else
					nodes.at(i)->successor2 = dist;
		}
	}

	//Serilize(Ostream &out)
	//Deserilize()
	virtual NODES::iterator begin() {return nodes.begin();}

	virtual std::size_t nodeCount() {return nodes.size();}
};

#endif /* ADJACENCY_LIST_H */
