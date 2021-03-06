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
    fileLog(string sPath, string tPath, string sType, string tType): sourcePath(sPath), targetPath(tPath), sourceType(sType), targetType(tType), visitDiskTimesOfStringGenerator(0), visitDiskTimesOfOutput(0), totalVisitDiskTimes(0) {}
    string sourcePath;
    string targetPath;
    string sourceType;
    string targetType;
    int visitDiskTimesOfStringGenerator;
    int visitDiskTimesOfOutput;
    int totalVisitDiskTimes;
};

// 外排序器
template <class T>
class externalSorter {
public:
    externalSorter(int bufferSize, fileLog file);
    void allWaySortAndOutput();
    void kWaySortAndOutput();
    int fileDistVisitTimes() const { return fileToSort.totalVisitDiskTimes; }
    void outputResult() const;
private:
    int bufferSize;
    int numbersOfSequentialString;
    fileLog fileToSort;
    void sequentialStringGenerator();
    void sortSequentialStringsByGroup(int mergeWays, int start, int end, int mergeRound, int sequentialGroupNumber);
};

template <class T>
externalSorter<T>::externalSorter(int buffer_size, fileLog file): bufferSize(buffer_size), fileToSort(file) {
    numbersOfSequentialString = 0;
    sequentialStringGenerator();
}

template <class T>
void externalSorter<T>::sequentialStringGenerator() {
    // 顺串生成器，利用最小输者树进行构造
    ifstream infile(fileToSort.sourcePath + fileToSort.sourceType);
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
        outfile.open(fileToSort.targetPath + "_0_" + to_string(sequentialTag) + fileToSort.targetType, ios::app);
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
            infile.open(fileToSort.targetPath + "_0_" + to_string(i) + fileToSort.targetType);
            while (infile >> t_value)
                cout << t_value << ' ';
            cout << endl;
            infile.close();
        }
        cout << "\n# 以上是生成的顺串.\n";
    }
    cout << "共生成" + to_string(numbersOfSequentialString) + "个顺串, 顺串构建的访存次数为: " << fileToSort.visitDiskTimesOfStringGenerator << endl;
}

// 缓存区需要留出输出部分，否则一次一次输出内外存读取实在是太耗时间了
template <class T>
void externalSorter<T>::sortSequentialStringsByGroup(int mergeWays, int start, int end, int mergeRound, int sequentialGroupNumber) {
    // 参数说明: mergeWays = 归并参与的顺串数目 , start = 归并顺串号起始位置, end = 归并顺串号结束位置, mergeRound = 当前归并轮次, sequentialGroupNumber = 当前一组顺串的组号
    // 生成缓冲区
    while (bufferSize < mergeWays + 1) {
        cout << "缓冲区空间不足, 请更新缓冲区大小, 大小至少为: " << mergeWays + 1 << "\n请输入缓冲区可存放元素个数: ";
        cin >> bufferSize;
    }
    ofstream outfile;
    // 检验特殊情况-只有一个顺串，直接输出即可
    if (numbersOfSequentialString == 1) {
        ifstream infile;
        T *buffer = new T[bufferSize + 1];
        infile.open(fileToSort.targetPath + '_' + to_string(mergeRound) + "_1" + fileToSort.targetType);
        outfile.open(fileToSort.targetPath + "_1_1" + fileToSort.targetType);
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
        return;
    }
    // 归并路数为1 --- 此处归并路数是指这一次归并的顺串数目
    if (mergeWays == 1) {
        ifstream infile;
        T *buffer = new T[bufferSize + 1];
        infile.open(fileToSort.targetPath + '_' + to_string(mergeRound) + '_' + to_string(start) + fileToSort.targetType);
        outfile.open(fileToSort.targetPath + '_' + to_string(mergeRound + 1) + '_' + to_string(sequentialGroupNumber) + fileToSort.targetType, ios::app);
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
        return;
    }
    // 归并路大于1，进行多路归并
    ifstream infile[mergeWays + 1];
    // 计算每个顺串拥有的缓存区大小，此处顺串数目+1，目的是为了给输出区域分出一定空间
    int lines = bufferSize / (mergeWays + 1);
    // 设置缓存区
    T **buffer = new T*[lines];
    for (int i = 0; i < lines; i++)
        // 设置+1，此时buffer[i][0]表示输出区域，填满即输出
        buffer[i] = new T[mergeWays + 1];

    int index = 1;
    // 设置读取文件流
    for (int i = start; i <= end; i++) {
        infile[index].open(fileToSort.targetPath + '_' + to_string(mergeRound) + '_' + to_string(i) + fileToSort.targetType);
        for (int j = 0; j < lines; j++) {
            // 将文件读入缓存区，读满/完为止
            if (!(infile[index] >> buffer[j][index])) {
                buffer[j][index] = INT_MAX;
                break;
            }
        }
        ++fileToSort.visitDiskTimesOfOutput;
        index++;
    }
    // 生成辅助败者树
    minLoserTree<T> supportLoserTree(buffer[0], mergeWays);
    // 败者树循环生成最终结果
    int winner = 0;
    int indexOf[mergeWays + 1];
    memset(indexOf, 0, sizeof(indexOf));

    while (buffer[indexOf[supportLoserTree.winner()]][supportLoserTree.winner()] != INT_MAX) {
        // 循环，直到最终赢者为INT_MAX
        winner = supportLoserTree.winner();
        // 将输者树赢者在缓存区存放位置的值(winnerValue)放入输出缓存区
        buffer[indexOf[0]++][0] = buffer[indexOf[winner]][winner];
        // buffer[i][0]是输出缓存区，indexOf[0]记录这一列的当前位置，当indexOf[0] == lines时，说明输出缓存区满了，这时候输出到文件
        if (indexOf[0] == lines) {
            outfile.open(fileToSort.targetPath + '_' + to_string(mergeRound + 1) + '_' + to_string(sequentialGroupNumber) + fileToSort.targetType, ios::app);
            // 输出到文件
            for (int i = 0; i < lines; i++)
                outfile << buffer[i][0] << ' ';
            indexOf[0] = 0;
            ++fileToSort.visitDiskTimesOfOutput;
            outfile.close();
        }
        // 当前缓冲区没有读取完，所以将下一个元素替换当前胜者，重构输者树，该情况包括了文件读取完的情况，前面设置了INT_MAX作为标记
        if (indexOf[winner] + 1 < lines) {
            supportLoserTree.rePlay(winner, buffer[indexOf[winner] + 1][winner]);
            indexOf[winner]++;
        }
        // 如果当前winner所在缓冲区已经全部读取完，需要清空重新读入新的一列
        else {
            for (int i = 0; i < lines; i++) {
                // 将新的一批数据读入缓存区
                if (!(infile[winner] >> buffer[i][winner])) {
                    buffer[i][winner] = INT_MAX;
                    break;
                }
            }
            // 重新读入新的一列，访问文件
            ++fileToSort.visitDiskTimesOfOutput;
            // 记录下一次读取文件的开始位置
            indexOf[winner] = 0;
            supportLoserTree.rePlay(winner, buffer[0][winner]);
        }
    }
    // 完成上述过程，需要将输出缓存区内容全部输出到文件里
    if (indexOf[0]) {
        // 如果输出缓存区有内容
        outfile.open(fileToSort.targetPath + '_' + to_string(mergeRound + 1) + '_' + to_string(sequentialGroupNumber) + fileToSort.targetType, ios::app);
        // 输出到文件，此时结尾是indexOf[0]
        for (int i = 0; i < indexOf[0]; i++)
            outfile << buffer[i][0] << ' ';
        indexOf[0] = 0;
        // 访存一次
        ++fileToSort.visitDiskTimesOfOutput;
        outfile.close();
    }
    // 关闭文件流
    for (int i = 1; i <= mergeWays; i++)
        infile[i].close();
    for (int i = 0; i < lines; i++)
        delete[] buffer[i];
    delete[] buffer;
}

