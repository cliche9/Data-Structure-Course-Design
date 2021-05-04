#include "catalogTree.h"
#include "catalogTree.cpp"

int main() {
    string rootName;
    cout << "请输入您要设置的根目录名称: ";
    cin >> rootName;
    catalogTree fileTree(rootName);

    fileTree.execute();

    return 0;
}