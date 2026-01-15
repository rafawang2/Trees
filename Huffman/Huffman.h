#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;
using std::sort;
using std::queue;

struct Huffnode {
    Huffnode* Lchild;
    Huffnode* Rchild;
    char word;
    int freq;
    string code;
    Huffnode(char w, int f): word(w), freq(f), Lchild(nullptr), Rchild(nullptr), code("") {}
};

class HuffmanTree {
    private:
    void pri_printTree(Huffnode* Node,
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

    void pri_printNode(Huffnode* node, const string &buffer) {
        cout << buffer << "+-(" <<node->word<<", "<< node->freq << ")\n";
    }

    public:
    Huffnode* root;
    vector<Huffnode*> Nodes;

    void printTree() {
        cout<<"====================\n";
        pri_printTree(root, "   ", "   ", "");
        cout<<"====================\n";
    }

    HuffmanTree(const vector<pair<char,int>>& datas) {
        Nodes = {};
        for (auto& d : datas) {
            Nodes.push_back(new Huffnode(d.first, d.second));
        }
        sort(Nodes.begin(), Nodes.end(), comp);
    }

    static bool comp(Huffnode* a, Huffnode* b) {    // freq由大到小排好
        return a->freq > b->freq;
    }
    Huffnode* chooseMin() {
        Huffnode* minNode = Nodes.back();
        Nodes.pop_back();
        return minNode;
    }
    void BuildTree() {
        while (Nodes.size()>1) {
            Huffnode* node1 = chooseMin();
            Huffnode* node2 = chooseMin();
            Huffnode* parent = new Huffnode('\0', node1->freq+node2->freq);
            parent->Lchild = node1;
            parent->Rchild = node2;
            Nodes.push_back(parent);
            sort(Nodes.begin(), Nodes.end(), comp);
        }
        root = Nodes.front();   // 剩下最後的當root
        encode(root, "");
    }

    void encode(Huffnode* cur, string code) {
        if (cur == nullptr)
            return;
        if (cur->Rchild)
            encode(cur->Rchild, code + "1");
        cur->code = code;
        if (cur->Lchild)
            encode(cur->Lchild, code + "0");
    }

    void decode(const string& code) {
        Huffnode* cur = root;
        for (const char& c : code) {
            if (c == '0' && cur->Lchild) {
                cur = cur->Lchild;
            }
            else if (c == '1' && cur->Rchild) {
                cur = cur->Rchild;
            }
            else {
                cout << "Wrong code format\n";
                return;
            }
        }
        cout << code <<" -> " <<cur->word<<endl;
    }

    vector<Huffnode*> BFS() {
        vector<Huffnode*> sequence = {};
        queue<Huffnode*> Q  = {};
        Q.push(root);
        while (!Q.empty()) {
            Huffnode* cur = Q.front();
            Q.pop();
            sequence.push_back(cur);
            if (cur->Lchild)
                Q.push(cur->Lchild);
            if (cur->Rchild)
                Q.push(cur->Rchild);
        }
        return sequence;
    }
};
