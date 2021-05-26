#include "redBlackTree.h"
#include <cstdlib>

int main() {
    // freopen("a.in", "r", stdin);
    RBTree<string, string> t1;
    string key, opt, value;
    while (true) {
        cin >> opt;
        if (opt == "insert") {
            cin >> key >> value;
            t1.insert(key, value);
            cout << "Compare count of insert: " << t1.numberOfComparasion()
                 << "\nRotate count of insert: " << t1.numberOfRotation() << endl; 
        } else if (opt == "erase") {
            cin >> key;
            t1.erase(key);
            cout << "Compare count of erase: " << t1.numberOfComparasion()
                 << "\nRotate count of erase: " << t1.numberOfRotation() << endl; 
        } else if (opt == "find") {
            cin >> key;
            RBNode<string, string> *theNode = t1.find(key);
            if (theNode == nullptr)
                cout << "Not found key : " << key << endl;
            else
                cout << "[key: " << theNode->key << ", value: " << theNode->value << "]"
                     << "\nCompare count of find: " << t1.numberOfComparasion() << endl;
        } else if (opt == "check") {
            if (t1.isRBTree())
                cout << "Valid RBTree.\n";
            else
                cout << "Error, RBTree is invalid.\n";
        } else if (opt == "show") {
            t1.inOrderTraverse();
        } else if (opt == "visual") {
            t1.visual();
            system("dot -Tjpg 红黑树/data/1.dot -o 红黑树/data/1.jpg");
        } else if (opt == "quit") {
            break;
        } else 
            cout << "Invalid operation.\n";
    }

    return 0;
}