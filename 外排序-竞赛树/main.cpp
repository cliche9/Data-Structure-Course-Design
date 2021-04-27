#include "minLoserTree.h"
#include "minLoserTree.cpp"
#include "externalSorter.cpp"

/*
缓冲区大 - 归并路数大 --> 归并次数小一点 : 每个顺串分配到的空间小，可能需要多次访存，但是归并次数小
缓冲区大 - 归并路数小 --> 归并次数多一点，但是多不了太多 : 每个顺串分配到的空间大，访存次数可能要小一点
缓冲区小 - 归并路数大 --> 归并次数小 : 每个顺串分配到的空间更小，访存次数一定比较多
缓冲区小 - 归并路数小 --> 归并次数多 : 每个顺串分配到的空间相对大一点，访存次数相对小一点
*/

int main() {
    cout << "请输入待排序文件所在目录的绝对路径: \n";
    string rootDirectoryOfInput;
    cin >> rootDirectoryOfInput;
    cout << "请输入输出文件所在目录的绝对路径: \n";
    string rootDirectoryOfOutput;
    cin >> rootDirectoryOfOutput;
    string inFilename;
    cout << "请输入你要进行排序的文件名称: ";
    cin >> inFilename;
    cout << "请输入待排序文件的文件类型: (如.txt, .in)\n";
    string sType, tType;
    cin >> sType;
    cout << "请输入输出文件的文件类型: (如.txt, .out)\n";
    cin >> tType;

    fileLog fileToSort(rootDirectoryOfInput + inFilename, rootDirectoryOfOutput + inFilename, sType, tType);
    int bufferSize = 0;
    cout << "请输入缓冲区可存放元素个数: ";
    cin >> bufferSize;
    externalSorter<int> sorter(bufferSize, fileToSort);
    cout << "请选择归并方式: \n直接归并--输入1; k路归并--输入2\n归并方式 = ";
    int judge = 0;
    cin >> judge;
    while (judge != 1 && judge != 2) {
        cout << "输入方式无效, 请重新输入, 归并方式 = ";
        cin >> judge;
    }
    if (judge == 1)
        sorter.allWaySortAndOutput();
    else
        sorter.kWaySortAndOutput();
    cout << "\n# 外排序结束, 总的访存次数为: " << sorter.fileDistVisitTimes() << endl;

    return 0;
}