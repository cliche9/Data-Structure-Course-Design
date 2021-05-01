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
    // 每次操作完成都需要重新输出当前目录的绝对路径?
    // 该输出相对路径还是绝对路径呢?
    display();
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
    bool legalPath = true;
    // 拆分地址成string
    vector<string> path = split(targetPath);
    // 绝对路径
    vector<string>::iterator iter = path.begin(); 
    logNode *curNode = nullptr;
    if (*iter == "/") {
        ++iter;
        // 绝对路径
        curNode = root;
    }
    else 
        // 相对路径
        curNode = deepestDir;
    // 寻找路径对应目录
    while (iter != path.end()) {
        // 进入下一层目录进行判断
        curNode = curNode->child;
        while (curNode != nullptr)
            if (curNode->fileName == *iter)
                break;
        // 无对应目录或文件
        if (curNode == nullptr) {
            legalPath = false;
            break;
        }
        // 当前目录有对应子目录
        // 进入下一层目录
        ++iter;
    }
    // 地址不合法
    if (!legalPath) {
        cout << "cd: no such file or directory: " << targetPath << endl;
        return;
    }
    // 地址合法, 更新当前目录
    deepestDir = curNode;
    // 输出当前目录
    display();
}

void catalogTree::mkdir(const string &dirName) {

}

void catalogTree::mkfile(const string &fileName) {
    
}

vector<string> catalogTree::split(const string &thePath, char tag) const {
    vector<string> res;
    string temp;

    for (int i = 0; i < thePath.length(); i++) {
        // 每遇到一个'/', 就拆分一部分地址
        if (thePath[i] == tag) {
            if (temp.empty())
                // 说明是绝对路径, push一个根目录
                res.push_back("/");
            else {
                res.push_back(temp);
                temp.clear();
            }
        }
    }

    return res;
}

void catalogTree::display() const {
    cout << deepestDir->fileName << " % ";
}