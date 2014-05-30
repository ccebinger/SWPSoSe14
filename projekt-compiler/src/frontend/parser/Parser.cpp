/*
 * Parser.cpp
 *
 *  Created on: 30.04.2014
 *      Author: LeonBornemann
 */


#include <frontend/lexer/Lexer.h>
#include <frontend/parser/Parser.h>


Parser::Parser(std::shared_ptr<RailFunction> railFunction) {
	this->board = railFunction;
	//errorMessage - empty String: Everything is ok
	errorMessage = "";
	abstractSyntaxGraph = NULL;
}

void Parser::setRowCol(int newRow, int newCol){
	posRow = newRow;
	posCol = newCol;
}

shared_ptr<Adjacency_list> Parser::parseGraph() {
	addNextNodeAsTruePathOfPreviousNode = true;
	int startPosY=-1;
	//find $, initiate pos
	for(uint32_t i=0; i<board->getWidth(); ++i) {
		if(board->get(0, i) == '$') {
			startPosY = i;
			break;
		}
	}
	if(startPosY==-1){
		errorMessage ="'" + board->getName() + "': $ not found";
		return NULL;
	}
	return parseGraph(0,startPosY,SE);
}

shared_ptr<Adjacency_list> Parser::parseGraph(int startPosRow, int startPosCol, Direction startDir) {
	cout << "Begin parsing..." << endl;
	setRowCol(startPosRow, startPosCol);
	dir = startDir;
	parsingNotFinished = true;
	while(parsingNotFinished) {
		cout << "\t@(" << posRow << ", " << posCol << ", " << Encoding::unicodeToUtf8(board->get(posRow, posCol)) << ")" << endl;
		move();
		if(errorMessage != ""){
			cout << "\t" << errorMessage <<endl;
			cout << "\tparsing aborted" << endl;
			break;
		}
	}
	cout << "Finished parsing" << endl;
	return abstractSyntaxGraph;
}

void Parser::move() {

	//straight
	uint32_t straightRow = posRow + rowOffsetMap.at(dir).offsets[STRAIGHT];
	uint32_t straightCol = posCol + colOffsetMap.at(dir).offsets[STRAIGHT];
	bool straightIsInBoardBounds = straightRow >= 0 && straightRow < board->getHeight() && straightCol >= 0 && straightCol < board->getWidth();

	//left
	uint32_t leftRow = posRow + rowOffsetMap.at(dir).offsets[LEFT];
	uint32_t leftCol = posCol + colOffsetMap.at(dir).offsets[LEFT];
	bool leftIsInBoardBounds =  leftRow >=0 && leftRow < board->getHeight() && leftCol >=0 && leftCol < board->getWidth();

	//right
	uint32_t rightRow = posRow + rowOffsetMap.at(dir).offsets[RIGHT];
	uint32_t rightCol = posCol + colOffsetMap.at(dir).offsets[RIGHT];
	bool rightIsInBoardBounds =  rightRow >=0 && rightRow < board->getHeight() && rightCol >=0 && rightCol < board->getWidth();

	//bool vars that will be checked in the end
	bool leftIsValidRail = false;
	bool rightIsValidRail = false;
	bool straightIsValidRail = false;
	if(straightIsInBoardBounds) {
		uint32_t charAtStraight = board->get(straightRow, straightCol);
		list<uint32_t> allowedRails = validRailMap.at(dir).straight;
		straightIsValidRail = std::find(allowedRails.begin(),allowedRails.end(),charAtStraight)!=allowedRails.end();
		if(straightIsValidRail) {
			//if allowedRails contains charAtStraight just move forwards
			setRowCol(straightRow,straightCol);
			if(charAtStraight == leftDirChangeMap.at(dir)){
				turnLeft45Deg();
			}
			if(charAtStraight == rightDirChangeMap.at(dir)){
				turnRight45Deg();
			}
			return;
		}
		//check for other symbols that are allowed
		bool didGoStraight = checkForValidCommandsInStraightDir(straightRow, straightCol);
		if(didGoStraight) {
			return;
		}
	}
	if(leftIsInBoardBounds) {
		uint32_t charAtLeft = board->get(leftRow, leftCol);
		list<uint32_t> allowedRailsLeft = validRailMap.at(dir).left;
		leftIsValidRail = std::find(allowedRailsLeft.begin(), allowedRailsLeft.end(), charAtLeft) != allowedRailsLeft.end();
	}
	if(rightIsInBoardBounds) {
		uint32_t charAtRight = board->get(rightRow, rightCol);
		list<uint32_t> allowedRailsRight = validRailMap.at(dir).right;
		rightIsValidRail = std::find(allowedRailsRight.begin(),allowedRailsRight.end(),charAtRight)!=allowedRailsRight.end();
	}

	// ---------------------------------------------------------------------
	// Begin of error handling
	// ---------------------------------------------------------------------
	if(leftIsValidRail && rightIsValidRail) {
		std::stringstream sstm;
		sstm << "'" << board->getName() << "' (" << posRow+1 << ", " << posCol+1 << "): ambiguous move";
		errorMessage = sstm.str();
		return;
	}
	if(!leftIsValidRail && !rightIsValidRail) {
		std::stringstream sstm;
		sstm << "'" << board->getName() << "' (" << posRow+1 << ", " << posCol+1 << "): no valid move possible";
		errorMessage = sstm.str();
		return;
	}
	//error handling end
	if(leftIsValidRail) {
		setRowCol(leftRow,leftCol);
		turnLeft45Deg();
		return;
	}
	if(rightIsValidRail) {
		setRowCol(rightRow,rightCol);
		turnRight45Deg();
		return;
	}
	errorMessage = "'" + board->getName() + "' : end of move-function reached - this should never happen and is an internal error";
	return;
}

