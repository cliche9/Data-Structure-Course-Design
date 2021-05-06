#include "catalogTree.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include <stack>

catalogTree::catalogTree() {
    root = new logNode("/", nullptr, 0);
    currentDir = root;
}

catalogTree::~catalogTree() {
    delete root;
}

void catalogTree::ls() const {
    logNode *curNode = currentDir->child;
    int count = 0;
    while (curNode != nullptr && ++count) {
        // 规定输出格式
        cout << std::left << setw(30) << curNode->fileName;
        if (count == 3) {
            cout << endl;
            count = 0;
        }
        curNode = curNode->slibing;
    }
    if (count > 0 && count < 3)
        cout << endl;
    // 每次操作完成都需要重新输出当前目录的绝对路径?
    // 该输出相对路径还是绝对路径呢?
}

void catalogTree::dir() const {
    vector<string> res;
    string temp;
    logNode *curNode = currentDir->child;
    // res使用sort排序一下, 实现字典序有序
    while (curNode != nullptr) {
        temp = curNode->fileName;
        if (curNode->dirOrfile)
            temp += "*";
        res.push_back(temp);
        curNode = curNode->slibing;
    }
    sort(res.begin(), res.end(), cmp);
    // 输出结果
    /*
    测试数据 - 删除count部分
    */
    int count = 0;
    for (vector<string>::iterator iter = res.begin(); iter != res.end() && ++count; ++iter) {
        cout << std::left << setw(20) << *iter;
        if (count == 3) {
            cout << endl;
            count = 0;
        }
    }
    if (count > 0 && count < 3)
        cout << endl;
}

void catalogTree::cd() const {
    stack<string> path;
    logNode *curNode = currentDir;
    // 自底向上遍历到根, 存储路径名称
    while (curNode->parent != nullptr) {
        path.push(curNode->fileName);
        curNode = curNode->parent;
    }
    // 只有根目录
    if (path.empty()) {
        cout << '/' << endl;
        return;
    }
    // 自顶向下输出路径名称, 直到栈空
    while (!path.empty()) {
        cout << '/' << path.top();
        path.pop();
    }
    cout << endl;
}

void catalogTree::cdStr(const string &targetPath) {
    // 返回上一层目录
    if (targetPath == "..") {
        if (currentDir != root)
            // 只有当前不是根目录才返回
            currentDir = currentDir->parent;
        return;
    }
    // 进入目录
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
        while (curNode != nullptr) {
            if (curNode->fileName == *iter && !curNode->dirOrfile)
                break;
            curNode = curNode->slibing;
        }
        // 无对应目录
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
    logNode *preNode = currentDir;
    logNode *curNode = currentDir->child;
    // 寻找fileName名称的文件/目录
    while (curNode != nullptr) {
        if (curNode->fileName == fileName)
            break;
        preNode = curNode;
        curNode = curNode->slibing;
    }
    // 不存在, 直接返回
    if (curNode == nullptr)
        return;
    // 存在, 删除
    if (preNode->child == curNode)
        preNode->child = curNode->slibing;
    else
        preNode->slibing = curNode->slibing;
    delete curNode;
    currentDir->childSize--;
}

void catalogTree::save(const string &targetPath) {
    outfile.open(targetPath);
    subSave(root);
    outfile.close();
}

void catalogTree::load(const string &sourcePath) {
    // 删除原有的树
    delete root;
    root = currentDir = nullptr;
    // 读取文件建立新树
    infile.open(sourcePath);
    subLoad(root, -1);
    infile.close();
    currentDir = root;
}

void catalogTree::quit() const {
    exit(1);
}

void catalogTree::execute() {
    display();
    string oneLineOpt;
    while (true) {
        fflush(stdin);
        getline(cin, oneLineOpt);
        stringstream ss(oneLineOpt);
        vector<string> opts;
        while (ss >> oneLineOpt)
            opts.push_back(oneLineOpt);
        switch (opts.size()) {
            case 1: {
                if (oneLineOpt == "dir" || oneLineOpt == "ls")
                    dir();
                else if (oneLineOpt == "cd")
                    cd();
                else if (oneLineOpt == "quit")
                    quit();
                else 
                    cout << "Invalid operation!\n";
                break;
            }
            case 2: {
                if (opts[0] == "cd")
                    cdStr(opts[1]);
                else if (opts[0] == "mkdir")
                    mkdir(opts[1]);
                else if (opts[0] == "mkfile")
                    mkfile(opts[1]);
                else if (opts[0] == "delete")
                    erase(opts[1]);
                else if (opts[0] == "save")
                    save(opts[1]);
                else if (opts[0] == "load")
                    load(opts[1]);
                else
                    cout << "Invalid operation!\n";
                break;
            }
            default:
                cout << "Invalid operation!\n";
        }
        display();
    }
}

vector<string> catalogTree::split(const string &thePath, char tag) const {
    vector<string> res;
    string temp;
    if (thePath[0] == '/')
        res.push_back("/");

    for (int i = 0; i < thePath.length(); i++) {
        // 每遇到一个'/', 就拆分一部分地址
        if (thePath[i] == tag) {
            if (!temp.empty()) {
                res.push_back(temp);
                temp.clear();
            }
        }
        else
            temp += thePath[i];
    }
    if (!temp.empty())
        res.push_back(temp);

    return res;
}

void catalogTree::subSave(logNode *root) {
    // 递归保存
    if (root == nullptr)
        return;
    // 保存一个根节点
    outfile << root->fileName << ' ' << root->dirOrfile << ' ' << root->childSize << endl;
    logNode *curNode = root->child;
    while (curNode != nullptr) {
        subSave(curNode);
        curNode = curNode->slibing;
    }
}

void catalogTree::subLoad(logNode *parent, int nodeCount) {
    // 待接收参数
    string fileName;
    bool dirOrfile;
    int childSize;
    // 根节点
    if (nodeCount == -1) {
        infile >> fileName >> dirOrfile >> childSize;
        root = new logNode(fileName, nullptr, dirOrfile, childSize);
        subLoad(root, childSize);
    }
    else {
        // 其他节点
        if (nodeCount) {
            // 对child特别处理
            infile >> fileName >> dirOrfile >> childSize;
            parent->child = new logNode(fileName, parent, dirOrfile, childSize);
            subLoad(parent->child, childSize);
        }
        logNode *curNode = parent->child;
        // 处理slibling
        for (int i = 1; i < nodeCount; i++) {
            infile >> fileName >> dirOrfile >> childSize;
            curNode->slibing = new logNode(fileName, parent, dirOrfile, childSize);
            subLoad(curNode->slibing, childSize);
            curNode = curNode->slibing;
        }
    }
}

void catalogTree::display() const {
    cout << "abc_mac@macBook-Pro " << currentDir->fileName << " % ";
}

bool catalogTree::cmp(const string &s1, const string &s2) {
    // 同类比较
    int l1 = s1.length(), l2 = s2.length();
    if ((s1[l1 - 1] == '*' && s2[l2 - 1] == '*') || (s1[l1 - 1] != '*' && s2[l2 - 1] != '*'))
        return s1 < s2;
    else
        // 具体文件优先级高
        return s1[l1 - 1] == '*';
}