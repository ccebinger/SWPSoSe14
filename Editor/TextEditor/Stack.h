#include <stddef.h>
#ifndef STACK_H
#define STACK_H

class Stack{

	private:
    Stack *next;
	int count;
	int x,y;
	char sign;
	int color;

	public:
	Stack(void){
        next = NULL;
	}
    Stack(int x, int y,char sign,int color,Stack *next){
		this->x = x;
		this->y = y;
		this->sign = sign;
        this->color = color;
        this->next = next;
	}
    ~Stack(void){
	}
	void push(int x,int y,char sign, int color){
        this->next = new Stack(x,y,sign,color,next);
	}
	Stack* pop(void){
		Stack *tmp;
        if(next != NULL){
			tmp = next;
			next = tmp->getNext();
			return tmp;
        } return NULL;
	}
	int getX(void){
		return x;
	}
	int getY(void){
		return y;
	}
	char getSign(void){
		return sign;
	}
    int getColor(void){
        return color;
    }

	protected:
	Stack* getNext(void){
		return next;
	}	
};

#endif // STACK_H
