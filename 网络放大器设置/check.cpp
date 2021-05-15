#include <iostream>
#include <cstdlib>
using namespace std;

int main() {
    system("clang++ solution.cpp -o solution.cpp");
    for (int i = 1; i <= 100; i++) {
        cout << "正在测试数据" << i << endl;
        string name = to_string(i);
        string todo;
        todo = "./solution.cpp < data/input/input" + name + ".in > data/output/" + name + ".out";
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
    puts("All Accept");
    return 0;
}