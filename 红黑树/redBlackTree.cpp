#include "redBlackTree.h"

int main() {
    RBTree<string, int> t1;
    string key;
    int value;
    while (cin >> key >> value) {
        if (key == "eof")
            break;
        t1.insert(key, value);
    }
    t1.inOrderTraverse();

    return 0;
}