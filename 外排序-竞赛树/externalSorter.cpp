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
    fileLog(string sPath, string tPath): sourcePath(sPath), targetPath(tPath), visitDiskTimesOfStringGenerator(0), visitDiskTimesOfOutput(0), totalVisitDiskTimes(0) {}
    string sourcePath;
    string targetPath;
    int visitDiskTimesOfStringGenerator;
    int visitDiskTimesOfOutput;
    int totalVisitDiskTimes;
};

// 外排序器
template <class T>
class externalSorter {
public:
    externalSorter(int bufferSize, fileLog file);
    void sortAndOutput();
    void kWaySortAndOutput();
    int fileDistVisitTimes() const { return fileToSort.totalVisitDiskTimes; }
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
    ifstream infile(fileToSort.sourcePath + ".in");
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
    // 初始化选手数组，访存1次
    ++fileToSort.visitDiskTimesOfStringGenerator;
    // 生成辅助败者树
    minLoserTree<sequentialStringElement<T> > supportLoserTree(buffer_s, bufferSize / 2);
    // 将顺串生成到多个小文件中，编号为1-n
    int winner = 0;
    sequentialStringElement<T> s;
    ofstream outfile;
    while (buffer_s[supportLoserTree.winner()].value != INT_MAX) {
        // 循环，直到最终赢者为INT_MAX
        winner = supportLoserTree.winner();
        // 更新顺串个数，到后面最后的顺串编号肯定是顺串的个数
        int sequentialTag = buffer_s[winner].key;
        numbersOfSequentialString = sequentialTag;
        // 打开对应顺串的文件
        outfile.open(fileToSort.targetPath + '_' + to_string(sequentialTag) + ".out", ios::app);
        // 输出到对应文件
        outfile << buffer_s[winner].value << ' ';
        ++fileToSort.visitDiskTimesOfStringGenerator;
        // 从源文件继续读取
        if (!(infile >> s.value))
            s.value = INT_MAX;
        ++fileToSort.visitDiskTimesOfStringGenerator;
        s.key = (s.value < buffer_s[winner].value || s.value == INT_MAX) ? sequentialTag + 1 : sequentialTag;
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
            infile.open(fileToSort.targetPath + '_' + to_string(i) + ".out");
            while (infile >> t_value)
                cout << t_value << ' ';
            cout << endl;
            infile.close();
        }
        cout << "\n# 以上是生成的顺串, 该阶段的访存次数为: " << fileToSort.visitDiskTimesOfStringGenerator << endl;
    }
}

