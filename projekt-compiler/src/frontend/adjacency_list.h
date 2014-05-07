#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>

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
	typedef const std::string& str;

	NODES nodes;
public:

	Adjacency_list() {};
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

	//void serialize(std::ostream &out);

	void deserialize(str file, char delimiter) 
	{
	  std::ifstream infile(file);
	  std::string line;
	  std::getline(infile, line); //TABLE HEADER
	  while(std::getline(infile, line))
	  {
	  	Node n;
	    std::stringstream lineStream(line);
	    std::string cell;
	    if (std::getline(lineStream, cell, delimiter)) // id
	 		n.id = std::stoi(cell);
	 	// if (std::getline(lineStream, cell, delimiter)) // arg
	 		// n.command = getCommand(cell);
	 	// if (std::getline(lineStream, cell, delimiter)) // adj
	 		//todo
	  }
	  infile.close();
	}



	virtual NODES::iterator begin() {return nodes.begin();}

	virtual std::size_t nodeCount() {return nodes.size();}
private:
	Command getCommand(str cmd);
};

#endif /* ADJACENCY_LIST_H */
