#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;
using std::queue;

struct Huffnode {
    Huffnode* Lchild;
    Huffnode* Rchild;
    char word;
    int freq;
    string code;
    Huffnode(char w, int f): word(w), freq(f), Lchild(nullptr), Rchild(nullptr), code("") {}
};

void heapify(vector<Huffnode*>& nodes, int parent) { // min-heap
    int Lchild = parent*2 + 1;
    int Rchild = parent*2 + 2;
    int min_idx = parent;
    if (Lchild < nodes.size() && nodes[Lchild]->freq < nodes[min_idx]->freq) {
        min_idx = Lchild;
    }
    if (Rchild < nodes.size() && nodes[Rchild]->freq < nodes[min_idx]->freq) {
        min_idx = Rchild;
    }

    if (min_idx != parent) {
        std::swap(nodes[parent], nodes[min_idx]);
        heapify(nodes, min_idx);
    }
}

void heapify_up(vector<Huffnode*>& nodes, int idx) {
    if (idx == 0) return;
    int parent = idx/2;
    if (nodes[parent]->freq > nodes[idx]->freq) {
        std::swap(nodes[parent], nodes[idx]);
        heapify_up(nodes, parent);
    }
}

void build_heap(vector<Huffnode*>& nodes) {
    int n = nodes.size();
    for (int parent = (n-1)/2; parent>=0; --parent) {
        heapify(nodes, parent);
    }
}
Huffnode* ExtractMinNode(vector<Huffnode*>& nodes) {
    Huffnode* minNode = nodes[0];
    nodes[0] = nodes.back();
    nodes.pop_back();
    heapify(nodes, 0);
    return minNode;
}

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
        if (node->word == '\0') {
            cout << buffer << "+-(" << node->freq << ")\n";
        }
        else {
            cout << buffer << "\033[31m+-(" <<node->word<<", "<< node->freq<<", "<<node->code << ")\033[0m\n";
        }
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
        build_heap(Nodes);
    }

    void BuildTree() {
        while (Nodes.size()>1) {
            Huffnode* node1 = ExtractMinNode(Nodes);
            Huffnode* node2 = ExtractMinNode(Nodes);
            Huffnode* parent = new Huffnode('\0', node1->freq+node2->freq);
            parent->Lchild = node1;
            parent->Rchild = node2;
            Nodes.push_back(parent);
            heapify_up(Nodes, Nodes.size()-1);
        }
        root = Nodes.front();   // 剩下最後的當root
        encode(root, "");
    }

    void encode(Huffnode* cur, string code) {
        if (cur == nullptr)
            return;
        if (cur->Lchild == nullptr && cur->Rchild == nullptr) { // encode at leaf node
            cur->code = code;
        }
        encode(cur->Lchild, code + "0");
        encode(cur->Rchild, code + "1");
    }

    void decode(const string& bits) {   // 解一整串密文
        Huffnode* cur = root;
        for (const char& c : bits) {
            if (cur->Lchild && c == '0') {
                cur = cur->Lchild;
            }
            else if (cur->Rchild && c == '1') {
                cur = cur->Rchild;
            }
            else {
                cout << "decode failed.\n";
            }
            if (cur->Lchild == nullptr && cur->Rchild == nullptr) {
                cout << cur->word;
                cur = root;
            }
        }
        cout << endl;
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
