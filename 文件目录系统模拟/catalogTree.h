#ifndef _CATALOG_TREE_H_
#define _CATALOG_TREE_H_
#include "../exception.h"
#include <fstream>
#include <vector>

struct logNode {
    // 当前目录名或文件名
    string fileName;
    // 父级
    logNode *parent;
    // 子级
    logNode *child;
    // 同级
    logNode *slibing;
    // 0 -- dir; 1 -- file
    bool dirOrfile;
    // child的数目
    int childSize;
    // 构造函数等 - fileTag: 0-dir, 1-file
    logNode(const string &theName, logNode *theParent, bool fileTag, int size = 0)
        : fileName(theName), parent(theParent), child(nullptr), slibing(nullptr), dirOrfile(fileTag), childSize(size) {}
    ~logNode() {
        // 递归析构 -- 删一个目录只需要删一个节点即可
        // 调用起来方便
        logNode *curNode = child, *preNode = child;
        while (curNode != nullptr) {
            preNode = curNode;
            curNode = curNode->slibing;
            delete preNode;
        }
        if (parent)
            parent->childSize--;
    }
};

class catalogTree {
public:
    catalogTree();
    ~catalogTree();
    // 列出当前目录下的文件和目录
    void ls() const;
    // for data test
    void dir() const;
    // 列出当前目录的绝对路径
    void cd() const;
    // 进入str对应的路径（绝对/相对）
    void cdStr(const string &targetPath);
    // 在当前目录创建名为dirName的目录
    void mkdir(const string &dirName);
    // 在当前目录创建名为fileName的文件
    void mkfile(const string &fileName);
    // 删除当前目录下名为fileName的目录或文件
    void erase(const string &fileName);
    // 删除空目录(-p : 连带parent一块)
    // void rmdir(const string &dirName, bool tag = false);
    // 将当前的目录树保存到targetPath对应的文件里
    void save(const string &targetPath);
    // 从sourcePath文件读入并恢复目录树信息
    void load(const string &sourcePath);
    // 退出程序
    void quit() const;
    // 运行程序
    void execute();
private:
    // 根目录
    logNode *root;
    // 当前所在目录(最底层目录)
    logNode *currentDir;
    // infile/outfile
    ifstream infile;
    ofstream outfile;
    // split, 将地址拆分成string
    vector<string> split(const string &thePath, char tag = '/') const;
    void subSave(logNode *root);
    void subLoad(logNode *parent, int nodeCount);
    void display() const;
    static bool cmp(const string &s1, const string &s2);
};

#endif