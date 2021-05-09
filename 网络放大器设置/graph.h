#ifndef _LINKED_W_DI_GRAPH_H_
#define _LINKED_W_DI_GRAPH_H_

#include "../exception.h"
#include <vector>

struct node {
    int number;                     // 节点序号
    int degradeFromParent;          // 从起点出发的衰减量
    int degradeToLeaf;              // 到达终点的衰减量
    bool boosterHere;               // 此处是否放了放大器
    vector<node> reachs;            // 可达节点 - 数组表示

    node(int theNumber, int dFP): number(theNumber), degradeFromParent(dFP), boosterHere(false) {}
    void output(ostream &out) const {
        out << boosterHere << ' ' << degradeFromParent << ' ' << degradeToLeaf << ' ';
    }
};

ostream & operator<<(ostream &out, const node &x) {
    x.output(out);
    return out;
}

class wDigraph {
public:
    void placeBoosters(node *x);
    void output(ostream &out) const;
private:
    node *sourceNode;
    int tolerance;                  // 最大可容忍衰减量   
};

void wDigraph::placeBoosters(node *x) {
    // 计算x的衰减量, 若小于容忍值, 则在x的子节点放置一个放大器
    x->degradeToLeaf = 0;           // 初始化
    
    for (vector<node>::iterator iter = x->reachs.begin(); iter != x->reachs.end(); iter++) {
        placeBoosters(&*iter);
        int degradation = iter->degradeToLeaf + iter->degradeFromParent;
        if (degradation > tolerance) {
            iter->boosterHere = true;
            degradation = iter->degradeFromParent;
        }
        if (x->degradeToLeaf < degradation)
            x->degradeToLeaf = degradation;
    }
}

#endif