#include "linkedWDigraph.h"

void solution1() {
    DAG graph;
    graph.placeBooster(0, 0);
    graph.output();
}

int main() {
    freopen("/Users/abc_mac/Documents/Code/数据结构课设/网络放大器设置/data/input/input44.in", "r", stdin);
    freopen("/Users/abc_mac/Documents/Code/数据结构课设/网络放大器设置/data/output/44.out", "w", stdout);
    solution1();

    return 0;
}