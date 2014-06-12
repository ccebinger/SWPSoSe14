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
    struct stringMarker{
        int x;
        int y;
        stringMarker *next;
    };
    stringMarker *stringSymbols;

public:
	Graph_Interface(){
        root = new Point(0,0);
		undo = new Stack();
        check = new DoubleCheck();
        stringSymbols = NULL;
	}
    // dont use clone at the moment not safe;
	Graph_Interface(Point* root){
		this->root = root;
		undo = new Stack();
        check = new DoubleCheck();
	}
private:
    void addStringSymbol(int x, int y){
        if(stringSymbols == NULL){
            stringSymbols = new stringMarker;
            stringSymbols->x = x;
            stringSymbols->y = y;
            stringSymbols->next = NULL;
        }else{
            stringMarker *tmp = new stringMarker;
            tmp->x = x;
            tmp->y = y;
            tmp->next = stringSymbols;
            stringSymbols = tmp;
        }
    }
    int stringDirections(int x, int y){
        stringMarker *tmp = stringSymbols;
        int directions = 0;
        while(tmp != NULL){
            if(x==tmp->x && y<tmp->y) directions |= 2;
            else if(x==tmp->x && y>tmp->y) directions |= 64;
            else if(y==tmp->y && x<tmp->x) directions |= 8;
            else if(y==tmp->y && x>tmp->x) directions |= 16;
            else if(y-tmp->y == x-tmp->x){
                if(x-tmp->x > 0) directions |= 128;
                else if(x-tmp->x < 0) directions |= 1;
            }
            else if(y-tmp->y == -(x-tmp->x)){
                if(x-tmp->x > 0) directions |= 4;
                else if(x-tmp->x < 0) directions |= 32;
            } tmp = tmp->next;
        }
        return directions;
    }
    void removeStringSymbol(int x, int y){
        stringMarker *temp1 = stringSymbols, *temp2 = stringSymbols->next;
        if(temp1->x == x && temp1->y == y){
            stringSymbols = temp1->next;
            delete temp1;
            return;
        }
        while(temp2 != NULL){
            if(temp2->x == x && temp2->y == y){
                temp1->next = temp2->next;
                delete temp2;
                return;
            }temp1 = temp2; temp2 = temp2->next;
        }
    }

    Stack* setSignIntern(int x,int y, char sign){
        Stack *retStack = new Stack();
        InternStack* change = new InternStack();
        if(tmp->getSign() == '[' || tmp->getSign() == ']'){
            removeStringSymbol(x,y);
            deleteString(change,x,y);
        }
		int bCons = tmp->setSign(sign);
		if(!(bCons & 128) && x-1 >= 0 && y-1 >= 0)tmp->setCons(128,root->getPoint(x-1,y-1));
		if(!(bCons & 64) && y-1 >= 0)tmp->setCons(64,root->getPoint(x,y-1));
		if(!(bCons & 32) && y-1 >= 0)tmp->setCons(32,root->getPoint(x+1,y-1));
		if(!(bCons & 16) && x-1 >= 0)tmp->setCons(16,root->getPoint(x-1,y));
		if(!(bCons & 8))tmp->setCons(8,root->getPoint(x+1,y));
		if(!(bCons & 4) && x-1 >= 0)tmp->setCons(4,root->getPoint(x-1,y+1));
		if(!(bCons & 2))tmp->setCons(2,root->getPoint(x,y+1));
		if(!(bCons & 1))tmp->setCons(1,root->getPoint(x+1,y+1));
        do{
        tmp->makeCons(change);
        check->add(tmp);
        if(tmp->getSign() == '[' || tmp->getSign() == ']'){
            addStringSymbol(x,y);
            this->stringSearch(x,y,change,stringDirections(x,y));
        }
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
    Stack* deleteSign(int x, int y){
        // TODO: implement DELETE SIGN
        return new Stack();
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
protected:
    void stringSearch(int x, int y, InternStack *change, int direction){
        Point* temp1 = tmp, *temp2;
        InternStack *string = new InternStack();
        int maxY = getMaxColm(), maxX = getMaxRow();
        int startX = x, startY = y;
        int start = direction;
        int *directions = new int[8];
        int count = 0;
        bool found = false, isString =false;
        if(start & 128)directions[count++]=128;
        if(start & 64)directions[count++]=64;
        if(start & 32)directions[count++]=32;
        if(start & 16)directions[count++]=16;
        if(start & 8)directions[count++]=8;
        if(start & 4)directions[count++]=4;
        if(start & 2)directions[count++]=2;
        if(start & 1)directions[count++]=1;
        for (int var = 0; var < count; var++) {
            temp2 = temp1->get_D_Next(directions[var]);
            isString = temp2->isString(directions[var]);
            while (!isString && !found && x>=0 && x<=maxX && y>=0 && y<=maxY){
                switch(directions[var]){
                    case 128:
                        x--;y--;
                        break;
                    case 64:
                        y--;
                        break;
                    case 32:
                        x++;y--;
                        break;
                    case 16:
                        x--;
                        break;
                    case 8:
                        x++;
                        break;
                    case 4:
                        y++;x--;
                        break;
                    case 2:
                        y++;
                        break;
                    case 1:
                        y++;x++;
                        break;
                }
                if(temp2 == NULL) temp2 = root->getPoint(x,y);
                temp1 = temp2;
                if(temp1->getSign() == '[' || temp1->getSign() == ']' && temp1->getStart()){ found = true;break;}
                else string->push(temp2);
                temp2 = temp1->get_D_Next(directions[var]);
            }
            if(directions[var] & 129)start = 129;
            else if(directions[var] & 66)start = 66;
            else if(directions[var] & 36)start = 36;
            else if(directions[var] & 24)start = 24;
            while(found && (temp1 = string->pop()) != NULL){
                temp1->setStringMuster(start);
                change->push(temp1);
            }string->clear();
            x = startX; y = startY; temp1 = tmp;
            isString = false;
        }
        }
    void deleteString(InternStack *change,int x, int y){
        int startX = x, startY = y;
        Point *temp1 = tmp, *temp2;
        int *stringDirections = new int[8];
        int count = 0, muster = 0;
        int direction = tmp->getStart();
        if(direction & 128)stringDirections[count++] = 128;
        if(direction & 64)stringDirections[count++] = 64;
        if(direction & 32)stringDirections[count++] = 32;
        if(direction & 16)stringDirections[count++] = 16;
        if(direction & 8)stringDirections[count++] = 8;
        if(direction & 4)stringDirections[count++] = 4;
        if(direction & 2)stringDirections[count++] = 2;
        if(direction & 1)stringDirections[count++] = 1;
        for (int var = 0; var < count; var++) {
            if(stringDirections[var] & 129) muster = 129;
            if(stringDirections[var] & 66) muster = 66;
            if(stringDirections[var] & 36) muster = 36;
            if(stringDirections[var] & 24) muster = 24;
            temp2 = temp1->get_D_Next(stringDirections[var]);
            while (temp2->setNoString(muster)){
                switch(stringDirections[var]){
                    case 128:
                        x--;y--;
                        break;
                    case 64:
                        y--;
                        break;
                    case 32:
                        x++;y--;
                        break;
                    case 16:
                        x--;
                        break;
                    case 8:
                        x++;
                        break;
                    case 4:
                        y++;x--;
                        break;
                    case 2:
                        y++;
                        break;
                    case 1:
                        y++;x++;
                        break;
                }
                if(temp2 == NULL) temp2 = root->getPoint(x,y);
                temp1 = temp2;
                change->push(temp1);
                temp2 = temp1->get_D_Next(stringDirections[var]);
            } x = startX; y = startY;
        }
    }
};
#endif // GRAPHINTERFACE_H

/* Point* temp1 = tmp, *temp2;
        InternStack *string = new InternStack();
        int maxY = getMaxColm(), maxX = getMaxRow();
        int startX = x, startY = y;
        int start = tmp->getStart();
        int *directions = new int[8];
        int count = 0;
        bool found = false;
        if(start & 128)directions[count++]=128;
        if(start & 64)directions[count++]=64;
        if(start & 32)directions[count++]=32;
        if(start & 16)directions[count++]=16;
        if(start & 8)directions[count++]=8;
        if(start & 4)directions[count++]=4;
        if(start & 2)directions[count++]=2;
        if(start & 1)directions[count++]=1;
        for (int var = 0; var < count; var++) {
            do {
                temp2 = temp1->get_D_Next(directions[var]);
                switch(directions[var]){
                case 128:
                    x++;y++;
                    break;
                case 64:
                    y++;
                    break;
                case 32:
                    x--;y++;
                    break;
                case 16:
                    x++;
                    break;
                case 8:
                    x--;
                    break;
                case 4:
                    y--;x++;
                    break;
                case 2:
                    y--;
                    break;
                case 1:
                    y--;x--;
                    break;
                }
                if(temp2 == NULL) temp2 = root->getPoint(x,y);
                string->push(temp2);
                temp1 = temp2;
                if(temp1->getSign() == '[' || temp1->getSign() == ']' ) found = true;
            } while (!found && x>=0 && x<=maxX && y>=0 && y<=maxY);
            if(directions[var] & 129)start = 129;
            else if(directions[var] & 66)start = 66;
            else if(directions[var] & 36)start = 36;
            else if(directions[var] & 24)start = 24;
            while(found && (temp1 = string->pop()) != NULL){
                temp1->setStringMuster(start);
            }string->clear();
            x = startX; y = startY; temp1 = tmp;
        }
*/
