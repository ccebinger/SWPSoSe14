#ifndef INTERSTACK_H
#define INTERSTACK_H

#include <stddef.h>

class Point;

class Node{
private:
    Node *next;
    Node *left, *right;
    Node *parent;
    Point *h;
public:
    Node (void){
        next = NULL;
        h = NULL;
        left = NULL; right = NULL;
        parent = NULL;
    }
    Node (Point* p){
        next = NULL;
        h = p;
        left = NULL; right = NULL;
        this->parent = NULL;
    }
    Node (Point* p,Node* parent){
        next = NULL;
        h = p;
        left = NULL; right = NULL;
        this->parent = parent;
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
    Node* add(Point* p){
        parent = NULL;
        if(p==h) return NULL;
        else if(p>h){ if(right == NULL)right=new Node(p);return right;}
        else if(p<h){ if(left == NULL)left=new Node(p);return left;}
    }
    Node* add(Point* p,Node* n){
        if(p==h){return NULL;}
        else if(p>h){ if(right == NULL)right=new Node(p,n);return right;}
        else if(p<h){ if(left == NULL)left=new Node(p,n);return left;}
    }
    Node* getLast(void){
        if(left != NULL) return left;
        else if(right != NULL) return right;
        else return NULL;
    }
    Node* getParent(void){
        return parent;
    }
    void deleteNode(void){
        if(left != NULL){ delete left; left = NULL;}
        else if(right != NULL){ delete right; right = NULL;}
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
class DoubleCheck{
private:
    Node* start;
public:
    DoubleCheck(void){
        start = NULL;
    }
    void add(Point *p){
        Node* tmp = start;
        if(start != NULL)
            while((tmp = tmp->add(p,tmp)) != NULL);
        else
            start = new Node(p);
    }
    Point* get(void){
        Node *tmp1, *tmp2;
        Point *h = NULL;
        tmp1 = start;
        while(tmp1 != NULL){
            tmp2 = tmp1->getLast();
            if(tmp2 == NULL){
                h = tmp1->get();
                tmp2 = tmp1->getParent();
                if(tmp2 != NULL)tmp2->deleteNode();
                else {
                    delete tmp1; start = NULL;
                }
                return h;
            }else tmp1 = tmp2;
        } return h;
    }
};

#endif // INTERSTACK_H
