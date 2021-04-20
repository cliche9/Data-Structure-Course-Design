#include "minLoserTree.h"
#include "minLoserTree.cpp"
#include "externalSorter.cpp"

int main() {
    string rootDirectory = "/Users/abc_mac/Documents/Code/数据结构课设/外排序-竞赛树/data/";
    string inFilename;
    cout << "请输入你要进行排序的文件名称: ";
    cin >> inFilename;

    fileLog fileToSort(rootDirectory + "input/" + inFilename, rootDirectory + "output/" + inFilename);
    int bufferSize = 0;
    cout << "请输入缓冲区可存放元素个数: ";
    cin >> bufferSize;
    externalSorter<int> sorter(bufferSize, fileToSort);
    sorter.sortAndOutput();
    
    return 0;
}