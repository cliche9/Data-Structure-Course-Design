#include "catalogTree.h"
#include <iomanip>
#include <fstream>
#include <stack>

catalogTree::catalogTree(const string &rootName) {
    root = new logNode(rootName, nullptr, 0);
    currentDir = root;
}

catalogTree::~catalogTree() {
    delete root;
}

void catalogTree::dir() const {
    logNode *curNode = currentDir->child;
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
    logNode *curNode = currentDir;
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
        curNode = currentDir;
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
    currentDir = curNode;
    // 输出当前目录
    display();
}

void catalogTree::mkdir(const string &dirName) {
    // 当前目录下没有目录/孩子
    if (currentDir->child == nullptr) {
        currentDir->child = new logNode(dirName, currentDir, 0);
        currentDir->childSize++;
        return;
    }
    // 当前目录下有目录/孩子
    logNode *curNode = currentDir->child;
    while (curNode->slibing != nullptr) {
        // 当前已有dirName的目录, 直接返回
        if (curNode->fileName == dirName)
            return;
        curNode = curNode->slibing;
    }
    // 不存在dirName的目录, 新建兄弟节点
    curNode->slibing = new logNode(dirName, currentDir, 0);
    currentDir->childSize++;
}

void catalogTree::mkfile(const string &fileName) {
    // 当前目录下没有文件/孩子
    if (currentDir->child == nullptr) {
        currentDir->child = new logNode(fileName, currentDir, 1);
        currentDir->childSize++;
        return;
    }
    // 当前目录下有文件/孩子
    logNode *curNode = currentDir->child;
    while (curNode->slibing != nullptr) {
        // 当前已有fileName的文件, 直接返回
        if (curNode->fileName == fileName)
            return;
        curNode = curNode->slibing;
    }
    // 不存在fileName的文件, 新建兄弟节点
    curNode->slibing = new logNode(fileName, currentDir, 1);
    currentDir->childSize++;
}

void catalogTree::erase(const string &fileName) {
    logNode *curNode = currentDir->child;
    // 寻找fileName名称的文件/目录
    while (curNode != nullptr) {
        if (curNode->fileName == fileName)
            break;
        curNode = curNode->slibing;
    }
    // 不存在, 直接返回
    if (curNode == nullptr)
        return;
    // 存在, 删除
    delete curNode;
    currentDir->childSize--;
}

void catalogTree::save(const string &targetPath) {
    // 设置输出路径
    ofstream outfile(targetPath);
    // 保存一个根节点
    outfile << root->fileName << ' ' << root->dirOrfile << ' ' << root->childSize << endl;
    // 循环 - 一层一层保存所有节点
    for (logNode *curNode = root; curNode != nullptr; curNode = curNode->child)
        for (logNode *temp = curNode->child; temp != nullptr; temp = temp->slibing)
            outfile << temp->fileName << ' ' << temp->dirOrfile << ' ' << temp->childSize << endl;
    // 后续复原时可以根据部分复原的树决定读取的节点个数
}

void catalogTree::load(const string &sourcePath) {
    // 删除原有的树
    delete root;
    root = currentDir = nullptr;
    // 读取文件建立新树
    ifstream infile(sourcePath);
    string fileName;
    bool dirOrfile;
    int childSize;
    // 读取根节点
    infile >> fileName >> dirOrfile >> childSize;
    root = new logNode(fileName, nullptr, dirOrfile, childSize);
    logNode *curNode = root;
    while (!infile.eof()) {
        // 循环, 直到文件读取完
        if (curNode->childSize == 0)
            // 对于没有子文件的节点, 直接跳过
            continue;
        infile >> fileName >> dirOrfile >> childSize;
        // 处理有子文件的节点
        curNode->child = new logNode(fileName, curNode, dirOrfile, childSize);
        logNode *temp = curNode->child;
        // 循环, 看是否有兄弟节点
        for (int i = 1; i < curNode->childSize; i++) {
            infile >> fileName >> dirOrfile >> childSize;
            temp->slibing = new logNode(fileName, curNode, dirOrfile, childSize);
            temp = temp->slibing;
        }
        // 进入下一层
        curNode = curNode->child;
    }
}

void catalogTree::quit() const {
    exit(1);
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
    cout << currentDir->fileName << " % ";
}