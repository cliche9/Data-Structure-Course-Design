#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include "redBlackTree.h"

template <class K, class E>
class dict {
public:
    dict();
    ~dict();
    dict(const dict &theDict);
    dict &operator=(const dict &theDict);
    string &operator[](const string &theKey);
    pair<
private:
    RBTree<K, E> rbTree;
};

#endif