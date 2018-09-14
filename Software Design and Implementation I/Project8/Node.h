//
// Created by Arkan Abuyazid on 2018-04-10.
//

#ifndef PROJECT8_NODE_H
#define PROJECT8_NODE_H
#include <vector>
#include <string.h>
#include <iostream>
using namespace std;


class Node {
public:
    bool isOperand;
    int operand;
    string optr;
    Node* left;
    Node* right;
    Node(bool isOperand, int operand, string optr) {
        left = NULL;
        right = NULL;
        this->isOperand = isOperand;
        this->operand = operand;
        this->optr = optr;
    }
    void print() {
        if (isOperand) {
                cout << operand << endl;
        }
        else {
            cout << optr << endl;
        }
    }
};
class ExpTree {
private:
    Node* overallRoot;
    Node* add(Node* root, vector<Node*>& expr);
    int parse(Node*);
    int operation(int arg1, int arg2, string optr);
public:
    int parse();
    ExpTree(vector<Node*>& expr);
};


#endif //PROJECT8_NODE_H
