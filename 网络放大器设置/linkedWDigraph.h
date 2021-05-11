#ifndef _LINKED_W_DI_GRAPH_H_
#define _LINKED_W_DI_GRAPH_H_

#include "../exception.h"
#include <sstream>
#include <vector>
#include <queue>

// 有权边
struct wEdge {
    int from;                   // 起始点
    int to;                     // 目的点
    int weight;                 // 权值 - 消耗
    wEdge(int source = 0, int target = 0, int theWeight = 0): from(source), to(target), weight(theWeight) {}
};

struct vertexNode {
    int number;                 // 编号
    int inDegree;               // 节点的入度, 用于拓扑序
    int pressure;               // 压力值
    int degradeToEnd;           // 当前节点到终点的衰减量
    bool boosterHere;           // 是否放石油放大器
    vector<wEdge> preEdges;     // 终点为this的边集
    vector<wEdge> edges;        // 起点为this的边集
    vertexNode(int theNumber = 0, int p = 0): number(theNumber), pressure(p), inDegree(0), degradeToEnd(0), boosterHere(false) {
        edges.clear();
    }
};

class DAG {
private:
    int numberOfVertex;         // 节点个数
    int numberOfEdges;          // 边的个数
    int Pmin, Pmax;             // 压力最小值 & 加压后的压力
    int numberOfboosters;          // 加压器个数
    vector<vertexNode> vertexes;   // 节点集
    vector<int> sequence;          // 节点访问顺序
public:
    DAG() {
        cin >> numberOfVertex >> numberOfEdges >> Pmax;
        Pmin = 0;
        vertexes.clear(); 
        // 初始化元素
        for (int i = 1; i <= numberOfVertex; ++i)
            vertexes.push_back(vertexNode(i, Pmax));
        // 读入m条边
        for (int i = 0; i < numberOfEdges; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            ++vertexes[v].inDegree;
            vertexes[u].edges.push_back(wEdge(u, v, w));
            vertexes[v].preEdges.push_back(wEdge(u, v, w));
        }
    }
    // 拓扑排序, 找出节点的访问顺序, 每个节点都要先访问完前面的节点再访问
    void topoSort() {
        int inDegreeOf[numberOfVertex + 1];
        queue<int> q;
        for (int i = 1; i <= numberOfVertex; ++i) {
            inDegreeOf[i] = vertexes[i].inDegree;
            if (!vertexes[i].inDegree)
                q.push(i);
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            sequence.push_back(v);
            for (vector<wEdge>::iterator e = vertexes[v].edges.begin(); e != vertexes[v].edges.end(); ++e)
                if ((--inDegreeOf[e->to]) == 0)
                    q.push(e->to);
        }
    }

    void placeBooster(int source) {        
        // 更新当前点的pressure

        int maxCost = 0;
        // 回溯, 从当前点出发, 找可达点, 进行同样的操作, 记录下到达所有可达点所需要的最小压力 - 即最大消耗
        for (vector<wEdge>::iterator e = vertexes[source].edges.begin(); e != vertexes[source].edges.end(); ++e) {
            placeBooster(e->to);
            maxCost = max(maxCost, e->weight + vertexes[e->to].degradeToEnd);
        }
        // 如果当前点压力不满足要求, 放置放大器, 更新当前点压力
        if (vertexes[source].pressure < maxCost) {
            vertexes[source].boosterHere = true;
            vertexes[source].pressure = Pmax;
            ++numberOfboosters;
        }
        // 更新当前点的degradeToEnd为最大消耗
        vertexes[source].degradeToEnd = maxCost;
        
    }

    void solution1() {
        for (int i = 0; i < numberOfVertex; ++i) {
            int s = sequence[i];

        }
    }
};

#endif