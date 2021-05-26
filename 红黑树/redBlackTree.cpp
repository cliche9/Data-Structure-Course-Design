#include "redBlackTree.h"
#include <map>

int main() {
    freopen("红黑树/data/a.in", "r", stdin);
    map<string, RBTree<string, string> *> treeSet;
    string key, opt, value, treeName, t1, t2;
    while (true) {
        cin >> opt;
        if (opt == "create") {
            cin >> treeName;
            if (treeSet.count(treeName))
                cout << "红黑树\"" << treeName << "\"已存在!\n";
            else {
                treeSet[treeName] = new RBTree<string, string>();
                cout << "红黑树: \"" << treeName << "\"创建成功!\n";
            }
        } else if (opt == "merge") {
            cin >> t1 >> t2;
            cout << "请输入合并生成红黑树名称: ";
            cin >> treeName;
            if (treeSet.count(t1) && treeSet.count(t2)) {
                treeSet[treeName] = RBTree<string, string>::merge(treeSet[t1], treeSet[t2]);
                cout << "红黑树\"" << t1 << "\"与\"" << t2 << "\"合并成功, 得到\"" << treeName << "\"\n";
                treeSet.erase(t1);
                treeSet.erase(t2);
            }
            else
                cout << "待合并的红黑树不存在!\n";
        } else if (opt == "insert") {
            cin >> treeName;
            cout << "请输入插入节点的[key => value]: ";
            cin >> key >> value;
            treeSet[treeName]->insert(key, value);
            cout << "插入成功!\n";
        } else if (opt == "erase") {
            cin >> treeName;
            cout << "请输入删除元素的key: ";
            cin >> key;
            treeSet[treeName]->erase(key);
            cout << "删除成功!\n";
        } else if (opt == "find") {
            cin >> treeName;
            cout << "请输入查找节点的key: ";
            cin >> key;
            RBNode<string, string> *theNode = treeSet[treeName]->find(key);
            if (theNode == nullptr)
                cout << "Not found key : " << key << endl;
            else
                cout << "[key: " << theNode->key << ", value: " << theNode->value << "]\n";
        } else if (opt == "check") {
            cin >> treeName;
            if (treeSet[treeName]->isRBTree())
                cout << "Valid RBTree.\n";
            else
                cout << "Error, RBTree is invalid.\n";
        } else if (opt == "show") {
            cin >> treeName;
            treeSet[treeName]->inOrderTraverse();
        } else if (opt == "quit") {
            break;
        } else {
            cout << "Invalid operation.\n";
            fflush(stdin);
        }
        for (auto &t : treeSet)
            t.second->visual(t.first);
    }

    return 0;
}