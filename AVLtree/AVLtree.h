#pragma once
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

const int nil = -99999;

struct AVLnode {
    int value;
    int height; // longest path from self to leaf
    int BF = 0; // blance factor = self.left.height - self.right.height
    AVLnode* Lchild;
    AVLnode* Rchild;
    AVLnode(int v) : value(v), height(1),
                    Lchild(nullptr),
                    Rchild(nullptr) {}
};

class AVL {
    private:
    void pri_printTree(AVLnode* Node,
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

    void pri_printNode(AVLnode* node, const string &buffer) {
        cout << buffer << "+-" << node->value<<"("<<node->height<<", "<<node->BF<<")" << "\n";
    }

    public:
    AVLnode* root;

    void printTree() {
        cout<<"====================\n";
        pri_printTree(root, "   ", "   ", "");
        cout<<"====================\n";
    }

    AVL() {
        root = nullptr;
    }

    int GetHeight(AVLnode* node) {
        if (node == nullptr) {
            return 0;
        }
        else
            return node->height;
    }

    int GetBF(AVLnode* node) {
        if (node == nullptr) {
            return 0;
        }
        else {
            return GetHeight(node->Lchild) - GetHeight(node->Rchild);
        }
    }

    void SetHeightAndBF(AVLnode* node) {
        if (node != nullptr) {
            node->height = 1 + std::max(GetHeight(node->Lchild), GetHeight(node->Rchild));
            node->BF = GetBF(node);
        }
    }

    AVLnode* search(int val) {
        AVLnode* cur = root;
        while (cur != nullptr) {
            if (val > cur->value) {
                cur = cur->Rchild;
            }
            else if (val < cur->value) {
                cur = cur->Lchild;
            }
            else {
                return cur;
            }
        }
        cout << "node "<<val<<" is not exist.\n";
        return cur;
    }

    AVLnode* LeftRotate(AVLnode* node) {
        AVLnode* A = node;
        AVLnode* B = node->Rchild;
        AVLnode* Y = B->Lchild;

        // Rotate
        A->Rchild = Y;
        B->Lchild = A;

        // Set height
        SetHeightAndBF(A);
        SetHeightAndBF(B);
        return B;
    }

    AVLnode* RightRotate(AVLnode* node) {
        AVLnode* A = node;
        AVLnode* B = node->Lchild;
        AVLnode* Y = B->Rchild;

        // Rotate
        A->Lchild = Y;
        B->Rchild = A;

        // Set height
        SetHeightAndBF(A);
        SetHeightAndBF(B);
        return B;
    }


    AVLnode* insert(AVLnode* cur, int val) {
        if (cur == nullptr) {
            cur = new AVLnode(val);
            cur->height = 1;
            return cur;
        }
        if (val > cur->value) {
            cur->Rchild = insert(cur->Rchild, val);
        }
        else if (val < cur->value) {
            cur->Lchild = insert(cur->Lchild, val);
        }
        else
            return cur;
        SetHeightAndBF(cur);

        if (cur->BF > 1) {  // cur的左子樹高於右子樹
            if (cur->Lchild->BF >= 0) {
                cout << "LL\n";
                return RightRotate(cur);
            }
            else {
                cout << "LR\n";
                cur->Lchild = LeftRotate(cur->Lchild);  // turn LR to LL
                return RightRotate(cur);
            }
        }
        else if (cur->BF < -1) { // cur的左子樹低於右子樹
            if (cur->Rchild->BF < 0) {
                cout << "RR\n";
                return LeftRotate(cur);
            }
            else {
                cout << "RL\n";
                cur->Rchild = RightRotate(cur->Rchild);  // turn RL to RR
                return LeftRotate(cur);
            }
        }
        return cur;
    }

    AVLnode* findMinNode(AVLnode* cur) {
        if (cur->Lchild == nullptr) {
            return cur;
        }
        else
            return findMinNode(cur->Lchild);
    }

    AVLnode* deleteNode(AVLnode* cur, int val) {
        if (cur == nullptr) {
            return nullptr;
        }
        if (val > cur->value) {
            cur->Rchild = deleteNode(cur->Rchild, val);
        }
        else if (val < cur->value) {
            cur->Lchild = deleteNode(cur->Lchild, val);
        }
        else {  // found node val
            if (cur->Lchild == nullptr || cur->Rchild == nullptr) {
                AVLnode* temp;
                if (cur->Lchild != nullptr) {
                    temp = cur->Lchild;
                }
                else {
                    temp = cur->Rchild;
                }
                if (temp == nullptr) {  // temp is leaf
                    delete cur;
                    return nullptr;
                }
                else {
                    *cur = *temp;   // 用temp取代cur
                    delete temp;
                }
            }
            else {  // 2 children
                AVLnode* temp = findMinNode(cur->Rchild);
                cur->value = temp->value;
                cur->Rchild = deleteNode(cur->Rchild, temp->value);
            }
        }
        if (cur == nullptr) {
            return cur;
        }
        SetHeightAndBF(cur);
        if (cur->BF > 1) {  // cur的左子樹高於右子樹
            if (GetBF(cur->Lchild) >= 0) {
                cout << "LL\n";
                return RightRotate(cur);
            }
            else {
                cout << "LR\n";
                cur->Lchild = LeftRotate(cur->Lchild);  // turn LR to LL
                return RightRotate(cur);
            }
        }
        else if (cur->BF < -1) { // cur的左子樹低於右子樹
            if (GetBF(cur->Rchild) < 0) {
                cout << "RR\n";
                return LeftRotate(cur);
            }
            else {
                cout << "RL\n";
                cur->Rchild = RightRotate(cur->Rchild);  // turn RL to RR
                return LeftRotate(cur);
            }
        }
        return cur;
    }
};