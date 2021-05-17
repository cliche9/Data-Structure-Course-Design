#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
using namespace std;

void diff() {
    system("clang++ solution1.cpp -o solution1");
    ofstream time1("data/time1.out");
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
        time1 << 1000.0 * double(e - s) / CLOCKS_PER_SEC << '\n';
        if (ret) {
            puts("error");
            return;
        } else {
            cout << "Accept " << i << ", time = " << 1000.0 * double(e - s) / CLOCKS_PER_SEC << "ms\n";
        }
    }
    cout << "All Accept -- by backtrack\n\n";
    time1.close();

    ofstream time2("data/time2.out");
    system("clang++ solution2.cpp -o solution2");
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
        time2 << 1000.0 * double(e - s) / CLOCKS_PER_SEC << '\n';
        if (ret) {
            puts("error");
            return;
        } else {
            cout << "Accept " << i << ", time = " << 1000.0 * double(e - s) / CLOCKS_PER_SEC << "ms\n";
        }
    }
    cout << "All Accept -- by branchjudge\n\n";
    time2.close();
}

int main() {
    diff();
    system("python3 visual.py");

    return 0;
}