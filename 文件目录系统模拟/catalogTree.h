#ifndef _CATALOG_TREE_H_
#define _CATALOG_TREE_H_
#include "../exception.h"

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
    logNode(const string &theName, logNode *theParent, bool fileTag)
        : fileName(theName), parent(theParent), child(nullptr), slibing(nullptr), dirOrfile(fileTag), childSize(0) {}
    ~logNode() {
        // 递归析构 -- 删一个目录只需要删一个节点即可
        // 调用起来方便
        logNode *curNode = child, *preNode = child;
        while (child != nullptr) {
            preNode = curNode;
            curNode = curNode->slibing;
            delete preNode;
        }
    }
};

class catalogTree {
public:
    catalogTree(const string &rootName);
    ~catalogTree();
    // 列出当前目录下的文件和目录
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
    // 将当前的目录树保存到targetPath对应的文件里
    void save(const string &targetPath);
    // 从sourcePath文件读入并恢复目录树信息
    void load(const string &sourcePath);
    // 退出程序
    void quit() const;
private:
    // 根目录
    logNode *root;
    // 当前所在目录(最底层目录)
    logNode *deepestDir;
};

#endif