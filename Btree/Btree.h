#pragma once
#include <iostream>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

struct Bnode {
    vector<int> keys;
    vector<Bnode*> children;
    Bnode() :
                keys({}),
                children({}) {}
};

class BTree {
    public:
    Bnode* root;
    int t;  // 最小度數
    int max_key;

    BTree(int order) {
        t = order;
        max_key = 2*t - 1;
        root = nullptr;
    }

    void printNode(Bnode* node) {
        cout << "(";
        int n = node->keys.size()-1;
        for (int i=0; i<n; ++i) {
            cout << node->keys[i] <<", ";
        }
        cout <<node->keys[n];
        cout << ")\n";
    }

    void printTree(Bnode* cur) {
        // Since B-Tree is full tree, so printNode in BFS order
        if (cur==nullptr)
            return;
        printNode(cur);
        for (auto& n : cur->children) {
            printTree(n);
        }
    }

    Bnode* Search(Bnode* cur, int val) {
        int i = 0;

        // 找第一個 >= val
        while (i < cur->keys.size() && cur->keys[i] < val) {
            i++;
        }

        // 找到
        if (i < cur->keys.size() && cur->keys[i] == val) {
            return cur;
        }

        // 是 leaf，找不到
        if (cur->children.empty()) {
            return nullptr;
        }

        // 往子樹走
        return Search(cur->children[i], val);
    }

    void splitchild(Bnode* parent, int child_idx) {
        Bnode* child = parent->children[child_idx];
        Bnode* new_child = new Bnode();
        int mid = t-1;
        // 1. parent 中間插入key
        parent->keys.insert(parent->keys.begin()+child_idx, child->keys[mid]);

        // 2. new_child 取得 右半的child keys
        new_child->keys.assign(child->keys.begin() + mid + 1, child->keys.end());

        // 3. child 只留左半
        child->keys.resize(mid);

        // 4. 處理child的children
        if (!child->children.empty()) {
            new_child->children.assign(child->children.begin() + mid + 1, child->children.end());
            child->children.resize(mid + 1);
        }

        // 5. parent的children插入new_child
        parent->children.insert(parent->children.begin()+child_idx+1, new_child);
    }

    void insertNonFull(Bnode* node, int val) {
        int i = node->keys.size() - 1;

        // case: node is leaf
        if (node->children.empty()) {
            // 先撐一格，找到適當的插入位置
            node->keys.push_back(0);
            while (i >= 0 && val < node->keys[i]) {
                node->keys[i+1] = node->keys[i];
                i--;
            }
            node->keys[i+1] = val;
        }
        // case: internal node
        else {
            // 找val在哪個區間
            while (i >= 0 && val < node->keys[i]) {
                i--;
            }
            i++;

            // 如果 child 滿，先 split
            if (node->children[i]->keys.size() == max_key) {
                splitchild(node, i);

                if (val > node->keys[i]) {
                    i++;
                }
            }

            insertNonFull(node->children[i], val);
        }
    }


    void insert(int val) {
        // Empty Tree
        if (root == nullptr) {
            root = new Bnode();
            root->keys.push_back(val);
            return;
        }

        // root is full => split root
        if (root->keys.size() == max_key) {
            Bnode* newRoot = new Bnode();
            newRoot->children.push_back(root);
            splitchild(newRoot, 0);
            root = newRoot;
        }

        insertNonFull(root, val);
    }

};