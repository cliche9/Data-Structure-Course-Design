#ifndef _CATALOG_TREE_H_
#define _CATALOG_TREE_H_
#include "../exception.h"

template <class T>
struct logNode {
    // 当前目录名称
    string currentPathName;
    // 父级
    logNode *parent;
    // 子级
    logNode *child;
    // 同级
    logNode *slibing;
};

template <class T>
class catalogTree {
public:
    catalogTree();
    ~catalogTree();
    void insert();
    void erase();
    void listChilds() const;
private:
    logNode<T> *root;
    string currentPath;
    int size;
};

#endif