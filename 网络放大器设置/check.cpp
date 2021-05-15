#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

int main() {
    int start = 0, end = 0;

    system("clang++ solution1.cpp -o solution1.cpp");
    start = clock();
    for (int i = 1; i <= 100; i++) {
        cout << "正在测试数据" << i << endl;
        string name = to_string(i);
        string todo;
        todo = "./solution1.cpp < data/input/input" + name + ".in > data/output/" + name + ".out";
        system(todo.c_str());
        todo = "diff data/output/" + name + ".out data/dagData/outputSTD/" + name + ".out";
        auto ret = system(todo.c_str());
        if (ret) {
            puts("error");
            return 0;
        } else {
            puts("Accept");
        }
    }
    end = clock();
    cout << "All Accept -- by backtrack -- time = " << double(end - start) / CLOCKS_PER_SEC << "s\n\n";

    system("clang++ solution2.cpp -o solution2.cpp");
    start = clock();
    for (int i = 1; i <= 100; i++) {
        cout << "正在测试数据" << i << endl;
        string name = to_string(i);
        string todo;
        todo = "./solution2.cpp < data/input/input" + name + ".in > data/output/" + name + ".out";
        system(todo.c_str());
        todo = "diff data/output/" + name + ".out data/dagData/outputSTD/" + name + ".out";
        auto ret = system(todo.c_str());
        if (ret) {
            puts("error");
            return 0;
        } else {
            puts("Accept");
        }
    }
    end = clock();
    cout << "All Accept -- by branchjudge -- time = " << double(end - start) / CLOCKS_PER_SEC << "s\n\n";

    return 0;
}