#ifndef _SKIP_LIST_H_
#define _SKIP_LIST_H_
#include <iostream>
using namespace std;

template <class T>
struct skipNode {
    T element;
    skipNode<T> **next;
    skipNode(const T &theElement, int size): element(theElement) {
        next = new skipNode<T> *[size];      // 指针数组--每个节点记录多层
    }
};

template <class T>
class skipList {
public:
    skipList(T largeKey, int maxPairs = 10000, float prob = 0.5);
    ~skipList();
    bool empty() const { return _size == 0; }
    int size() const { return _size; }
    skipNode<T> *find(const T &);
    void erase(const T &);
    void insert(const T &);
    int removeMin();
    int removeMax();
    int xorValue();
    void restruct();
    int compareResult() const { return compareCount; }
    void display() const;   // 展示当前跳表

private:
    float cutOff;           // 用于产生级号
    int curMaxLevel;        // 当前最大级数
    int _size;               // 数对个数
    int maxLevel;           // 所允许最大级数
    int compareCount;       // 当前操作的比较次数
    T tailKey;              // 一个很大的key值，作为末尾标记
    skipNode<T> *headerNode;     // 头节点指针
    skipNode<T> *tailNode;       // 尾节点指针
    skipNode<T> **last;          // 指针数组--存放什么？
    int level() const;      // 对每个节点产生级数
    skipNode<T> *search(const T &);    // search，设置last
};

#endif