#include "minLoserTree.h"

template <class T>
void minLoserTree<T>::initialize(T *thePlayers, int theNumberOfPlayers) {
    int n = theNumberOfPlayers;
    if (n < 2)
        throw illegalParameterValue("must have at least 2 players!");
    // 初始化private元素
    players = thePlayers;
    numberOfPlayers = theNumberOfPlayers;
    if (tree != NULL)
        delete[] tree;
    if (winners != NULL)
        delete[] winners;
    tree = new int[n + 1];
    winners = new int[n + 1];

    // 计算 s = 2^log(n - 1)
    int i, s;
    // 找最底层最左端的内部节点，s
    for (s = 1; 2 * s <= n - 1; s += s);
    // lowExt: 最底层内部节点个数
    lowExt = 2 * (n - s);
    // offset: 设置的偏移量
    offset = 2 * s - 1;

    // 比赛，直到最外部一行
    for (int i = 2; i <= lowExt; i += 2)
        play((offset + i) / 2, i - 1, i);
    
    // 处理特殊情况，n为奇数，让内外节点比赛
    if (n % 2 == 1) {
        play(n / 2, tree[n - 1], lowExt + 1);
        i = lowExt + 3;
    }
    else
        i = lowExt + 2;
    
    // i是最左端余下的外部节点
    for (; i <= n; i += 2)
        play((i - lowExt + n - 1) / 2, i - 1, i);
    // 设置全局赢者
    tree[0] = winners[1];
}

template <class T>
void minLoserTree<T>::play(int p, int leftChild, int rightChild) {
    // 从p开始向上比赛，结果存在tree[p]中
    // leftChild - p的左孩子
    // rightChild - p的右孩子
    winners[p] = (players[leftChild] <= players[rightChild]) ? leftChild : rightChild;
    // 平局默认leftChild赢
    tree[p] = (players[leftChild] <= players[rightChild]) ? rightChild : leftChild;

    // 可能有更多比赛
    while (p % 2 == 1 && p > 1) {
        winners[p / 2] = (players[winners[p - 1]] <= players[winners[p]]) ? winners[p - 1] : winners[p];
        tree[p / 2] = (players[winners[p - 1]] <= players[winners[p]]) ? winners[p] : winners[p - 1];
        // 到父节点
        p /= 2;
    }
}

template <class T>
void minLoserTree<T>::rePlay(int thePlayer, T newElement) {
    // 外排序中重赛的永远是最后赢者，所以不需要考虑赢者树了
    int n = numberOfPlayers;
    if (thePlayer <= 0 || thePlayer > n)
        throw illegalParameterValue("Player index is illegal");
    
    players[thePlayer] = newElement;
    int matchNode = 0;

    // 找到thePlayer的第一场比赛
    if (thePlayer <= lowExt)
        // 从最底层开始
        matchNode = (offset + thePlayer) / 2;
    else
        matchNode = (thePlayer - lowExt + n - 1) / 2;

    // 改变的是赢者
    for (; matchNode >= 1; matchNode /= 2) {
        if (players[tree[matchNode]] <= players[thePlayer]) {
            int winner = tree[matchNode];
            tree[matchNode] = thePlayer;
            thePlayer = winner;
        }
    }
    // 记录最终赢者
    tree[0] = thePlayer;
}

template <class T>
void minLoserTree<T>::output() const {
    cout << "number of players  = " << numberOfPlayers
         << " lowExt = " << lowExt
         << " offset = " << offset << endl;
    cout << "complete loser tree pointers are " << endl;
    for (int i = 1; i < numberOfPlayers; i++)
        cout << tree[i] << ' ';
    cout << endl;
    cout << "the final winner is " << endl;
    cout << tree[0] << endl;
}
