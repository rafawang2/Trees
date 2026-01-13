#include "RedBlack_tree.h"
// g++ main.cpp RedBlack_tree.cpp -o main & main
// 50 10 80 90 70 60 65 62 -1 50 10 80 90 70 60 65 62 -1
int main() {
    RB_tree RBT = RB_tree();
    int n;
    while (cin>>n) {
        if (n==-1) {
            break;
        }
        RBT.insert(RBT.root, n);
        cout << "Result:\n";
        RBT.printTree();
    }
    while (cin>>n) {
        if (n==-1) {
            break;
        }
        RBT.deleteNode(n);
        cout << "Result:\n";
        RBT.printTree();
    }
}