bool Parser::checkForValidCommandsInStraightDir(int straightRow, int straightCol) {
	uint32_t charAtStraight = board->get(straightRow, straightCol);

	//cout << "\tcheckForValidCommandsInStraightDir(" << straightRow << ", " << straightCol << ") " << Encoding::unicodeToUtf8(charAtStraight) << endl;

	bool didGoStraight = true;
	switch(charAtStraight) {
		case 'o':
			setRowCol(straightRow, straightCol);
			addToAbstractSyntaxGraph("o", Command::Type::OUTPUT);
			break;
		case '[':
			setRowCol(straightRow, straightCol);
			//TODO: ueberpruefen ob notwendig: list<char> invalidCharList = {'[','{','(',},;
			addToAbstractSyntaxGraph(readCharsUntil(']'), Command::Type::PUSH_CONST);
			//TODO: create pushNode in graph
			break;
		case ']':
			setRowCol(straightRow, straightCol);
			addToAbstractSyntaxGraph(readCharsUntil('['), Command::Type::PUSH_CONST);
			break;
		case '@':
			setRowCol(straightRow, straightCol);
			reverseDirection();
			break;
		case '#':
			setRowCol(straightRow, straightCol);
			addToAbstractSyntaxGraph("#", Command::Type::FINISH);
			parsingNotFinished = false;
			break;
		case '<':
			didGoStraight = parseJunctions(E, straightRow, straightCol, SE, NE, "<", Command::Type::EASTJUNC);
			break;
		case '>':
			didGoStraight = parseJunctions(W, straightRow, straightCol, NW, SW, ">", Command::Type::WESTJUNC);
			break;
		case '^':
			didGoStraight = parseJunctions(S, straightRow, straightCol, SW, SE, "^", Command::Type::SOUTHJUNC);
			break;
		case 'v':
			didGoStraight = parseJunctions(N, straightRow, straightCol, NE, NW, "v", Command::Type::NORTHJUNC);
			break;
		case '{':
			setRowCol(straightRow, straightCol);
			addToAbstractSyntaxGraph(readCharsUntil('}'), Command::Type::CALL);
			break;
		case '}':
			setRowCol(straightRow, straightCol);
			addToAbstractSyntaxGraph(readCharsUntil('{'), Command::Type::CALL);
			break;
		case 't':
		case 'f':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			setRowCol(straightRow, straightCol);
			addToAbstractSyntaxGraph(Encoding::unicodeToUtf8(charAtStraight), Command::Type::PUSH_CONST);
			break;
		default:
			didGoStraight = false;
			break;
	}
	if(errorMessage!="") {
		//TODO:Error stuff?
	}
	return didGoStraight;
}



