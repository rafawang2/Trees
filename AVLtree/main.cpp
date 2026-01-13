#include "AVLtree.h"
// g++ main.cpp -o main & main
// 50 10 80 90 70 60 65 62
int main() {
    AVL avl = AVL();
    int n;
    while (cin>>n) {
        if (n==-1) {
            break;
        }
        avl.root = avl.insert(avl.root, n);
        cout << "Result:\n";
        avl.printTree();
    }
    while (cin>>n) {
        if (n==-1) {
            break;
        }
        avl.root = avl.deleteNode(avl.root, n);
        cout << "Result:\n";
        avl.printTree();
    }
}