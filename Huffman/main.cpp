#include "Huffman.h"
// g++ main.cpp -o main & main
int main () {
    vector<pair<char,int>> datas = {
        pair<char,int>('a', 10),
        pair<char,int>('b', 50),
        pair<char,int>('c', 60),
        pair<char,int>('d', 62),
        pair<char,int>('e', 65),
        pair<char,int>('f', 70),
        pair<char,int>('g', 80),
    };
    HuffmanTree HT(datas);
    HT.BuildTree();
    HT.printTree();
    HT.decode("111110101110101000100"); // output: edbacgf
}