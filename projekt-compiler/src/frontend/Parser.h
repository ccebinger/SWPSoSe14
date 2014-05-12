
#ifndef Parser_h
#define Parser_h
using namespace std;
#include <list>
#include <map>
#include <string>
#include "adjacency_list.h"
#include "ast.h"
/*assignment of the enum values must not be changed without adapting the functions
 * void turnLeft45Deg() and
 * void turnRight45Deg()
 * in Parser.cpp since they rely on this order
*/
enum Direction {E=0,SE=1,S=2,SW=3,W=4,NW=5,N=6,NE=7};

struct offsetvalues{
	//used to store the offsets on where to look(concerning the current position), first in offsets is for looking to the left, second looks straight ahead, third for looking right
	int offsets[3];
};

struct allowedChars{
	//used to store the acceptable chars for looking left, straight or right
	list<char> left;
	list<char> straight;
	list<char> right;
};

struct BoardContainer{
	char (*board)[1024];
	int xlen;
	int ylen;
};

class Parser{
	private:
		const int LEFT = 0;
		const int STRAIGHT = 1;
		const int RIGHT = 2;
		int posX,posY,xlen,ylen;
		Direction dir;
		char (*board)[1024];
		std::shared_ptr<Adjacency_list> abstractSyntaxGraph;
		std::shared_ptr<Node> currentNode;
		map<Direction,offsetvalues> xOffsetMap;
		map<Direction,offsetvalues> yOffsetMap;
		map<Direction,allowedChars> validRailMap;
		string graphName;
		bool parsingNotFinished;
		void move();
		void addToAbstractSyntaxGraph(string);
		void initializeOffsetMaps();
		void initializeValidRailMap();
		void turnLeft45Deg();
		void turnRight45Deg();
		bool checkForValidCommandsInStraightDir(int,int);
		string readCharsUntil(char);
	public:
		string errorMessage;
		Parser(BoardContainer,string);
		shared_ptr<Adjacency_list> parseGraph();
};

#endif