/*
 * requiredDir: The direction the train has to have at the moment for this being a valid junction (for dir must be E for < junction)
 * juncX/juncY are the coordinates of the junction
 * truePathDir/falsePathDir are the starting directions of the true/false paths
 * command name: the junction symbol as a string
 * juncType: ast.h Command::type enum value of the junction
*/
bool Parser::parseJunctions(Direction requiredDir, int juncRow, int juncCol, Direction truePathDir, Direction falsePathDir, string commandName, Command::Type juncType) {
	if(dir==requiredDir){
		addToAbstractSyntaxGraph(commandName, juncType);
		std::shared_ptr<Node> ifNode = currentNode;
		parseGraph(juncRow, juncCol, truePathDir);
		parsingNotFinished = true;
		currentNode = ifNode;
		addNextNodeAsTruePathOfPreviousNode = false;
		parseGraph(juncRow, juncCol, falsePathDir);
		parsingNotFinished = false;
		return true;
	} else{
		return false;
	}
}

void Parser::addToAbstractSyntaxGraph(string commandName, Command::Type type) {
	//debug
	cout << "\tNode creation: " << commandName << endl;
	std::shared_ptr<Node> node(new Node());
	node->command = {type,commandName};
	//TODO:an Graph Schnittstelle anpassen
	if(abstractSyntaxGraph == NULL) {
		//this is the first node that we meet create a new one
		node->id = 1;
		lastUsedId = 1;
		abstractSyntaxGraph.reset(new Adjacency_list(board->getName(), node));
	} else {
		node->id = ++lastUsedId;
		abstractSyntaxGraph->addNode(node);
		abstractSyntaxGraph->addEdge(currentNode, node, addNextNodeAsTruePathOfPreviousNode);
		if(!addNextNodeAsTruePathOfPreviousNode) {
			//restore default behavior: always add new nodes to the 'true' path of their predecessor
			//unless it was set before the call of this method (in case an 'IF' was read and we parse the first node of the 'false' branch
			addNextNodeAsTruePathOfPreviousNode = true;
		}
	}
	currentNode = node;
}

//FIXME translate -> english
//setzt position auf until falls er existiert, und gibt den gelesenen string inklusive anfangs und endzeichen zurueck
//falls nicht wir ein leerer string zurueckgegeben und die fehlermeldung gesetzt
string Parser::readCharsUntil(uint32_t until) {
	string result = "";

	//result += board->get(posRow, posCol);

	// Can be decoded anyway - doesn't matter
	result += Encoding::unicodeToUtf8(board->get(posRow, posCol));
	while(true) {
		uint32_t nextRow = posRow + rowOffsetMap.at(dir).offsets[STRAIGHT];
		uint32_t nextCol = posCol + colOffsetMap.at(dir).offsets[STRAIGHT];
		if(posRow >= board->getHeight() || nextCol >= board->getWidth()) {
			//TODO:Dir auch ausgeben
			std::stringstream sstm;
			sstm << "'" << board->getName() << "' (" << posRow+1 << ", " << posCol+1 << "): Parsing ran out of valid space";
			errorMessage = sstm.str();
			return "";
		}
		posRow = nextRow;
		posCol = nextCol;

		//result += board->get(posRow, posCol);
		result += Encoding::unicodeToUtf8(board->get(posRow, posCol));

		if(board->get(posRow, posCol) == until) {
			break;
		}
	}
	//cout << "readCharsUntil: " << result << endl;
	return result;
}

void Parser::turnLeft45Deg() {
	switch(dir) {
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

void Parser::turnRight45Deg() {
	switch(dir) {
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

void Parser::reverseDirection() {
	switch(dir) {
		case E: dir = W; break;
		case SE: dir = NW; break;
		case S: dir = N; break;
		case SW: dir = NE; break;
		case W: dir = E; break;
		case NW: dir = SE; break;
		case N: dir = S; break;
		case NE: dir = SW; break;
	}
}






