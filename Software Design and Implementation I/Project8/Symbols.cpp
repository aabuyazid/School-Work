//
// Created by Arkan Abuyazid on 2018-04-22.
//

#include "Symbols.h"

map<string, int> table;
map<string, int>::iterator iterator1;
vector<Node*> expression;
vector<Node*> copyExpr;
int lenExpr = 0;

void createExpression() {
    string peek;
    peek = peek_next_token();
    Node* token;
    while(peek != "text" && peek != "output"
          && peek != "var" && peek != "set" && peek != "//") {
        read_next_token();
        string next = next_token();
        iterator1 = table.find(next);
        if(next_token_type == NUMBER) {
            token = new Node(true, token_number_value, "/0");
        }
        else if(iterator1 != table.end()){
            int value = table[next];
            token = new Node(true, value, "/0");
        }
        else {
            token = new Node(false, 0, next);
        }
        expression.push_back(token);
        lenExpr++;
        peek = peek_next_token();
    }
}

void destroyExpression() {
    for(int i = 0; i < lenExpr; i++) {
        expression.pop_back();
        copyExpr.pop_back();
    }
    lenExpr = 0;
}

void cmdVar() {
    read_next_token();
    string next = next_token();
    iterator1 = table.find(next);
    if(iterator1 == table.end()) {
        table.insert(pair<string, int>(next, 0));
    }
    else {
        cout << "variable " << next << " incorrectly re-initialized\n";
    }
    createExpression();
    copyExpr = expression;
    ExpTree exptree = ExpTree(copyExpr);
    table[next] = exptree.parse();
    destroyExpression();
}

void cmdSet() {
    read_next_token();
    string next = next_token();
    iterator1 = table.find(next);
    if(iterator1 == table.end()) {
        cout << "variable " << next << " not declared\n";
    }
    createExpression();
    copyExpr = expression;
    ExpTree exptree(copyExpr);
    table[next] = exptree.parse();
    destroyExpression();
}

void cmdOut() {
    createExpression();
    copyExpr = expression;
    ExpTree exptree(copyExpr);
    cout << exptree.parse();
    destroyExpression();
}

void destroyTable() {
    table.clear();
}