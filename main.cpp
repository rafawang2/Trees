#include "RedBlack_tree.h"

// 50 10 80 90 70 60
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
}