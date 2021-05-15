#include "linkedWDigraph.h"

void solution1() {
    DAG graph;
    graph.placeBooster(1, 0);
    graph.output();
}

void solution2() {
    DAG graph;
    graph.branchJudge();
    graph.output();
}

int main() {
    // solution1();
    solution2();

    return 0;
}