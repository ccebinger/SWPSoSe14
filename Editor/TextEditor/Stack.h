#ifndef STACK_H
#define STACK_H

class Stack{

	private:
	Stack *next, *parent;
	int count;
	int x,y;
	char sign;
	int color;

	public:
	Stack(void){
		next = NULL;
		parent = NULL;
	}
	Stack(int x, int y,char sign,int color,Stack *parent){
		this->x = x;
		this->y = y;
		this->sign = sign;
		this->color = color;
		this->parent = parent;
		next = NULL;
	}
	~Stack(void){
		if(next != NULL)delete next;
	}
	void push(int x,int y,char sign, int color){
		this->next = new Stack(x,y,sign,color,this);
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
	protected:
	Stack* getNext(void){
		return next;
	}	
};

#endif // STACK_H
