#include <iostream>
#include <cstdlib>
using namespace std;

// 44 92 97
int main() {
    system("clang++ 网络放大器设置/solution.cpp -o 网络放大器设置/solution.cpp");
    for (int i = 1; i <= 100; i++) {
        cout << "正在测试数据" << i << endl;
        string name = to_string(i);
        string todo;
        todo = "./网络放大器设置/solution.cpp < 网络放大器设置/data/input/input" + name + ".in > 网络放大器设置/data/output/" + name + ".out";
        system(todo.c_str());
        todo = "diff 网络放大器设置/data/output/" + name + ".out 网络放大器设置/data/dagData/outputSTD/" + name + ".out";
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