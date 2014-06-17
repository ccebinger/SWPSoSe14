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
    enum Type { RAIL, COM, STRING, FUNC, EMPTY, START, END, VER, GATE, SPEC, DEF, HOR};
    int muster, connections, bCons, color, x , y, start, stringMuster;
	char sign;
	Point* cons[8];
    bool string;
    enum Type type;
	

	private:
	void setMuster(void){
		switch(sign){
			case '$' :
                muster = 129;
                type = START;
                start = 129;
				break;
			case '@' :
                muster = 255;
                type = DEF;
				break;
			case '#' :
                muster = 255;
                type = END;
				break;
			case '-' :
				muster = 24;
                type = HOR;
				break;
			case '|' :
                muster = 66;
                type = VER;
				break;
			case '\\' :
                muster = 255;
                type = SPEC;
				break;
			case '/' :
                muster = 255;
                type = SPEC;
				break;
			case '^' :
				muster = 69;
                type = GATE;
				break;
			case '<' :
				muster = 49;
                type = GATE;
				break;
			case '>' :
				muster = 134;
                type = GATE;
				break;
			case 'v' :
				muster = 164;
                type = GATE;
				break;
			case '[' :
                muster = 255;
                type = FUNC;
				break;
			case ']' :
                muster = 255;
                type = FUNC;
				break;
			case '(' :
                muster = 255;
                type = FUNC;
				break;
			case ')' :
                muster = 255;
                type = FUNC;
				break;
            case '{' :
				muster = 255;
                type = FUNC;
				break;
			case '}' :
				muster = 255;
                type = FUNC;
                break;
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
        start = 0;
		this->sign = sign;
		setMuster();
		bCons = 0;
		connections = 0;
        if(type == STRING) string = true;
        else string = false;
        color = 0;
        this->x = x;
        this->y = y;
        stringMuster = 0;
	}
    Point(int x, int y){
        start = 0;
        sign = ' ';
        setMuster();
        connections = 0;
        if(type == STRING) string = true;
        else string = false;
        color = 0;
        this->x = x;
        this->y = y;
        bCons = 0;
        stringMuster = 0;
    }
    Point(Point *next, char sign, int x, int y){
        start = 0;
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
        stringMuster = 0;
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
protected:
    void makeColor(void){
        if(!string){
            switch(type){
            case START:
                color = 0x00FF0000;
                break;
            case END:
                color = 0x00FF0000;
                break;
            case GATE:
                color = 0;
                break;
            case VER:
                color = 0;
                break;
            case HOR:
                color = 0;
                break;
            case SPEC:
                color = 0;
                break;
            case COM:
                color = 0xFF000000;
                break;
            case STRING:
                color = 0xFF000000;
                break;
            case FUNC:
                color = 0x00FF0000;
                break;
            case EMPTY:
                color = 0xFF000000;
                break;
            default:
                color = 0xFF000000;
                break;
            }
        }else{
            switch(type){
            case FUNC:
                color = 0x0000FF00;
                break;
            default:
                color = 0x0000FF00;
                break;
            }
        }
    }
public:
    void setConnections(int connections, bool mode,InternStack *change, int start){
        int old = muster;
        muster = posibleCons();
        (mode)?this->connections |= connections:this->connections &= (~connections);
        if(muster != old){
            change->push(this);
        }old = this->start;
        if(start) this->start |= start;
        else if (!start && (connections & this->start) && (sign != '$')) {
            this->start = this->start & (~connections);
        }if(old != this->start) change->push(this);
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
    Point* get_D_Next(int con){
        if(con & 128) return (bCons & 128)? cons[0] : NULL;
        if(con & 64) return (bCons & 64)? cons[1] : NULL;
        if(con & 32) return (bCons & 32)? cons[2] : NULL;
        if(con & 16) return (bCons & 16)? cons[3] : NULL;
        if(con & 8) return (bCons & 8)? cons[4] : NULL;
        if(con & 4) return (bCons & 4)? cons[5] : NULL;
        if(con & 2) return (bCons & 2)? cons[6] : NULL;
        if(con & 1) return (bCons & 1)? cons[7] : NULL;
    }
    bool isString(int direction){
        if(stringMuster & direction) return true;
        else return false;
    }
    Type getType(void){
        return type;
    }

    int posibleCons(void){
        int start = this->start;
        int pCons = 0;
        switch (type) {
        case START:
            pCons = 1;
            break;
        case END:
            pCons = 0;
            break;
        case FUNC:
            if(start & 128 && cons[0]->getType() != VER && cons[0]->getType() != HOR) pCons |= 1;
            if(start & 64) pCons |= 2;
            if(start & 32 && cons[0]->getType() != VER && cons[0]->getType() != HOR) pCons |= 4;
            if(start & 16) pCons |= 8;
            if(start & 8) pCons |= 16;
            if(start & 4 && cons[0]->getType() != VER && cons[0]->getType() != HOR) pCons |= 32;
            if(start & 2) pCons |= 64;
            if(start & 1 && cons[0]->getType() != VER && cons[0]->getType() != HOR) pCons |= 128;
            break;
        case GATE:
            if(start & 16) pCons |= 33;
            if(start & 2) pCons |= 160;
            if(start & 8) pCons |= 132;
            if(start & 64) pCons |= 5;
        case SPEC:
            if(start & 128 && sign == '\\') pCons |= 11;
            if(start & 1 && sign == '\\') pCons |= 208;
            if(start & 4 && sign == '/') pCons |= 104;
            if(start & 32 && sign == '/') pCons |= 22;
            if(start & 16 && cons[3]->getType() == HOR && sign == '/') pCons |= 104;
            if(start & 16 && cons[3]->getType() == HOR && sign == '\\') pCons |= 11;
            if(start & 8 && cons[4]->getType() == HOR && sign == '/') pCons |= 22;
            if(start & 8 && cons[4]->getType() == HOR && sign == '\\') pCons |= 208;
            if(start & 64 && cons[1]->getType() == VER && sign == '/') pCons |=22;
            if(start & 64 && cons[1]->getType() == VER && sign == '\\') pCons |=11;
            if(start & 2 && cons[6]->getType() == VER && sign == '/') pCons |=104;
            if(start & 2 && cons[6]->getType() == VER && sign == '\\') pCons |=208;
            break;
        case HOR:
            if(start & 128 && cons[0]->getType() != HOR && cons[0]->getType() != VER) pCons |= 41;
            if(start & 4 && cons[5]->getType() != HOR && cons[5]->getType() != VER) pCons |= 41;
            if(start & 16) pCons |= 41;
            if(start & 32 && cons[2]->getType() != HOR && cons[2]->getType() != VER) pCons |= 148;
            if(start & 1 && cons[7]->getType() != HOR && cons[7]->getType() != VER) pCons |= 148;
            if(start & 8) pCons |= 148;
            break;
        case VER:
            if(start & 128 && cons[0]->getType() != HOR && cons[0]->getType() != VER) pCons |=7;
            if(start & 32 && cons[2]->getType() != HOR && cons[2]->getType() != VER) pCons |=7;
            if(start & 64) pCons |=7;
            if(start & 4 && cons[5]->getType() != HOR && cons[5]->getType() != VER) pCons |=224;
            if(start & 1 && cons[7]->getType() != HOR && cons[7]->getType() != VER) pCons |=224;
            if(start & 2) pCons |=224;
            break;
        default:
            break;
        }
        if(string) pCons |= stringMuster;
        return pCons;
    }

    void makeCons(InternStack *change){
        muster = posibleCons();
        if(muster) makeColor();
        else color = 0xFF000000;
        if(bCons & 128)cons[0]->setConnections(1,(muster&128)?true:false,change,(((~128) & start) && (muster & 128))?1:0);
        if(bCons & 64)cons[1]->setConnections(2,(muster&64)?true:false,change,(((~64) & start) && (muster & 64))?2:0);
        if(bCons & 32)cons[2]->setConnections(4,(muster&32)?true:false,change,(((~32) & start) && (muster & 32))?4:0);
        if(bCons & 16)cons[3]->setConnections(8,(muster&16)?true:false,change,(((~16) & start) && (muster & 16))?8:0);
        if(bCons & 8)cons[4]->setConnections(16,(muster&8)?true:false,change,(((~8) & start) && (muster & 8))?16:0);
        if(bCons & 4)cons[5]->setConnections(32,(muster&4)?true:false,change,(((~4) & start) && (muster & 4))?32:0);
        if(bCons & 2)cons[6]->setConnections(64,(muster&2)?true:false,change,(((~2) & start) && (muster & 2))?64:0);
        if(bCons & 1)cons[7]->setConnections(128,(muster&1)?true:false,change,(((~1) & start) && (muster & 1))?128:0);
	}
    int getRow(void){
        return x;
    }
    int getCol(void){
        return y;
    }
    int getStyle(void){
        return color;
    }
    int getStart(void){
        return start;
    }
    void setStringMuster(int muster){
        string = true;
        stringMuster |= muster;
    }
    bool setNoString(int muster){
        if(!string) return false;
        stringMuster &= (~muster);
        if(!stringMuster) string = false;
        return true;
    }
    int getStringDirections(void){
        return stringMuster;
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
