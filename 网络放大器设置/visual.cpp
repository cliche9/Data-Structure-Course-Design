#include <iostream>
#include <cstdlib>
using namespace std;

void draw1() {
    system("clang++ visual1.cpp -o visual1");
    for (int i = 1; i <= 100; i++) {
        string name = to_string(i);
        string todo;
        todo = "./visual1 < data/input/input" + name + ".in > data/outputImg/" + name + "a.dot";
        system(todo.c_str());
        todo = "dot -Tjpg data/outputImg/" + to_string(i) + "a.dot -o " + "data/outputImg/img" + to_string(i) + "a.jpg";
        system(todo.c_str());
    }
}

void draw2() {
    system("clang++ visual2.cpp -o visual2");
    for (int i = 1; i <= 100; i++) {
        string name = to_string(i);
        string todo;
        todo = "./visual2 < data/input/input" + name + ".in > data/outputImg/" + name + "b.dot";
        system(todo.c_str());
        todo = "dot -Tjpg data/outputImg/" + to_string(i) + "b.dot -o " + "data/outputImg/img" + to_string(i) + "b.jpg";
        system(todo.c_str());
    }
}

int main() {
    draw1();
    draw2();

    return 0;
}