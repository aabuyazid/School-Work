//
// Created by Arkan Abuyazid on 2018-04-10.
//

#include "Node.h"

ExpTree::ExpTree(vector<Node*>& expr) {
    overallRoot = add(overallRoot, expr);
}

int ExpTree::parse() {
    return parse(overallRoot);
}

int ExpTree::parse(Node* root) {
    if(root->isOperand) return root->operand;
    int arg1 = parse(root->left);
    int arg2 = 0;
    if(root->right != NULL) arg2 = parse(root->right);
    return operation(arg1, arg2, root->optr);
}

int ExpTree::operation(int arg1, int arg2, string optr) {
    if (optr == "+") return arg1 + arg2;
    if (optr == "-") return arg1 - arg2;
    if (optr == "*") return arg1 * arg2;
    if (optr == "/") return arg1 / arg2;
    if (optr == "%") return arg1 % arg2;
    if (optr == "&&") return arg1 && arg2;
    if (optr == "||") return arg1 || arg2;
    if (optr == "<") return (arg1 < arg2) ? 1:0;
    if (optr == ">") return (arg1 > arg2) ? 1:0;
    if (optr == "==") return (arg1 == arg2) ? 1:0;
    if (optr == "!=") return (arg1 != arg2) ? 1:0;
    if (optr == "<=") return (arg1 <= arg2) ? 1:0;
    if (optr == ">=") return (arg1 >= arg2) ? 1:0;
    if(optr == "!") return (arg1 == 0) ? 1:0;
    if(optr == "~") return -arg1;
}

Node* ExpTree::add(Node* root, vector<Node*>& expr) {
    Node* first = expr[0];
    expr.erase(expr.begin());
    root = first;
    if(!(first->isOperand)) {
        root->left = add(root->left, expr);
        if(root->optr != "!" && root->optr != "~") {
            root->right = add(root->right, expr);
        }
    }
    return root;
}