// 缓存区需要留出输出部分，否则一次一次输出内外村读取实在是太耗时间了
template <class T>
void externalSorter<T>::sortAndOutput() {
    // 生成缓冲区
    while (bufferSize < numbersOfSequentialString + 1) {
        cout << "缓冲区空间不足, 请更新缓冲区大小, 大小至少为: " << numbersOfSequentialString + 1 << "\n请输入缓冲区可存放元素个数: ";
        cin >> bufferSize;
    }
    ifstream infile;
    ofstream outfile;
    // 检验特殊情况-只有一个顺串，直接输出即可
    if (numbersOfSequentialString == 1) {
        T *buffer = new T[bufferSize + 1];
        infile.open(fileToSort.targetPath + "_1.out");
        outfile.open(fileToSort.targetPath + ".out");
        int i = 1;
        while (buffer[i] != INT_MAX) {
            // 填满一次缓存区/读取一次文件记一次文件读取
            ++fileToSort.visitDiskTimesOfOutput;
            for (i = 1; i <= bufferSize; i++) {
                if (!(infile >> buffer[i])) {
                    buffer[i] = INT_MAX;
                    break;
                }
            }
            // 是否将缓冲区填满，填满则说明可以继续读取，否则说明文件已经读取完了，两种情况都要输出到文件
            for (int j = 1; j < i; j++)
                outfile << buffer[j] << ' ';
            // 清空一次缓存区/输出一次文件记一次文件读取
            ++fileToSort.visitDiskTimesOfOutput;
        }
        infile.close();
        outfile.close();
        delete[] buffer;
    }
    else {
        // 设置位置记录器
        int points[numbersOfSequentialString + 1];
        // 计算每个顺串拥有的缓存区大小，此处顺串数目+1，目的是为了给输出区域分出一定空间
        int lines = bufferSize / (numbersOfSequentialString + 1);
        // 设置缓存区
        T **buffer = new T*[lines];
        for (int i = 0; i < lines; i++)
            // 设置+1，此时buffer[i][0]表示输出区域，填满即输出
            buffer[i] = new T[numbersOfSequentialString + 1];

        // 设置读取文件流
        for (int i = 1; i <= numbersOfSequentialString; i++) {
            infile.open(fileToSort.targetPath + '_' + to_string(i) + ".out");
            for (int j = 0; j < lines; j++) {
                // 将文件读入缓存区，读满/完为止
                if (!(infile >> buffer[j][i])) {
                    buffer[j][i] = INT_MAX;
                    break;
                }
            }
            ++fileToSort.visitDiskTimesOfOutput;
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
            // 将输者树赢者在缓存区存放位置的值(winnerValue)放入输出缓存区
            buffer[indexOf[0]++][0] = buffer[indexOf[winner]][winner];       
            // buffer[i][0]是输出缓存区，indexOf[0]记录这一列的当前位置，当indexOf[0] == lines时，说明输出缓存区满了，这时候输出到文件
            if (indexOf[0] == lines) {
                outfile.open(fileToSort.targetPath + ".out", ios::app);
                // 输出到文件
                for (int i = 0; i < lines; i++)
                    outfile << buffer[i][0] << ' ';
                indexOf[0] = 0;
                ++fileToSort.visitDiskTimesOfOutput;
                outfile.close();
            }
            // 当前缓冲区没有读取完，所以将下一个元素替换当前胜者，重构输者树，该情况包括了文件读取完的情况，前面设置了INT_MAX作为标记
            if (indexOf[winner] + 1 < lines)
                supportLoserTree.rePlay(winner, buffer[++indexOf[winner]][winner]);
            // 如果当前winner所在缓冲区已经全部读取完，需要清空重新读入新的一列
            else {
                infile.open(fileToSort.targetPath + '_' + to_string(winner) + ".out");
                // 设置指针位置，恢复到上次读取文件末尾数据的下一个位置
                infile.seekg(points[winner]);
                for (int i = 0; i < lines; i++) {
                    // 将新的一批数据读入缓存区
                    if (!(infile >> buffer[i][winner])) {
                        buffer[i][winner] = INT_MAX;
                        break;
                    }
                }
                // 重新读入新的一列，访问文件
                ++fileToSort.visitDiskTimesOfOutput;
                // 记录下一次读取文件的开始位置
                points[winner] = infile.tellg();
                infile.close();
                indexOf[winner] = 0;
                supportLoserTree.rePlay(winner, buffer[0][winner]);
            }
        }
        // 完成上述过程，需要将输出缓存区内容全部输出到文件里
        if (indexOf[0]) {
            // 如果输出缓存区有内容
            outfile.open(fileToSort.targetPath + ".out", ios::app);
            // 输出到文件，此时结尾是indexOf[0]
            for (int i = 0; i < indexOf[0]; i++)
                outfile << buffer[i][0] << ' ';
            indexOf[0] = 0;
            // 访存一次
            ++fileToSort.visitDiskTimesOfOutput;
            outfile.close();
        }

        for (int i = 0; i < lines; i++)
            delete[] buffer[i];
        delete[] buffer;
    }
    fileToSort.totalVisitDiskTimes = fileToSort.visitDiskTimesOfOutput + fileToSort.visitDiskTimesOfStringGenerator;
    // 排序结束，输出结果
    cout << "外排序成功, 是否需要输出排序结果? (Y/N)\n";
    char tag;
    cin >> tag;
    if (tag == 'y' || tag == 'Y') {
        T t_value;
        int count = 0;
        infile.open(fileToSort.targetPath + ".out");
        while (infile >> t_value) {
            if (count == 10) {
                cout << endl;
                count = 0;
            }
            cout << t_value << ' ';
            count++;
        }
        infile.close();
        cout << "\n# 以上是外排序的结果, 该阶段的访存次数为: " << fileToSort.visitDiskTimesOfOutput << endl;
    }
}

