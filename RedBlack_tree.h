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
        root = NIL;
    }

    void printTree() {
        cout<<"====================\n";
        pri_printTree(root, "   ", "   ", "");
        cout<<"====================\n";
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

    }
};