#include "Btree.h"

// g++ main.cpp -o main & main
// 50 10 80 90 70 60 65 62 -1 50 10 80 90 70 60 65 62 -1
int main() {
    BTree BT = BTree(2);
    int n;
    while (cin>>n) {
        if (n==-1) {
            break;
        }
        BT.insert(n);
        cout << "Result:\n";
        BT.printTree(BT.root);
    }
    // while (cin>>n) {
    //     if (n==-1) {
    //         break;
    //     }
    //     RBT.deleteNode(n);
    //     cout << "Result:\n";
    //     RBT.printTree();
    // }
}