template <class T>
void externalSorter<T>::kWaySortAndOutput() {
    // 引导输入k路归并
    int mergeWays = 0;
    cout << "请输入你要进行的外排序的归并路数 k = ";
    cin >> mergeWays;
    while (mergeWays < 2) {
        cout << "归并路数太小，无法进行外排序, 请重新输入 k = ";
        cin >> mergeWays;
    }
    // 生成缓冲区
    while (bufferSize < mergeWays + 1) {
        cout << "缓冲区空间不足, 请更新缓冲区大小, 大小至少为: " << mergeWays + 1 << "\n请输入缓冲区可存放元素个数: ";
        cin >> bufferSize;
    }
    ifstream infile;
    ofstream outfile;
    // 循环直到顺串数目为0
    int mergeRound = 0;
    while (numbersOfSequentialString > 1) {
        int mergeGroup = (numbersOfSequentialString % mergeWays) ? numbersOfSequentialString / mergeWays + 1 : numbersOfSequentialString / mergeWays;
        // 设置函数参数即可
        for (int i = 1; i <= mergeGroup; i++) {
            int numbersOfString = (numbersOfSequentialString - i * mergeWays > 0) ? mergeWays : (numbersOfSequentialString - (i - 1) * mergeWays);
            sortSequentialStringsByGroup(numbersOfString, (i - 1) * mergeWays + 1, (i - 1) * mergeWays + numbersOfString, mergeRound, i);
        }
        ++mergeRound;
        numbersOfSequentialString = mergeGroup;
    }
    // 排序结束，得到总访存次数
    fileToSort.totalVisitDiskTimes = fileToSort.visitDiskTimesOfOutput + fileToSort.visitDiskTimesOfStringGenerator;
    // 排序结束，输出结果
    fileToSort.targetPath += '_' + to_string(mergeRound) + "_1";
    outputResult();
}

template <class T>
void externalSorter<T>::allWaySortAndOutput() {
    sortSequentialStringsByGroup(numbersOfSequentialString, 1, numbersOfSequentialString, 0, 1);
    fileToSort.totalVisitDiskTimes = fileToSort.visitDiskTimesOfOutput + fileToSort.visitDiskTimesOfStringGenerator;
    fileToSort.targetPath += "_1_1";
    outputResult();
}

template <class T>
void externalSorter<T>::outputResult() const {
    // 排序结束，输出结果
    ifstream infile;
    cout << "外排序成功, 是否需要输出排序结果? (Y/N)\n";
    char judge;
    cin >> judge;
    if (judge == 'y' || judge == 'Y') {
        T t_value;
        int count = 0;
        infile.open(fileToSort.targetPath + fileToSort.targetType);
        while (infile >> t_value) {
            if (count == 10) {
                cout << endl;
                count = 0;
            }
            cout << t_value << ' ';
            count++;
        }
        infile.close();
        cout << "\n# 以上是外排序的结果.\n";
    }
    cout << "生成结果的访存次数为: " << fileToSort.visitDiskTimesOfOutput << endl;
}

