
#ifndef Parser_h
#define Parser_h

#include <list>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>



#include <frontend/adjacency_list.h>


using namespace std;


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

//TODO: x hinzufügen als valid rail und @ implementieren
class Parser{
	private:
		const int LEFT = 0;
		const int STRAIGHT = 1;
		const int RIGHT = 2;
		//IMPORTANT: Note that in all occurences in this file x is the line and y is the position of the character in line x(basically the column)
		//this is a little counter intuitive to a coordiante system since it is exactly the other way round(going right means increasing y value)
		//this should probably be refactored in the future, or x and y should be called i and j (since these are more commonly used when indexing matrices)
		int posX,posY,xlen,ylen;
		Direction dir;
		char (*board)[1024];
		std::shared_ptr<Adjacency_list> abstractSyntaxGraph;
		std::shared_ptr<Node> currentNode;
		int lastUsedId;
		bool addNextNodeAsTruePathOfPreviousNode;
		map<Direction,offsetvalues> xOffsetMap;
		map<Direction,offsetvalues> yOffsetMap;
		map<Direction,allowedChars> validRailMap;
		map<Direction,char> leftDirChangeMap;
		map<Direction,char> rightDirChangeMap;
		string graphName;
		bool parsingNotFinished;
		void move();
		void addToAbstractSyntaxGraph(string,Command::Type);
		void initializeOffsetMaps();
		void initializeValidRailMap();
		void initializeDirChangeMaps();
		void turnLeft45Deg();
		void turnRight45Deg();
		void reverseDirection();
		bool parseJunctions(Direction,int,int,Direction,Direction,string,Command::Type);
		bool checkForValidCommandsInStraightDir(int,int);
		int getNextUnusedId();
		void setXY(int,int);
		string readCharsUntil(char);
	public:
		string errorMessage;
		Parser(BoardContainer,string);
		shared_ptr<Adjacency_list> parseGraph();
		shared_ptr<Adjacency_list> parseGraph(int,int,Direction);
};

#endif
