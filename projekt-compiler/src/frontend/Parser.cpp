/*


* Parser.cpp
 *
 *  Created on: 30.04.2014
 *      Author: LeonBornemann
 */


#include <frontend/Parser.h>


std::list<char> listFromArray(char chars[], int size){
	std::list<char> myList;
	for(int i=0;i<size;i++){
		myList.push_back(chars[i]);
	}
	return myList;
}

Parser::Parser(BoardContainer boardContainer,string graphName){
	this->board = boardContainer.board;
	this->xlen = boardContainer.xlen;
	this->ylen = boardContainer.ylen;
	this->graphName = graphName;
	parsingNotFinished = true;
	//errorMessage - empty String: Everything is ok
	errorMessage = "";
	abstractSyntaxGraph = NULL;
	//Order in offsetvalues: [0] - Left, [1] - straight, [2] - right
	initializeOffsetMaps();
	//allowedChars
	initializeValidRailMap();

}

shared_ptr<Adjacency_list> Parser::parseGraph(){
	dir = SE;
	posX=0;
	posY=-1;
	//find $, initiate pos
	for(int i =0;i<ylen;i++){
		if(board[0][i]=='$'){
			posY = i;
		}
	}
	if(posY==-1){
		errorMessage ="$ not found in function "+graphName;
		return NULL;
	}
	parsingNotFinished = true;
	while(parsingNotFinished){
		cout << "I am at (" << posX << "," << posY << ")" << endl;
		cout << "current char: " << board[posX][posY] << endl;
		move();
		if(errorMessage != ""){
			cout << errorMessage <<endl;
			cout << "parsing aborted" << endl;
			break;
		}
	}
	cout << "finished parsing" << endl;
	return abstractSyntaxGraph;
}

void Parser::move(){
	//straight
	int straightX = posX + xOffsetMap[dir].offsets[STRAIGHT];
	int straightY = posY + yOffsetMap[dir].offsets[STRAIGHT];
	bool straightIsInBoardBounds = straightX >=0 && straightX < xlen && straightY >=0 && straightY <ylen;
	//left
	int leftX = posX + xOffsetMap[dir].offsets[LEFT];
	int leftY = posY + yOffsetMap[dir].offsets[LEFT];
	bool leftIsInBoardBounds =  leftX >=0 && leftX < xlen && leftY >=0 && leftY <ylen;
	//right
	int rightX = posX + xOffsetMap[dir].offsets[RIGHT];
	int rightY = posY + yOffsetMap[dir].offsets[RIGHT];
	bool rightIsInBoardBounds =  rightX >=0 && rightX < xlen && rightY >=0 && rightY <ylen;
	//bool vars that will be checked in the end
	bool leftIsValidRail = false;
	bool rightIsValidRail = false;
	bool straightIsValidRail = false;
	if(straightIsInBoardBounds){
		char charAtStraight = board[straightX][straightY];
		list<char> allowedRails = validRailMap[dir].straight;
		cout << endl;
		straightIsValidRail = std::find(allowedRails.begin(),allowedRails.end(),charAtStraight)!=allowedRails.end();
		if(straightIsValidRail){
			//if allowedRails contains charAtStraight just move forwards
			//TODO: Richtungswechsel hier implementieren
			posX = straightX;
			posY = straightY;
			return;
		}
		//check for other symbols that are allowed
		bool didGoStraight = checkForValidCommandsInStraightDir(straightX,straightY);
		if(didGoStraight){
			return;
		}
	}
	//todo: check if straight is another char, like fname or blockbegin(bracket)
	if(leftIsInBoardBounds){
		char charAtLeft = board[leftX][leftY];
		list<char> allowedRailsLeft = validRailMap[dir].left;
		leftIsValidRail = std::find(allowedRailsLeft.begin(),allowedRailsLeft.end(),charAtLeft)!=allowedRailsLeft.end();
	}
	if(rightIsInBoardBounds){
		char charAtRight = board[rightX][rightY];
		list<char> allowedRailsRight = validRailMap[dir].right;
		rightIsValidRail = std::find(allowedRailsRight.begin(),allowedRailsRight.end(),charAtRight)!=allowedRailsRight.end();
	}
	//error handling begin
	if(leftIsValidRail && rightIsValidRail){
		//TODO: Direction mit ausgeben
		std::stringstream sstm;
		sstm << "abiguous move at line" << posX << ", character:" << posY;
		errorMessage = sstm.str();
		return;
	}
	if(!leftIsValidRail && !rightIsValidRail){
		//TODO: Direction mit ausgeben
		std::stringstream sstm;
		sstm << "no valid move possible at line" << posX << ", character:" << posY;
		errorMessage = sstm.str();
		return;
	}
	//error handling end
	if(leftIsValidRail){
		posX = leftX;
		posY = leftY;
		turnLeft45Deg();
		return;
	}
	if(rightIsValidRail){
		posX = leftX;
		posY = leftY;
		turnRight45Deg();
		return;
	}
	errorMessage = "end of move-function reached - this should never happen and is an internal error";
	return;
}

