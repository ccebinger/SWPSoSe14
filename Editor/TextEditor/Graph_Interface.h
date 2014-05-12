#ifndef GRAPHINTERFACE_H
#define GRAPHINTERFACE_H

//Nur diese Klasse verwenden um auf die Interne Repräsentation zuzugreifen
#include "Point.h"

class Graph_Interface{
	private:
	Point *root, *tmp;
	

	public:
	Graph_Interface(){
		root = new Point();
	}
	Graph_Interface(Point* root){
		this->root = root;
	}
	void setSign(int x, int y, char sign){
		root->getPoint(x,y)->setSign(sign);
	}
	void insertSign(int x, int y, char sign){
		root->getPoint(x,y)->insertSign(sign);
	}
	void setCharArray(int x, int y, char signs[], int length){
		Point *tmp2;
		int saveX = x, saveY = y, i = 0;
		tmp = root->getPoint(x,y);
		for( ; i < length ; i++){
			if(signs[i] == '\n'){
				tmp->setSign(' ');
				tmp = root->getPoint(x,++y);
			}else{
				tmp->setSign(signs[i]);
				tmp = tmp->getPoint(1,0);
			}
		}
	}
	char getSign(int x,int y){
		return root->getPoint(x,y)->getSign();
	}
	char* getLine(int y){
		return (y < root->countRift()) ? root->getPoint(0,y)->getLine(root->getPoint(0,y)->countLine()): NULL;
	}
	void clear(void){
		root->~Point();
		root = new Point();
	}
	char* deleteSign(int x, int y){
		root->getPoint(x,y)->deleteSign();
		return (y < root->countRift()) ? root->getPoint(0,y)->getLine(root->getPoint(0,y)->countLine()): NULL;
	}
	Graph_Interface* clone(void){
		tmp = new Point();
		root->clone(tmp);
		return new Graph_Interface(tmp);
	}
	int getMaxColm(void){
		return root->countRift();
	}
	int getMaxRow(void){
		int x, k = root->countRift(), line;
		for(int i = 0; i < k; i++){
			x = (x<( line = root->countLine())) ? line : x;
		}; 
		return x;
	}
	void makeConnections(void){
		
	}
};
#endif // GRAPHINTERFACE_H
