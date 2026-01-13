#pragma once
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

const bool BLACK = 1;
const bool RED = 0;
const int nil = -99999;

struct RBnode {
    int value;
    bool color;   // RED // BLACK
    RBnode* parent;
    RBnode* Lchild;
    RBnode* Rchild;
    RBnode(int v, bool c) : value(v), color(c),
                            parent(nullptr),
                            Lchild(nullptr),
                            Rchild(nullptr) {}
};

class RB_tree {
    private:
    void pri_printTree(RBnode* Node,
                        const string& prf_Rchild,  // 往右子樹要加的前綴
                        const string& prf_Lchild,   // 往左子樹要加的前綴
                        string buffer);
    void pri_printNode(RBnode* node, const string &buffer);
    public:
    RBnode* root;
    RBnode* NIL;
    RB_tree();
    void printTree();
    void left_rotate(RBnode* x);
    void right_rotate(RBnode* x);
    void insert_fixup(RBnode* cur);
    void insert(RBnode* cur, int v);
    void transplant(RBnode* u, RBnode* v);
    RBnode* find(int val);
    void deleteNode(int val);
    void delete_fixup(RBnode* x);

};