template <class T>
void externalSorter<T>::kWaySortAndOutput() {
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
        infile.open(fileToSort.targetPath + "_1.out");
        outfile.open(fileToSort.targetPath + ".out");
        int i = 1;
        while (buffer[i] != INT_MAX) {
            for (i = 1; i <= bufferSize; i++) {
                if (!(infile >> buffer[i])) {
                    ++fileToSort.visitDiskTimesOfOutput;
                    buffer[i] = INT_MAX;
                    break;
                }
                ++fileToSort.visitDiskTimesOfOutput;
            }
            // 是否将缓冲区填满，填满则说明可以继续读取，否则说明文件已经读取完了，两种情况都要输出到文件
            for (int j = 1; j < i; j++) {
                outfile << buffer[j] << ' ';
                ++fileToSort.visitDiskTimesOfOutput;
            }
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
            infile.open(fileToSort.targetPath + '_' + to_string(i) + ".out");
            for (int j = 0; j < lines; j++) {
                // 将文件读入缓存区，读满/完为止
                if (!(infile >> buffer[j][i])) {
                    ++fileToSort.visitDiskTimesOfOutput;
                    buffer[j][i] = INT_MAX;
                    break;
                }
                ++fileToSort.visitDiskTimesOfOutput;
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
            outfile.open(fileToSort.targetPath + ".out", ios::app);
            // 输出，访存次数++
            outfile << winnerValue << ' ';
            ++fileToSort.visitDiskTimesOfOutput;
            // 当前缓冲区没有读取完，所以将下一个元素替换当前胜者，重构输者树，该情况包括了文件读取完的情况，前面设置了INT_MAX作为标记
            if (indexOf[winner] + 1 < lines)
                supportLoserTree.rePlay(winner, buffer[++indexOf[winner]][winner]);
            // 如果当前winner所在缓冲区已经全部读取完，需要清空重新读入新的一列
            else {
                infile.open(fileToSort.targetPath + '_' + to_string(winner) + ".out");
                // 设置指针位置，恢复到上次读取文件末尾数据的下一个位置
                infile.seekg(points[winner]);
                for (int i = 0; i < lines; i++) {
                    // 将新的一批数据读入缓存区
                    if (!(infile >> buffer[i][winner])) {
                        ++fileToSort.visitDiskTimesOfOutput;
                        buffer[i][winner] = INT_MAX;
                        break;
                    }
                    ++fileToSort.visitDiskTimesOfOutput;
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
    fileToSort.totalVisitDiskTimes = fileToSort.visitDiskTimesOfOutput + fileToSort.visitDiskTimesOfStringGenerator;
    // 排序结束，输出结果
    cout << "外排序成功, 是否需要输出排序结果? (Y/N)\n";
    char tag;
    cin >> tag;
    if (tag == 'y' || tag == 'Y') {
        T t_value;
        int count = 0;
        infile.open(fileToSort.targetPath + ".out");
        while (infile >> t_value) {
            if (count == 10) {
                cout << endl;
                count = 0;
            }
            cout << t_value << ' ';
            count++;
        }
        infile.close();
        cout << "\n# 以上是外排序的结果, 该阶段的访存次数为: " << fileToSort.visitDiskTimesOfOutput << endl;
    }
}