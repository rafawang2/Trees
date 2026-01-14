#pragma once
#include <iostream>
#include <vector>
#include <algorithm> //for std::find
using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::find;

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
    int min_key;    // 最少key數
    int max_key;    // 最多key數

    BTree(int order) {
        t = order;
        min_key = t - 1;
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

    void printTree(Bnode* cur, int level) {
        // Since B-Tree is full tree, so printNode in BFS order
        if (cur==nullptr)
            return;
        cout << "level: "<<level<<" ";
        printNode(cur);
        level+=1;
        for (auto& n : cur->children) {
            printTree(n,level);
        }
    }

    pair<Bnode*, int> Search(Bnode* cur, int val) {
        int i = 0;

        // 找第一個 >= val
        while (i < cur->keys.size() && cur->keys[i] < val) {
            i++;
        }

        // 找到
        if (i < cur->keys.size() && cur->keys[i] == val) {
            return pair<Bnode*, int>(cur, i);
        }

        // 是 leaf，找不到
        if (cur->children.empty()) {
            return pair<Bnode*, int>(nullptr, -1);
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

    // 找第一個>=key的idx
    int findKey(Bnode* node, int key) {
        int idx = 0;
        while (idx < node->keys.size() && node->keys[idx] < key) {
            idx++;
        }
        return idx;
    }

    void merge(Bnode* parent, int idx) {
        int k = parent->keys[idx];
        Bnode* leftchild = parent->children[idx];
        Bnode* rightchild = parent->children[idx+1];
        // 1. 把 parent 的 key 下放到 left
        leftchild->keys.push_back(k);
        // 2. 把 right 的所有 keys 接到 left 後面
        for (int rk : rightchild->keys)
            leftchild->keys.push_back(rk);
        // 3. 如果right不是leaf，把 right 的 children 也接過來
        if (!rightchild->children.empty()) {
            for (Bnode* c : rightchild->children)
                leftchild->children.push_back(c);
        }
        // 4. 從parent刪掉key
        parent->keys.erase(parent->keys.begin() + idx);
        // 5. 從 parent 刪掉 right child 指標
        parent->children.erase(parent->children.begin() + idx+1);
        delete rightchild;
        rightchild = nullptr;
    }

    void fill(Bnode* parent, int idx) {
        Bnode* cur = parent->children[idx];
        // case 1: 左兄弟存在且key數>=t
        if (idx>0 && parent->children[idx-1]->keys.size() >= t) {
            Bnode* leftsib = parent->children[idx-1];
            // Borrow from left
            // 1. parent[idx-1] 插在cur的最前面
            cur->keys.insert(cur->keys.begin(), parent->keys[idx-1]);
            // 2. parent 的 key 換成 left 最右邊的
            parent->keys[idx-1] = leftsib->keys.back();
            leftsib->keys.pop_back();
            // 3. 如果有 children，也要一起搬
            if (!leftsib->children.empty()) {
                cur->children.insert(cur->children.begin(),
                                        leftsib->children.back());
                leftsib->children.pop_back();
            }
        }
        // case 2: 右兄弟存在且key數>=t
        else if (idx<parent->children.size()-1 && parent->children[idx+1]->keys.size() >= t) {
            Bnode* rightsib = parent->children[idx+1];
            // Borrow from right
            // 1. parent[idx]插在cur最後面
            cur->keys.push_back(parent->keys[idx]);
            // 2. parent 的 key 換成 right 最左邊的
            parent->keys[idx] = rightsib->keys[0];
            rightsib->keys.erase(rightsib->keys.begin());
            // 3. 如果有 children，也要一起搬
            if (!rightsib->children.empty()) {
                cur->children.push_back(rightsib->children[0]);
                rightsib->children.erase(rightsib->children.begin());
            }
        }
        // case 3: 左右兄弟key數都<t -> merge
        else {
            if (idx < parent->children.size()-1) {
                merge(parent, idx);
            } else {
                merge(parent, idx-1);
            }
        }
    }

    // 往下前先補，永遠不走進 key 數 < t 的節點
    void deleteNode(Bnode* cur, int val) {
        int idx = findKey(cur, val);

        if (idx < cur->keys.size() && cur->keys[idx] == val) {
            // case 1: val is at leaf node
            if (cur->children.empty() && cur->keys.size() > min_key) {
                // cur's keys num > min_key => 直接刪
                cur->keys.erase(cur->keys.begin() + idx);
            }
            // case 2: cur is an internal node
            else {
                Bnode* leftchild = cur->children[idx];
                Bnode* rightchild = cur->children[idx+1];
                // case 2-a: left child has t keys (左子的key數比最小還多1)
                if (leftchild->keys.size() >= t) {
                    int predecesser = leftchild->keys[leftchild->keys.size()-1]; // 左子最大的
                    // 1. 把這個前繼者往上拉到要被刪除的key
                    cur->keys[idx] = predecesser;
                    // 2. 遞迴把前繼者原本的位置刪掉
                    deleteNode(leftchild, predecesser);
                }
                // case 2-b: 2-a但變右邊
                else if (rightchild->keys.size() >= t) {
                    int successor = rightchild->keys[0]; // 右子最小的
                    cur->keys[idx] = successor;
                    deleteNode(rightchild, successor);
                }
                // case 2-c: 左右兩子的key都不夠刪 => merge
                else if (leftchild->keys.size() == min_key && rightchild->keys.size() == min_key) {
                    merge(cur, idx);
                    deleteNode(cur->children[idx], val);
                }
            }
        }
        // case 3: val is not at this level => go deep
        else {
            if (cur->children.empty()) {
                cout << "Key not found\n";
                return;
            }

            // 若node沒有t個key，需要補
            if (cur->children[idx]->keys.size() < t)
                fill(cur, idx);

            if (idx > cur->keys.size())
                deleteNode(cur->children[idx - 1], val);
            else
                deleteNode(cur->children[idx], val);
            }
        }

    void remove(int val) {
        if (root == nullptr) {
            cout << "Tree is empty!\n";
            return;
        }

        deleteNode(root, val);

        // 縮高
        if (root->keys.size() == 0) {
            if (!root->children.empty())
                root = root->children[0];
        }
    }
};