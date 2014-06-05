#ifndef INTERSTACK_H
#define INTERSTACK_H

class Point;

class Node{
private:
    Node *next;
    Point *h;
public:
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
public:
    void push(Point *h){
        first = new Node(first,h);
    }
    Point* pop(void){
        Node* tmp = first->getNext();
        Point* h = first->get();
        delete first;
        first = tmp;
        return h;
    }
};

#endif // INTERSTACK_H
