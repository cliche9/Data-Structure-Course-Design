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
    fileLog(string sPath, string tPath): sourcePath(sPath), targetPath(tPath), visitDiskLog(0) {}
    string sourcePath;
    string targetPath;
    int visitDiskLog;
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
}

template <class T>
void externalSorter<T>::sequentialStringGenerator() {
    // 顺串生成器，利用最小输者树进行构造
    ifstream infile(fileToSort.sourcePath);
    if (!infile.is_open())
        throw failToOpenFile("无法打开对应路径文件, 构建顺串失败!\n");
    
    // 生成缓存区
    buffer = new sequentialStringElement<T>[bufferSize / 2];
    for (int i = 0; i < bufferSize / 2; i++) {
        int t_value = 0;
        if (!infile.eof())
            infile >> t_value;
        else
            t_value = INT_MAX;
        buffer[i].key = 1;
        buffer[i].value = t_value;
    }
    // 生成辅助败者树
    supportLoserTree = new minLoserTree<sequentialStringElement<T> >(buffer, bufferSize / 2);
    // 将顺串生成到多个小文件中，编号为1-n
    int winner = 0, i = 0;
    sequentialStringElement<T> s;
    ofstream outfile;
    while (buffer[supportLoserTree->winner()].value != INT_MAX) {
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
    delete supportLoserTree;
}

template <class T>
void externalSorter<T>::sortAndOutput() {
    // 生成缓冲区
    while (bufferSize < numbersOfSequentialString + 1) {
        cout << "缓冲区空间不足, 请更新缓冲区大小, 大小至少为: " << numbersOfSequentialString << "\n请输入缓冲区可存放元素个数: ";
        cin >> bufferSize;
    }
    // 设置位置记录器
    int points[numbersOfSequentialString + 1] = {0};
    // 计算每个顺串拥有的缓存区大小
    int lines = bufferSize / numbersOfSequentialString;
    // 设置缓存区
    buffer = new T[lines][numbersOfSequentialString + 1];
    // 设置读取文件流
    ifstream infile;
    for (int i = 1; i <= numbersOfSequentialString; i++) {
        infile.open(fileToSort.sourcePath + i);
        for (int j = 0; j < lines; j++) {
            // 将文件读入缓存区，读满/完为止
            if (!infile.eof())
                infile >> buffer[j][i];
            else { 
                buffer[j][i] = INT_MAX;
                break;
            }
        }
        // 记录下一次读取文件时的开始位置
        points[i] = infile.tellg();
        infile.close();
    }
    // 生成辅助败者树
    supportLoserTree = new minLoserTree<T>(buffer[0], numbersOfSequentialString);
    // 败者树循环生成最终结果
    int winner = 0;
    int indexOf[numbersOfSequentialString + 1] = {0};
    ofstream outfile;
    while (buffer[indexOf[supportLoserTree->winner()]][supportLoserTree->winner()] != INT_MAX) {
        // 循环，直到最终赢者为INT_MAX
        winner = supportLoserTree->winner();
        int winnerValue = buffer[indexOf[winner]][winner];            
        outfile.open(fileToSort.targetPath, ios::app);
        outfile << winnerValue << ' ';
        // 当前缓冲区没有读取完，所以将下一个元素替换当前胜者，重构输者树，该情况包括了文件读取完的情况，前面设置了INT_MAX作为标记
        if (indexOf[winner] + 1 < lines)
            supportLoserTree->rePlay(winner, buffer[++indexOf[winner]][winner]);
        // 如果当前winner所在缓冲区已经全部读取完，需要清空重新读入新的一列
        else {
            infile.open(fileToSort.sourcePath + winner);
            // 设置指针位置，恢复到上次读取文件末尾数据的下一个位置
            infile.seekg(points[winner]);
            for (int i = 0; i < lines; i++) {
                // 将新的一批数据读入缓存区
                if (!infile.eof())
                    infile >> buffer[i][winner];
                else {
                    buffer[i][winner] = INT_MAX;
                    break;
                }
            }
            // 记录下一次读取文件的开始位置
            points[winner] = infile.tellg();
            infile.close();
            indexOf[winner] = 0;
            supportLoserTree->rePlay(winner, buffer[0][winner]);
        }
        outfile.close();        
    }
    for (int i = 0; i < lines; i++)
        delete[] buffer[i];
    delete[] buffer;
    delete supportLoserTree;
}


