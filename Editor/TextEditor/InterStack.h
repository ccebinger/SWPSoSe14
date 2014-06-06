#ifndef INTERSTACK_H
#define INTERSTACK_H

#include <stddef.h>

class Point;

class Node{
private:
    Node *next;
    Point *h;
public:
    Node (void){
        next = NULL;
        h = NULL;
    }
    Node (Node *next, Point* h){
        this->next = next;
        this->h = h;
    }
    Point* get(void){
        return h;
    }
    Node* getNext(void){
        return next;
    }
};
class InternStack{
private:
    Node *first;
    int count;
public:
    InternStack(void){
        first = new Node();
        count = 0;
    }

    void push(Point *h){
        first = new Node(first,h);
        count++;
    }
    Point* pop(void){
        Node* tmp = first->getNext();
        if(tmp != NULL){
        Point* h = first->get();
        delete first;
        first = tmp;
        count--;
        return h;}
        return NULL;
    }
};

#endif // INTERSTACK_H
