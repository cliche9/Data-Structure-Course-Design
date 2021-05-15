#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

void diff() {
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
            return;
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
            return;
        } else {
            puts("Accept");
        }
    }
    end = clock();
    cout << "All Accept -- by branchjudge -- time = " << double(end - start) / CLOCKS_PER_SEC << "s\n\n";
}

void draw() {
    system("clang++ solution1.cpp -o solution1.cpp");
    for (int i = 1; i <= 100; i++) {
        string name = to_string(i);
        string todo;
        todo = "./solution1.cpp < data/input/input" + name + ".in > data/outputImg/" + name + ".dot";
        system(todo.c_str());
        todo = "dot -Tjpg data/outputImg/" + to_string(i) + ".dot -o " + "data/outputImg/img" + to_string(i) + ".jpg";
        system(todo.c_str());
    }
}

int main() {
    draw();

    return 0;
}