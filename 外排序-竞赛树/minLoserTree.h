#ifndef _LOSER_TREE_H_
#define _LOSER_TREE_H_

#include "../exception.h"

template <class T>
class minLoserTree {
public:
    minLoserTree(T *thePlayers, int theNumberOfPlayers) {
        tree = winners = NULL;
        initialize(thePlayers, theNumberOfPlayers);
    }
    ~minLoserTree() { delete[] tree; delete[] winners; }
    void initialize(T *, int);
    int winner() const { return tree[0]; }
    int loser(int i) const { return (i < numberOfPlayers) ? tree[i] : 0; };
    void rePlay(int, T);
    void output() const;
private:
    T *players;
    int *tree;          // array for loser tree
    int *winners;       // array for winners
    int numberOfPlayers;
    int lowExt;         // lowest-level external nodes
    int offset;         // = 2^log(n - 1) - 1
    void play(int, int, int);
};

#endif