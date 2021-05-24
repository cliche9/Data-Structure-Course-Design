#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include "redBlackTree.h"

template <class K, class V>
class dict {
public:
    dict();
    ~dict();
    dict(const dict &theDict);
    dict &operator=(const dict &theDict);
    string &operator[](const string &theKey);
    string &
    pair<RBNode<K, V> *, bool> insert(const string &theKey, const string &theValue);
    void erase(const string &theKey);

private:
    RBTree<K, V> rbTree;
};

#endif