bool Parser::checkForValidCommandsInStraightDir(int straightX, int straightY){
	char charAtStraight = board[straightX][straightY];
	bool didGoStraight = true;
	string toPush;
	switch(charAtStraight){
	case 'o':
		posX = straightX;
		posY = straightY;
		addToAbstractSyntaxGraph("o");
		break;
	case '[':
		posX = straightX;
		posY = straightY;
		//TODO: ueberpruefen ob notwendig: list<char> invalidCharList = listFromArray({'[','{','(',},);
		toPush = readCharsUntil(']');
		addToAbstractSyntaxGraph(toPush);
		//TODO: create pushNode in graph
		break;
	case ']':
		posX = straightX;
		posY = straightY;
		toPush = readCharsUntil('[');
		addToAbstractSyntaxGraph(toPush);
		break;
	case '#':
		posX = straightX;
		posY = straightY;
		parsingNotFinished = false;
		break;
	default:
		didGoStraight = false;
		break;
	}
	if(errorMessage!=""){
		//TODO:Error stuff?
	}
	return didGoStraight;
}

void Parser::addToAbstractSyntaxGraph(string commandName){
	//debug
	cout << "Node creation: " << commandName << endl;
	//TODO: somehow pass this parameter since it is hardcoded for the first Milestone, this will need to change
	bool isLeftPath = true;
	std::shared_ptr<Node> node(new Node());
	node->command = {Command::Type::PUSH_CONST,commandName};
	//TODO:an Graph Schnittstelle anpassen
	if(abstractSyntaxGraph == NULL){
		//this is the first node that we meet create a new one
		node->id = 1;
		abstractSyntaxGraph.reset(new Adjacency_list(graphName,node));
	} else{
		node->id = (*currentNode).id +1;
		abstractSyntaxGraph->addEdge(currentNode,node,isLeftPath);
	}
	currentNode = node;
}

//setzt position auf until falls er existiert, und gibt den gelesenen string inklusive anfangs und endzeichen zurueck
//falls nicht wir ein leerer string zurueckgegeben und die fehlermeldung gesetzt
string Parser::readCharsUntil(char until){
	string result = "";
	result += board[posX][posY];
	while(true){
		int nextX = posX + xOffsetMap[dir].offsets[STRAIGHT];
		int nextY = posY + yOffsetMap[dir].offsets[STRAIGHT];
		if(posX >= xlen || nextY >= ylen){
			//TODO:Dir auch ausgeben
			std::stringstream sstm;
			sstm << "Parsing ran out of valid space for function in line" << posX << ", character:" << posY;
			errorMessage = sstm.str();
			return "";
		}
		posX = nextX;
		posY = nextY;
		result += board[posX][posY];
		if(board[posX][posY] == until){
			break;
		}
	}
	return result;
}

void Parser::turnLeft45Deg(){
	switch(dir){
	case E: dir = NE; break;
	case SE: dir = E; break;
	case S: dir = SE; break;
	case SW: dir = S; break;
	case W: dir = SW; break;
	case NW: dir = W; break;
	case N: dir = NW; break;
	case NE: dir = N; break;
	}
}

void Parser::turnRight45Deg(){
	switch(dir){
		case E: dir = SE; break;
		case SE: dir = S; break;
		case S: dir = SW; break;
		case SW: dir = W; break;
		case W: dir = NW; break;
		case NW: dir = N; break;
		case N: dir = NE; break;
		case NE: dir = E; break;
		}
}

