#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

void diff() {
    int start = 0, end = 0;
    system("clang++ solution1.cpp -o solution1");
    start = clock();
    for (int i = 1; i <= 100; i++) {
        int s, e;
        cout << "正在测试数据" << i << endl;
        string name = to_string(i);
        string todo;
        todo = "./solution1 < data/input/input" + name + ".in > data/output/" + name + ".out";
        s = clock();
        system(todo.c_str());
        e = clock();
        todo = "diff data/output/" + name + ".out data/dagData/outputSTD/" + name + ".out";
        auto ret = system(todo.c_str());
        if (ret) {
            puts("error");
            return;
        } else {
            cout << "Accept " << i << ", time = " << 1000.0 * double(e - s) / CLOCKS_PER_SEC << "ms\n";
        }
    }
    end = clock();
    cout << "All Accept -- by backtrack -- time = " << 1000.0 * double(end - start) / CLOCKS_PER_SEC << "ms\n\n";

    system("clang++ solution2.cpp -o solution2");
    start = clock();
    for (int i = 1; i <= 100; i++) {
        int s, e;
        cout << "正在测试数据" << i << endl;
        string name = to_string(i);
        string todo;
        todo = "./solution2 < data/input/input" + name + ".in > data/output/" + name + ".out";
        s = clock();
        system(todo.c_str());
        e = clock();
        todo = "diff data/output/" + name + ".out data/dagData/outputSTD/" + name + ".out";
        auto ret = system(todo.c_str());
        if (ret) {
            puts("error");
            return;
        } else {
            cout << "Accept " << i << ", time = " << 1000.0 * double(e - s) / CLOCKS_PER_SEC << "ms\n";
        }
    }
    end = clock();
    cout << "All Accept -- by branchjudge -- time = " << 1000.0 * double(end - start) / CLOCKS_PER_SEC << "ms\n\n";
}

int main() {
    diff();

    return 0;
}