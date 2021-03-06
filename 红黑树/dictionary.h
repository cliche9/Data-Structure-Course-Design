#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include "redBlackTree.h"
#include <fstream>
#include <sstream>

template <class K, class V>
class dict {
public:
    dict(string name): dictName(name) {}
    dict(RBTree<K, V> *theTree, string name): rbTree(theTree->root(), theTree->size()), dictName(name) {};
    dict(const dict<K, V> &theDict): rbTree(theDict.rbTree) {};
    dict &operator=(const dict<K, V> &theDict);
    V &operator[](const K &theKey);
    RBNode<K, V> *find(const K &theKey);
    pair<RBNode<K, V> *, bool> insert(const K &theKey, const V &theValue);
    void erase(const K &theKey);
    int size() const { return rbTree.size(); }
    bool empty() const { return rbTree.empty(); }
    RBTree<K, V> *theTree() { return &rbTree; }
    static dict<K, V> *merge(dict<K, V> *d1, dict<K, V> *d2, string name);
    void menu();
    void init();
private:
    RBTree<K, V> rbTree;
    string dictName;
    void inputOneLine(string &k, string &v, stringstream &ss);
    void visual();
};

template <class K, class V>
dict<K, V> &dict<K, V>::operator=(const dict<K, V> &theDict) {
    if (this != theDict)
        rbTree = theDict.rbTree;
    return *this;
}

template <class K, class V>
V &dict<K, V>::operator[](const K &theKey) {
    RBNode<K, V> *t = rbTree.find(theKey);
    if (t == nullptr)
        t = rbTree.insert(theKey, V()).first;
    return t->value;
}

template <class K, class V>
RBNode<K, V> *dict<K, V>::find(const K &theKey) {
    return rbTree.find(theKey);
}

template <class K, class V>
pair<RBNode<K, V> *, bool> dict<K, V>::insert(const K &thekey, const V &theValue) {
    return rbTree.insert(thekey, theValue);
}

template <class K, class V>
void dict<K, V>::erase(const K &theKey) {
    rbTree.erase(theKey);
}

template <class K, class V>
dict<K, V> *dict<K, V>::merge(dict<K, V> *d1, dict<K, V> *d2, string name) {
    return new dict<K, V>(RBTree<K, V>::merge(d1->theTree(), d2->theTree()), name);
}

template <class K, class V>
void dict<K, V>::menu() {
	cout << "|==============================================|\n"
	     << "|                   ????????????                   |\n"
	     << "|         1.????????????        2.????????????         |\n"
	     << "|         3.????????????        4.????????????         |\n"
	     << "|                                              |\n"
	     << "|             ?????????????????????????????????           |\n"
	     << "|             ?????????????????????????????????           |\n"
	     << "|==============================================|\n\n\n";
    K key;
    V value;
    string opt;
    while (true) {
        visual();
        cin >> opt;
        if (opt == "1") {
            cin >> key;
            RBNode<string, string> *theNode = find(key);
            if (theNode == nullptr)
                cout << "Not found word: " << key << endl;
            else
                cout << theNode->key << ": " << theNode->value << "\n";
        }
        else if (opt == "2") {
            cin >> key;
            erase(key);
        }
        else if (opt == "3") {
            string oneLine;
            getline(cin, oneLine);
            stringstream ss(oneLine);
            inputOneLine(key, value, ss);
            pair<RBNode<K, V> *, bool> t = insert(key, value);
            if (!t.second) 
                cout << key << "?????????:\n" << key << ": " << t.first->value << "\n";
        }
        else if (opt == "4")
            return;
        else {
            cout << "Invalid operation.\n";
            fflush(stdin);
        }
    }
}

template <class K, class V>
void dict<K, V>::init() {
    string path;
    ifstream infile;
    cout << "???????????????????????????: \n";
    cin >> path;
    infile.open(path);
    while (!infile.is_open()) {
        cout << "????????????, ???????????????: \n";
        cin >> path;
        infile.open(path);
    }
    string oneLine;
    while (getline(infile, oneLine)) {
        string key, value;
        stringstream ss(oneLine);
        inputOneLine(key, value, ss);
        insert(key, value);
    }
    cout << "?????????????????????!\n";
}

template <class K, class V>
void dict<K, V>::inputOneLine(string &k, string &v, stringstream &ss) {
    ss >> k;
    string t;
    while (ss >> t) {
        t += " ";
        v += t;
    }
}

template <class K, class V>
void dict<K, V>::visual() {
    rbTree.visual(dictName);
}

#endif