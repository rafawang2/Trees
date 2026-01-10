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
    bool color;   // RED / BLACK
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
                        string buffer) {          // 當前的 prefix
            if(Node->Rchild) {
                pri_printTree(Node->Rchild, "   ", "|  ", buffer+prf_Rchild);
            }
            pri_printNode(Node, buffer);
            if(Node->Lchild) {
                pri_printTree(Node->Lchild, "|  ", "   ", buffer+prf_Lchild);
            }
        }

    void pri_printNode(RBnode* node, const string &buffer) {
            if (node->color == RED) {
                cout << buffer << "+-" << "\033[31m" << node->value << "\033[0m" << "\n";
            }
            else {
                if (node->value == nil)
                    cout << buffer << "+-" << "NIL" << "\n";
                else
                    cout << buffer << "+-" << node->value << "\n";
            }
        }

    public:
    RBnode* root;
    RBnode* NIL;
    RB_tree() {
        NIL = new RBnode(nil, BLACK);
        NIL->Lchild = NIL;
        NIL->Rchild = NIL;
        NIL->parent = nullptr;
        root = NIL;
    }

    void printTree() {
        cout<<"====================\n";
        pri_printTree(root, "   ", "   ", "");
        cout<<"====================\n";
    }

    void left_rotate(RBnode* x) {
        RBnode* y = x->Rchild;
        RBnode* T2 = y->Lchild;

        // rotate
        x->Rchild = T2;
        if (T2 != NIL) {
            T2->parent = x;
        }
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->Lchild) {  // x在左
            x->parent->Lchild = y;
            y->parent = x->parent;
        }
        else {
            x->parent->Rchild = y;
            y->parent = x->parent;
        }
        y->Lchild = x;
        x->parent = y;
    }

    void right_rotate(RBnode* x) {
        RBnode* y = x->Lchild;
        RBnode* T2 = y->Rchild;
        // rotate
        x->Lchild = T2;
        if (T2 != NIL) {
            T2->parent = x;
        }
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->Lchild) {  // x在左
            x->parent->Lchild = y;
            y->parent = x->parent;
        }
        else {
            x->parent->Rchild = y;
            y->parent = x->parent;
        }
        y->Rchild = x;
        x->parent = y;
    }

    void insert_fixup(RBnode* cur) {
        cout <<"current: "<<cur->value << " , "<<cur->color<<endl;
        if (cur->parent!=nullptr)
            cout <<"parent: "<<cur->parent->value << " , "<<cur->parent->color<<endl;
        if (cur->parent == nullptr) {   // cur is root
            if (cur->color == RED) cur->color = BLACK;
            return;
        }

        if (cur->color == RED && cur->parent->color == RED) {
            RBnode* gp = cur->parent->parent;
            // case 1: both parent & uncle are RED -> change color
            if (gp != nullptr && gp->Lchild->color == RED && gp->Rchild->color == RED) {
                printTree();
                cout << "both parent & uncle are RED -> change color\n";
                gp->color = RED;
                gp->Lchild->color = BLACK;
                gp->Rchild->color = BLACK;
                cout << "fuck\n";
                insert_fixup(gp);
            }
            else if (cur == cur->parent->Lchild) {   // cur is at left
                if (cur->parent == gp->Lchild) {
                    printTree();
                    cout <<"LL: single right rotate\n";
                    gp->color = RED;
                    cur->color = RED;
                    cur->parent->color = BLACK;
                    right_rotate(gp);
                    printTree();
                }
                else if (cur->parent == gp->Rchild) {
                    printTree();
                    cout <<"RL: right rotate on node "<<cur->parent->value<<endl;
                    right_rotate(cur->parent);
                    printTree();
                    cout <<"then change color & left rotate on node "<<gp->value<<endl;
                    gp->color = RED;
                    cur->color = BLACK;
                    cur->parent->color = RED;
                    left_rotate(gp);
                    printTree();
                }
            }
            else if (cur == cur->parent->Rchild) {  // cur is at right
                if (cur->parent == gp->Lchild) {
                    printTree();
                    cout <<"LR: left rotate on node "<<cur->parent->value<<endl;
                    left_rotate(cur->parent);
                    printTree();
                    cout <<"then change color & right rotate on node "<<gp->value<<endl;
                    gp->color = RED;
                    cur->color = BLACK;
                    cur->parent->color = RED;
                    right_rotate(gp);
                    printTree();
                }
                else if (cur->parent == gp->Rchild) {
                    printTree();
                    cout <<"RR: single left rotate\n";
                    gp->color = RED;
                    cur->color = RED;
                    cur->parent->color = BLACK;
                    cout << gp->value <<endl;
                    left_rotate(gp);
                    printTree();
                }
            }
        }

    }

    void insert(RBnode* cur, int v) {
        RBnode* insertNode = new RBnode(v, RED);
        insertNode->Lchild = NIL;
        insertNode->Rchild = NIL;
        RBnode* parent = nullptr;
        while (cur != NIL) {
            parent = cur;
            if (v > cur->value) {
                cur = cur->Rchild;
            }
            else if (v <= cur->value) {
                cur = cur->Lchild;
            }
        }
        insertNode->parent = parent;

        // BST insert
        // case 1 空樹
        if (parent == nullptr) {
            insertNode->color = BLACK;
            root = insertNode;
            return;
        }
        // case 2：決定插左 or 插右
        if (v < parent->value)
            parent->Lchild = insertNode;
        else
            parent->Rchild = insertNode;

        insert_fixup(insertNode);
    }
};