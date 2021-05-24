#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_
#include "redBlackTree.h"
#include <fstream>
#include <sstream>

template <class K, class V>
class dict {
public:
    dict() {};
    dict(const dict<K, V> &theDict): rbTree(theDict.rbTree) {};
    dict &operator=(const dict<K, V> &theDict);
    V &operator[](const K &theKey);
    RBNode<K, V> *find(const K &theKey);
    pair<RBNode<K, V> *, bool> insert(const K &theKey, const V &theValue);
    void erase(const K &theKey);
    int size() const { return rbTree.size(); }
    bool empty() const { return rbTree.empty(); }
    RBTree<K, V> *theTree() { return &rbTree; }
    void menu();
    void init();
private:
    RBTree<K, V> rbTree;
    void inputOneLine(string &k, string &v, stringstream &ss);
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
void dict<K, V>::menu() {
    init();
	cout << "|==============================================|\n"
	     << "|                   使用说明                   |\n"
	     << "|         1.查找单词        2.删除单词         |\n"
	     << "|         3.添加单词        4.关闭词典         |\n"
	     << "|                                              |\n"
	     << "|             查找和删除只需输入英文           |\n"
	     << "|             添加需要输入中文和英文           |\n"
	     << "|==============================================|\n\n\n";
    K key;
    V value;
    int opt;
    while (true) {
        cin >> opt;
        switch(opt) {
            case 1: {
                cin >> key;
                RBNode<string, string> *theNode = find(key);
                if (theNode == nullptr)
                    cout << "Not found word: " << key << endl;
                else
                    cout << theNode->key << ": " << theNode->value << "\n";
                break;
            }
            case 2: {
                cin >> key;
                erase(key);
                break;
            }
            case 3: {
                string oneLine;
                getline(cin, oneLine);
                stringstream ss(oneLine);
                inputOneLine(key, value, ss);
                pair<RBNode<K, V> *, bool> t = insert(key, value);
                if (!t.second) 
                    cout << key << "已存在:\n" << key << ": " << t.first->value << "\n";
                break;
            }
            case 4:
                return;
            default:
                cout << "Invalid operation.\n";
        }
    }
}

template <class K, class V>
void dict<K, V>::init() {
    string path;
    ifstream infile;
    cout << "请输入词典文件路径: \n";
    cin >> path;
    infile.open(path);
    while (!infile.is_open()) {
        cout << "路径错误, 请重新输入: \n";
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
    cout << "词典初始化完成!\n"; 
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

#endif