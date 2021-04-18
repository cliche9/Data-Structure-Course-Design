/*
    顺串的归并顺序会对排序的顺序造成影响，其中归并时先将所含元素最少的顺串进行归并，以此为基准，这样每个元素的比较次数
    相对较少，即重复比较次数最多的是最短的串，类似huffman编码-huffman树。
*/

#include "minLoserTree.h"
#include "minLoserTree.cpp"
#include <fstream>

// 顺串生成时使用元素
template <class T>
struct sequentialStringElement {
    T key;
    T value;
    bool operator<(const sequentialStringElement &s) {
        if (key == s.key)
            return value < s.value;
        return key < s.key;
    }
};

// 文件读取记录
struct fileLog {
    fileLog(string sPath, string tPath, int theSize): sourcePath(sPath), targetPath(tPath), size(theSize) {}

    string sourcePath;
    string targetPath;
    int size;
};

// 外排序器
template <class T>
class externalSorter {
public:
    externalSorter(int bufferSize, fileLog file);
    ~externalSorter();
    void sortAndOutput();
private:
    minLoserTree<T> *supportLoserTree;
    T *buffer;
    int bufferSize;
    int numbersOfSequentialString;
    fileLog fileToSort;
    void sequentialStringGenerator();
};

template <class T>
externalSorter<T>::externalSorter(int buffer_size, fileLog file): bufferSize(buffer_size), fileToSort(file) {
    supportLoserTree = NULL;
    numbersOfSequentialString = 0;
    sequentialStringGenerator();
    buffer = new int[bufferSize];
}

template <class T>
void externalSorter<T>::sequentialStringGenerator() {
    // 顺串生成器，利用最小输者树进行构造
    ifstream infile(fileToSort.sourcePath);
    if (!infile.is_open())
        throw failToOpenFile("无法打开对应路径文件, 构建顺串失败!\n");
    
    // 生成缓存区
    buffer = new sequentialStringElement<T>[bufferSize / 2];
    // 生成辅助败者树
    supportLoserTree = new minLoserTree<sequentialStringElement<T> >(buffer, bufferSize / 2);
    for (int i = 0; i < bufferSize / 2; i++) {
        int t_value = 0;
        if (!infile.eof())
            infile >> t_value;
        else
            t_value = INT_MAX;
        buffer[i].key = 1;
        buffer[i].value = t_value;
    }
    // 将顺串生成到多个小文件中，编号为1-n
    int winner = 0, i = 0;
    sequentialStringElement<T> s;
    ofstream outfile;
    while (supportLoserTree->winner() != INT_MAX) {
        // 循环，直到最终赢者为INT_MAX
        winner = supportLoserTree->winner();
        // 更新顺串个数
        int sequentialTag = buffer[winner].key;
        numbersOfSequentialString = (sequentialTag <= numbersOfSequentialString) ? numbersOfSequentialString : sequentialTag;
        outfile.open(fileToSort.targetPath + sequentialTag, ios::app);
        outfile << buffer[winner].value << ' ';
        if (!infile.eof())
            infile >> s.value;
        else
            s.value = INT_MAX;
        s.key = (s.value < buffer[winner].value) ? sequentialTag : sequentialTag + 1;
        supportLoserTree->rePlay(winner, s);
        outfile.close();
    }
    infile.close();
    delete[] buffer;
    delete[] supportLoserTree;
}

template <class T>
void externalSorter<T>::sortAndOutput() {
    // 生成缓冲区
    if (bufferSize < numbersOfSequentialString + 1)
        cout << "缓冲区空间不足, 请更新缓冲区大小, 大小至少为: " << numbersOfSequentialString << endl << "请输入缓冲区可存放元素个数: ";
    cin >> bufferSize;
    
    buffer = new T[numbersOfSequentialString + 1][bufferSize / (numbersOfSequentialString + 1)];
    // 生成辅助败者树
    supportLoserTree = new minLoserTree<sequentialStringElement<T> >(buffer, bufferSize / 2);
}
