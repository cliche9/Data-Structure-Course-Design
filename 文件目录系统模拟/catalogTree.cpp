#include "catalogTree.h"
#include <iomanip>
#include <stack>

catalogTree::catalogTree(const string &rootName) {
    root = new logNode(rootName, nullptr, 0);
    deepestDir = root;
}

catalogTree::~catalogTree() {
    delete root;
}

void catalogTree::dir() const {
    logNode *curNode = deepestDir->child;
    int count = 0;
    while (curNode != nullptr && ++count) {
        // 规定输出格式
        cout << setw(20) << curNode->fileName;
        if (count == 3) {
            cout << endl;
            count = 0;
        }
        curNode = curNode->slibing;
    }
    // 每次操作完成都需要重新输出当前目录的绝对路径
    cd();
}

void catalogTree::cd() const {
    stack<string> path;
    logNode *curNode = deepestDir;
    // 自底向上遍历到根, 存储路径名称
    while (curNode != nullptr) {
        path.push(curNode->fileName);
        curNode = curNode->parent;
    }
    // 自顶向下输出路径名称, 直到栈空
    while (!path.empty()) {
        cout << path.top() << '/';
        path.pop();
    }
}

void catalogTree::cdStr(const string &targetPath) {
    
}