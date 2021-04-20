/*
    顺串的归并顺序会对排序的顺序造成影响，其中归并时先将所含元素最少的顺串进行归并，以此为基准，这样每个元素的比较次数
    相对较少，即重复比较次数最多的是最短的串，类似huffman编码-huffman树。
*/

#include "minLoserTree.h"
#include <fstream>

// 顺串生成时使用元素
template <class T>
struct sequentialStringElement {
    T key;
    T value;
    bool operator<=(const sequentialStringElement &s) {
        if (key == s.key)
            return value < s.value;
        return key <= s.key;
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
    void sortAndOutput();
private:
    int bufferSize;
    int numbersOfSequentialString;
    fileLog fileToSort;
    void sequentialStringGenerator();
};

template <class T>
externalSorter<T>::externalSorter(int buffer_size, fileLog file): bufferSize(buffer_size), fileToSort(file) {
    numbersOfSequentialString = 0;
    sequentialStringGenerator();
}

template <class T>
void externalSorter<T>::sequentialStringGenerator() {
    // 顺串生成器，利用最小输者树进行构造
    ifstream infile(fileToSort.sourcePath + ".txt");
    if (!infile.is_open())
        throw failToOpenFile("无法打开对应路径文件, 构建顺串失败!\n");
    
    while (bufferSize < 2) {
        cout << "缓冲区空间不足, 请更新缓冲区大小, 大小至少为: 2\n请输入缓冲区可存放元素个数: ";
        cin >> bufferSize;
    }
    cout << "正在构建顺串...\n";
    // 生成缓存区
    // 多一个空间，是为了构建败者树，为了模拟缓存区
    sequentialStringElement<T> *buffer_s = new sequentialStringElement<T>[bufferSize / 2 + 1];
    for (int i = 1; i <= bufferSize / 2; i++) {
        int t_value = 0;
        if (!(infile >> t_value))
            t_value = INT_MAX;
        buffer_s[i].key = 1;
        buffer_s[i].value = t_value;
    }
    // 生成辅助败者树
    minLoserTree<sequentialStringElement<T> > supportLoserTree(buffer_s, bufferSize / 2);
    // 将顺串生成到多个小文件中，编号为1-n
    int winner = 0;
    sequentialStringElement<T> s;
    ofstream outfile;
    while (buffer_s[supportLoserTree.winner()].value != INT_MAX) {
        // 循环，直到最终赢者为INT_MAX
        winner = supportLoserTree.winner();
        // 更新顺串个数
        int sequentialTag = buffer_s[winner].key;
        numbersOfSequentialString = (sequentialTag <= numbersOfSequentialString) ? numbersOfSequentialString : sequentialTag;
        outfile.open(fileToSort.targetPath + to_string(sequentialTag) + ".txt", ios::app);
        outfile << buffer_s[winner].value << ' ';
        if (!infile.eof())
            infile >> s.value;
        else
            s.value = INT_MAX;
        s.key = (s.value < buffer_s[winner].value) ? sequentialTag + 1 : sequentialTag;
        supportLoserTree.rePlay(winner, s);
        outfile.close();
    }
    infile.close();
    delete[] buffer_s;
    // 输出顺串
    cout << "顺串构建成功, 是否需要输出构建的顺串? (Y/N)\n";
    char tag;
    cin >> tag;
    if (tag == 'y' || tag == 'Y') {
        T t_value;
        for (int i = 1; i <= numbersOfSequentialString; i++) {
            cout << "顺串" << i << ": ";
            infile.open(fileToSort.targetPath + to_string(i) + ".txt");
            while (infile >> t_value)
                cout << t_value << ' ';
            cout << endl;
            infile.close();
        }
        cout << "\n# 以上是生成的顺串\n";
    }
}

template <class T>
void externalSorter<T>::sortAndOutput() {
    // 生成缓冲区
    while (bufferSize < numbersOfSequentialString) {
        cout << "缓冲区空间不足, 请更新缓冲区大小, 大小至少为: " << numbersOfSequentialString << "\n请输入缓冲区可存放元素个数: ";
        cin >> bufferSize;
    }
    ifstream infile;
    ofstream outfile;
    // 检验特殊情况-只有一个顺串，直接输出即可
    if (numbersOfSequentialString == 1) {
        T *buffer = new T[bufferSize + 1];
        infile.open(fileToSort.targetPath + "1.txt");
        outfile.open(fileToSort.targetPath + ".txt");
        int i = 1;
        while (buffer[i] != INT_MAX) {
            for (i = 1; i <= bufferSize; i++) {
                if (!(infile >> buffer[i])) {
                    buffer[i] = INT_MAX;
                    break;
                }
            }
            // 是否将缓冲区填满，填满则说明可以继续读取，否则说明文件已经读取完了，两种情况都要输出到文件
            for (int j = 1; j < i; j++)
                outfile << buffer[j] << ' ';
        }
        infile.close();
        outfile.close();
        delete[] buffer;
    }
    else {
        // 设置位置记录器
        int points[numbersOfSequentialString + 1];
        // 计算每个顺串拥有的缓存区大小
        int lines = bufferSize / numbersOfSequentialString;
        // 设置缓存区
        T **buffer = new T*[lines];
        for (int i = 0; i < lines; i++)
            buffer[i] = new T[numbersOfSequentialString + 1];
        // 设置读取文件流
        for (int i = 1; i <= numbersOfSequentialString; i++) {
            infile.open(fileToSort.targetPath + to_string(i) + ".txt");
            for (int j = 0; j < lines; j++) {
                // 将文件读入缓存区，读满/完为止
                if (!(infile >> buffer[j][i])) {
                    buffer[j][i] = INT_MAX;
                    break;
                }
            }
            // 记录下一次读取文件时的开始位置
            points[i] = infile.tellg();
            infile.close();
        }
        // 生成辅助败者树
        minLoserTree<T> supportLoserTree(buffer[0], numbersOfSequentialString);
        // 败者树循环生成最终结果
        int winner = 0;
        int indexOf[numbersOfSequentialString + 1];
        memset(indexOf, 0, sizeof(indexOf));
        while (buffer[indexOf[supportLoserTree.winner()]][supportLoserTree.winner()] != INT_MAX) {
            // 循环，直到最终赢者为INT_MAX
            winner = supportLoserTree.winner();
            int winnerValue = buffer[indexOf[winner]][winner];            
            outfile.open(fileToSort.targetPath + ".txt", ios::app);
            outfile << winnerValue << ' ';
            // 当前缓冲区没有读取完，所以将下一个元素替换当前胜者，重构输者树，该情况包括了文件读取完的情况，前面设置了INT_MAX作为标记
            if (indexOf[winner] + 1 < lines)
                supportLoserTree.rePlay(winner, buffer[++indexOf[winner]][winner]);
            // 如果当前winner所在缓冲区已经全部读取完，需要清空重新读入新的一列
            else {
                infile.open(fileToSort.targetPath + to_string(winner) + ".txt");
                // 设置指针位置，恢复到上次读取文件末尾数据的下一个位置
                infile.seekg(points[winner]);
                for (int i = 0; i < lines; i++) {
                    // 将新的一批数据读入缓存区
                    if (!(infile >> buffer[i][winner])) {
                        buffer[i][winner] = INT_MAX;
                        break;
                    }
                }
                // 记录下一次读取文件的开始位置
                points[winner] = infile.tellg();
                infile.close();
                indexOf[winner] = 0;
                supportLoserTree.rePlay(winner, buffer[0][winner]);
            }
            outfile.close();        
        }
        for (int i = 0; i < lines; i++)
            delete[] buffer[i];
        delete[] buffer;
    }
    // 排序结束，输出结果
    cout << "外排序成功, 是否需要输出排序结果? (Y/N)\n";
    char tag;
    cin >> tag;
    if (tag == 'y' || tag == 'Y') {
        T t_value;
        int count = 0;
        infile.open(fileToSort.targetPath + ".txt");
        while (infile >> t_value) {
            if (count == 10) {
                cout << endl;
                count = 0;
            }
            cout << t_value << ' ';
            count++;
        }
        infile.close();
        cout << "\n# 以上是外排序的结果\n";
    }
}