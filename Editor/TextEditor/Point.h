#ifndef POINT_H
#define POINT_H

#include "InterStack.h"
#include "Stack.h"
#include <iostream>
#include <QDebug>

// Interne Repräsentation, wird nur von Graph_Interface verwendet
// color bytemask: Red: 128,Grey: 64, Blue:32, Green: 16, Fett:8, Kursiv: 4
// colors Red: commend, Green: Funktion, Black: Connected Rail, Grey: unconnectetd Rail, Blue: String, Blue+Fet: String+Funktion, Kursiv: not used yet, if no color is set color is black.
class Point{
	private:
    enum type { RAIL, COM, STRING, FUNC, EMPTY };
    int muster, connections, bCons, color, x , y;
	char sign;
	Point* cons[8];
    bool string;
    enum type type;
	

	private:
	void setMuster(void){
		switch(sign){
			case '$' :
                muster = 3;
                type = FUNC;
				break;
			case '@' :
                muster = 255;
                type = FUNC;
				break;
			case '#' :
                muster = 255;
                type = FUNC;
				break;
			case '-' :
				muster = 24;
                type = RAIL;
				break;
			case '|' :
				muster = 66;
                type = RAIL;
				break;
			case '\\' :
				muster = 219;
                type = RAIL;
				break;
			case '/' :
				muster = 126;
                type = RAIL;
				break;
			case '^' :
				muster = 69;
                type = RAIL;
				break;
			case '<' :
				muster = 49;
                type = RAIL;
				break;
			case '>' :
				muster = 134;
                type = RAIL;
				break;
			case 'v' :
				muster = 164;
                type = RAIL;
				break;
			case '[' :
                muster = 255;
                type = RAIL;
				break;
			case ']' :
                muster = 255;
                type = RAIL;
				break;
			case '(' :
                muster = 255;
                type = FUNC;
				break;
			case ')' :
                muster = 255;
                type = FUNC;
				break;
            /*case '{' :
				muster = 255;
				break;
			case '}' :
				muster = 255;
                break;*/
			case ' ' :
				muster = 0;
                type = EMPTY;
				break;
			default :
                muster = 0;
                type = EMPTY;
				break;
		}
	}
	public:
    Point(char sign, int x, int y){
		this->sign = sign;
		setMuster();
		bCons = 0;
		connections = 0;
        if(type == STRING) string = true;
        else string = false;
        color = 0;
        this->x = x;
        this->y = y;
	}
    Point(int x, int y){
        sign = ' ';
        setMuster();
        connections = 0;
        if(type == STRING) string = true;
        else string = false;
        color = 0;
        this->x = x;
        this->y = y;
        bCons = 0;
    }
    Point(Point *next, char sign, int x, int y){
		bCons = 8;
		cons[4] = next;
		this->sign = sign;
		setMuster();
		connections = 0;
        if(type == STRING) string = true;
        else string = false;
        color = 0;
        this->x = x;
        this->y = y;
	}
    Point* getPoint (int x, int y){
		if((bool)y){
			if(bCons & 2)
				return cons[6]->getPoint(x,y-1);
			else {
				bCons |= 2;
                cons[6] = new Point(this->x,this->y+1);
				return cons[6]->getPoint(x,y-1);
			} }
		else if((bool)x){
			if(bCons & 8)
				return cons[4]->getPoint(x-1,y);
			else {
				bCons |= 8;
                cons[4] = new Point(this->x+1,this->y);
				return cons[4]->getPoint(x-1,y);
			} }
		else
			return this;
	}
	char getSign(void){
		return sign;
	}
	int setSign(char sign){
		this->sign = sign;
		setMuster();
		return bCons;
	}
	char* getLine(int length){
		Point *tmp = this;
		char *line = new char[length];
		for( int z = 0 ; z < length ; z++){
			line[z] = tmp->getSign();
			tmp = tmp->getNextRight();
		}
		line[length] = '\n';
		return line;
	}
	char* getConnectionsLine(int length){
		Point *tmp = this;
		char *line = new char[length];
		for( int z = 0 ; z < length ; z++){
			line[z] = (char)(tmp->getConnections()+48);
			tmp = tmp->getNextRight();
		}
		line[length] = '\n';
		return line;
	}
	int countLine(){
		return (bCons & 8) ? ( 1 + cons[4]->countLine()) : 1;
	}
	int countRift(){
		return (bCons & 2) ? ( 1 + cons[6]->countRift()) : 1;
	}
	void clear(void);
	void clone(Point *akk);
	int getMuster(void){
		return muster;
	}
    void setConnections(int connections, bool mode,InternStack *change){
        int old = this->connections, count = 0;
        (mode)?this->connections |= connections:this->connections &= (~connections);
        if((old&muster) != (muster&this->connections)){
            if(!string){
                switch(type){
                case RAIL:
                    for(int i = 0; i<8;i++){
                        if(this->connections % 2) count++;
                        this->connections >>= 1;
                    }
                    if(count < 2) color = 64;
                    break;
                case COM:
                    color = 128;
                    break;
                case STRING:
                    color = 32;
                    break;
                case FUNC:
                    color = 16;
                    break;
                case EMPTY:
                    color = 0;
                    break;
                default:
                    color = 0;
                    break;
                }
            }else{
                switch(type){
                case FUNC:
                    color = 40;
                    break;
                default:
                    color = 32;
                    break;
                }
            }
            change->push(this);
        }
	}
	void testConnections(void){
        // TODO: set connections
	}
	int getConnections(void){
		return connections&muster ? 1 : 0;
	}
	void setCons(int pCon, Point *tmp){
		switch(pCon){
			case 128:
				cons[0] = tmp;
				break;
			case 64:
				cons[1] = tmp;
				break;
			case 32:
				cons[2] = tmp;
				break;
			case 16:
				cons[3] = tmp;
				break;
			case 8:
				cons[4] = tmp;
				break;
			case 4:
				cons[5] = tmp;
				break;
			case 2:
				cons[6] = tmp;
				break;
			case 1:
				cons[7] = tmp;
				break;
		}bCons |= pCon;
	}
    Point* setString(int con){
        string = true;
        if(con & 128) return (bCons & 1)? cons[7] : NULL;
        if(con & 64) return (bCons & 2)? cons[6] : NULL;
        if(con & 32) return (bCons & 4)? cons[5] : NULL;
        if(con & 16) return (bCons & 8)? cons[4] : NULL;
        if(con & 8) return (bCons & 16)? cons[3] : NULL;
        if(con & 4) return (bCons & 32)? cons[2] : NULL;
        if(con & 2) return (bCons & 64)? cons[1] : NULL;
        if(con & 1) return (bCons & 128)? cons[0] : NULL;
    }
    void getStringDirection(Stack *directions, int x, int y){
        if(!string)return;
        if(connections & 128)directions->push(x+1,y+1,' ',128);
        if(connections & 64)directions->push(x,y+1,' ',64);
        if(connections & 32)directions->push(x-1,y+1,' ',32);
        if(connections & 16)directions->push(x+1,y,' ',16);
        if(connections & 8)directions->push(x-1,y,' ',8);
        if(connections & 4)directions->push(x+1,y-1,' ',4);
        if(connections & 2)directions->push(x,y-1,' ',2);
        if(connections & 1)directions->push(x-1,y-1,' ',1);
    }

    void makeCons(InternStack *change){
		int tmp = bCons;
        if(tmp & 128)cons[0]->setConnections(1,(muster&128)?true:false,change);
        if(tmp & 64)cons[1]->setConnections(2,(muster&64)?true:false,change);
        if(tmp & 32)cons[2]->setConnections(4,(muster&32)?true:false,change);
        if(tmp & 16)cons[3]->setConnections(8,(muster&16)?true:false,change);
        if(tmp & 8)cons[4]->setConnections(16,(muster&8)?true:false,change);
        if(tmp & 4)cons[5]->setConnections(32,(muster&4)?true:false,change);
        if(tmp & 2)cons[6]->setConnections(64,(muster&2)?true:false,change);
        if(tmp & 1)cons[7]->setConnections(128,(muster&1)?true:false,change);
	}
	protected:
	Point* getNextRight(void){
		return (bCons & 8) ? cons[4] : NULL;
	}
	Point* getNextBottem(void){
		return (bCons & 2) ? cons[6] : NULL;
	}
	void setNextRight(Point* right){
		cons[4] = right;
		bCons |= 8;
	}
	void setNextBottem(Point *bottem){
		cons[6] = bottem;
		bCons |= 2;
	}
};
#endif // POINT_H
