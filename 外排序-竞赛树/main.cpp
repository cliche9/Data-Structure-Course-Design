#include "minLoserTree.h"
#include "minLoserTree.cpp"

int main() {
    int nums[11] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    minLoserTree<int> loserTree(nums, 10);

    loserTree.output();

    return 0;
}