//int calcXOffsetStraight
void Parser::initializeOffsetMaps(){
	//x offsets
	xOffsetMap[E] = offsetvalues{ {-1,0,+1} };  //Direction E
	xOffsetMap[SE] = offsetvalues{ {0,+1,+1} }; //Direction: SE
	xOffsetMap[S] = offsetvalues{ {+1,+1,+1} }; //Direction: S
	xOffsetMap[SW] = offsetvalues{ {+1,+1,0} };//Direction: SW
	xOffsetMap[W] = offsetvalues{ {+1,0,-1} };  //Direction: W
	xOffsetMap[NW] = offsetvalues{ {0,-1,-1} }; //Direction: NW
	xOffsetMap[N] = offsetvalues{ {-1,-1,-1} }; //Direction: N
	xOffsetMap[NE] = offsetvalues{ {-1,-1,0} };  //Direction: NE
	//y offsets:
	yOffsetMap[E] = offsetvalues{ {+1,+1,+1} };  //DIrection E
	yOffsetMap[SE] = offsetvalues{ {+1,+1,0} }; //Direction: SE
	yOffsetMap[S] = offsetvalues{ {+1,0,-1} }; //Direction: S
	yOffsetMap[SW] = offsetvalues{ {0,-1,-1} };//Direction: SW
	yOffsetMap[W] = offsetvalues{ {-1,-1,-1} };  //Direction: W
	yOffsetMap[NW] = offsetvalues{ {-1,-1,0} }; //Direction: NW
	yOffsetMap[N] = offsetvalues{ {-1,0,+1} }; //Direction: N
	yOffsetMap[NE] = offsetvalues{ {0,-1,-1} };  //Direction: NE
}

void Parser::initializeValidRailMap(){
	//East
	char Eleft[2] = {'/','*'};
	char EStraight[5] = {'-','/','\\','+','*'};
	char ERight[2] = {'\\','*'};
	validRailMap[E] = allowedChars{listFromArray(Eleft,2),listFromArray(EStraight,5),listFromArray(ERight,2)};
	//Southeast
	char SEleft[3] = {'-','*','+'};
	char SEStraight[4] = {'-','\\','|','*'};
	char SERight[3] = {'|','*','+'};
	validRailMap[SE] = allowedChars{listFromArray(SEleft,3),listFromArray(SEStraight,4),listFromArray(SERight,3)};
	//South
	char Sleft[2] = {'\\','*'};
	char SStraight[5] = {'|','\\','/','*','+'};
	char SRight[2] = {'/','*'};
	validRailMap[S] = allowedChars{listFromArray(Sleft,2),listFromArray(SStraight,5),listFromArray(SRight,2)};
	//Southwest
	char SWleft[3] = {'|','*','+'};
	char SWStraight[4] = {'-','/','|','*'};
	char SWRight[3] = {'-','*','+'};
	validRailMap[SW] = allowedChars{listFromArray(SWleft,3),listFromArray(SWStraight,4),listFromArray(SWRight,3)};
	//West
	char Wleft[2] = {'/','*'};
	char WStraight[5] = {'-','/','\\','+','*'};
	char WRight[2] = {'\\','*'};
	validRailMap[W] = allowedChars{listFromArray(Wleft,2),listFromArray(WStraight,5),listFromArray(WRight,2)};
	//Northwest
	char NWleft[3] = {'-','*','+'};
	char NWStraight[4] = {'-','\\','|','*'};
	char NWRight[3] = {'|','*','+'};
	validRailMap[NW] = allowedChars{listFromArray(NWleft,3),listFromArray(NWStraight,4),listFromArray(NWRight,3)};
	//North
	char Nleft[2] = {'\\','*'};
	char NStraight[5] = {'|','\\','/','*','+'};
	char NRight[2] = {'/','*'};
	validRailMap[N] = allowedChars{listFromArray(Nleft,2),listFromArray(NStraight,5),listFromArray(NRight,2)};
	//Northeast
	char NEleft[3] = {'|','*','+'};
	char NEStraight[4] = {'-','/','|','*'};
	char NERight[3] = {'-','*','+'};
	validRailMap[NE] = allowedChars{listFromArray(NEleft,3),listFromArray(NEStraight,4),listFromArray(NERight,3)};
}

