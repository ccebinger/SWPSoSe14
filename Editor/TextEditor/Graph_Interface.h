#ifndef GRAPHINTERFACE_H
#define GRAPHINTERFACE_H


#include "Point.h"
#include "InterStack.h"
#include "Stack.h"

//Nur diese Klasse verwenden um auf die Interne Repräsentation zuzugreifen


// TODO: UNDO STring
// TODO: Know Commands
// TODO: redo
class Graph_Interface{
	private:
	Point *root, *tmp;
	Stack *undo;
    DoubleCheck *check;
	

	public:
	Graph_Interface(){
        root = new Point(0,0);
		undo = new Stack();
        check = new DoubleCheck();
	}
    // dont use clone at the moment not safe;
	Graph_Interface(Point* root){
		this->root = root;
		undo = new Stack();
        check = new DoubleCheck();
	}
	private:
    Stack* setSignIntern(int x,int y, char sign){
        Stack *retStack = new Stack();
		int bCons = tmp->setSign(sign);
		if(!(bCons & 128) && x-1 >= 0 && y-1 >= 0)tmp->setCons(128,root->getPoint(x-1,y-1));
		if(!(bCons & 64) && y-1 >= 0)tmp->setCons(64,root->getPoint(x,y-1));
		if(!(bCons & 32) && y-1 >= 0)tmp->setCons(32,root->getPoint(x+1,y-1));
		if(!(bCons & 16) && x-1 >= 0)tmp->setCons(16,root->getPoint(x-1,y));
		if(!(bCons & 8))tmp->setCons(8,root->getPoint(x+1,y));
		if(!(bCons & 4) && x-1 >= 0)tmp->setCons(4,root->getPoint(x-1,y+1));
		if(!(bCons & 2))tmp->setCons(2,root->getPoint(x,y+1));
		if(!(bCons & 1))tmp->setCons(1,root->getPoint(x+1,y+1));
        InternStack* change = new InternStack();
        do{
        tmp->makeCons(change);
        check->add(tmp);
        }while((tmp = change->pop()) != NULL);
        while((tmp = check->get()) != NULL){
            retStack->push(tmp->getRow(),tmp->getCol(),tmp->getSign(), tmp->getStyle());
        }
        return retStack;
	}
	public:
    Stack* setSign(int x, int y, char sign){
		tmp = root->getPoint(x,y);
        //undo->push(x,y,tmp->getSign(),0);
        return setSignIntern(x,y,sign);
	}
    Stack* makeUndo(void){
		Stack *element = undo->pop();
		setSignIntern(element->getX(),element->getY(),element->getSign());
        return NULL;
	}
	void insertSign(int x, int y, char sign){
		// TODO: INSERT Sign
	}
    Stack* setCharArray(int x, int y, char *signs,int length){
		int tmp = x;
		for(int i = 0;i<length;i++){
			if(signs[i]=='\n'){y++;x = tmp;}
			else setSign(x++,y,signs[i]);
		}
        return NULL;
	}
	char getSign(int x,int y){
		return root->getPoint(x,y)->getSign();
	}
	char* getLine(int y){
		return (y < root->countRift()) ? root->getPoint(0,y)->getLine(root->getPoint(0,y)->countLine()): NULL;
	}
	char* getConnectionsLine(int y){
		return (y < root->countRift()) ? root->getPoint(0,y)->getConnectionsLine(root->getPoint(0,y)->countLine()): NULL;
	}
	void clear(void){
		root->clear();
        root = new Point(0,0);
	}
	char* deleteSign(int x, int y){
		// TODO: DELETE SIGN
        return NULL;
	}

	Graph_Interface* clone(void){
        tmp = new Point(0,0);
		root->clone(tmp);
		return new Graph_Interface(tmp);
	}
	int getMaxColm(void){
		return root->countRift();
	}
	int getMaxRow(void){
		int x = 0, k = root->countRift(), line;
		for(int i = 0; i < k; i++){
			x = (x<( line = root->getPoint(0,i)->countLine())) ? line : x;
		}; 
		return x;
	}
	void makeConnections(void){
		int maxX = getMaxRow(), maxY = getMaxColm(), connection;
		/*for(int y = 0; y < maxY; y++){
			for(int x = 0; x<maxX; x++){
				connection = root->getPoint(x,y)->getMuster();
				if(connection&128 && y-1>=0 && x-1>=0)root->getPoint(x-1,y-1)->setConnections(1);
				if(connection&64 && y-1>=0)root->getPoint(x,y-1)->setConnections(2);
				if(connection&32 && y-1>=0 && x+1<maxX)root->getPoint(x+1,y-1)->setConnections(4);
				if(connection&16 && x-1>=0 )root->getPoint(x-1,y)->setConnections(8);
				if(connection&8 && x+1<maxX)root->getPoint(x+1,y)->setConnections(16);
				if(connection&4 && y+1<maxY && x-1>=0)root->getPoint(x-1,y+1)->setConnections(32);
				if(connection&2 && y+1<maxY)root->getPoint(x,y+1)->setConnections(64);
				if(connection&1 && y+1<maxY && x+1<maxX)root->getPoint(x+1,y+1)->setConnections(128);
			}std::cout << "\n";
		}*/
		//std::cout << getMaxRow() << " " << maxY << " ";
		for(int y=0;y<maxY;y++){
			for(int x=0;x<maxX;x++){
				root->getPoint(x,y)->testConnections();
            }
		} 
	}
};
#endif // GRAPHINTERFACE_H
