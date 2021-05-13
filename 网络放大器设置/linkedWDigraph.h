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
    vector<wEdge> preEdges;     // 终点为this的边集
    vector<wEdge> edges;        // 起点为this的边集
    vertexNode(int theNumber = 0, int p = 0): number(theNumber), pressure(p), inDegree(0) {
        edges.clear();
    }
};

class DAG {
private:
    int numberOfVertex;         // 节点个数
    int numberOfEdges;          // 边的个数
    int Pmin, Pmax;             // 压力最小值 & 加压后的压力
    int numberOfBoosters;       // 加压器个数
    vertexNode *vertexOf;       // 节点集
    int *sequence;              // 节点访问顺序
    bool *boosterHere;          // 是否放石油放大器
public:
    DAG() {
        cin >> numberOfVertex >> numberOfEdges >> Pmax;
        Pmin = 0;
        vertexOf = new vertexNode[numberOfVertex + 1];
        sequence = new int[numberOfVertex];
        boosterHere = new bool[numberOfVertex + 1];
        // 初始化元素
        for (int i = 1; i <= numberOfVertex; ++i) {
            vertexOf[i].number = i;
            boosterHere[i] = false;
        }
        // 读入m条边
        for (int i = 0; i < numberOfEdges; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            ++vertexOf[v].inDegree;
            vertexOf[u].edges.push_back(wEdge(u, v, w));
            vertexOf[v].preEdges.push_back(wEdge(u, v, w));
        }
        // 求出节点访问顺序
        topoSort();
    }
    // 拓扑排序, 找出节点的访问顺序, 每个节点都要先访问完前面的节点再访问
    void topoSort() {
        int inDegreeOf[numberOfVertex + 1];
        int now = 0;
        queue<int> q;
        for (int i = 1; i <= numberOfVertex; ++i) {
            inDegreeOf[i] = vertexOf[i].inDegree;
            if (!vertexOf[i].inDegree)
                q.push(i);
        }

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            sequence[now++] = v;
            for (vector<wEdge>::iterator e = vertexOf[v].edges.begin(); e != vertexOf[v].edges.end(); ++e)
                if ((--inDegreeOf[e->to]) == 0)
                    q.push(e->to);
        }
    }
    // 回溯放置放大器
    void placeBooster(int level, int number_of_boosters) {
        // 标识是否需要放放大器
        bool tag = false;

        if (level >= numberOfVertex - 1) {
            // 到达决策树底层, 可以返回了
            if (number_of_boosters < numberOfBoosters) {
                // 找到更优解, 更新结果
                numberOfBoosters = number_of_boosters;
            }
            return;
        }
        // 找到该层决策的节点
        int theVertexNumber = sequence[level];
        vertexOf[theVertexNumber].pressure = 0;
        // 特殊处理源点
        if (level == 0) {
            vertexOf[theVertexNumber].pressure = Pmax;
            boosterHere[level] = false;
            placeBooster(1, 0);
            return;
        }
        // 求theVertexNumber处的油压
        for (int i = 0; i < level; ++i)
            for (vector<wEdge>::iterator e = vertexOf[sequence[i]].edges.begin(); e != vertexOf[sequence[i]].edges.end(); ++e)
                if (e->to == theVertexNumber)
                    vertexOf[theVertexNumber].pressure = max(vertexOf[theVertexNumber].pressure, vertexOf[sequence[i]].pressure - e->weight);

        int k = 0;
        for (; k < 2; ++k) {
            if (vertexOf[theVertexNumber].pressure == Pmax) {
                boosterHere[level] = false;
                placeBooster(level + 1, number_of_boosters);
                break;
            }
            if (vertexOf[theVertexNumber].pressure <= Pmin) {
                boosterHere[level] = true;
                ++number_of_boosters;
                vertexOf[theVertexNumber].pressure = Pmax;
                if (number_of_boosters >= numberOfBoosters)
                    return;
                placeBooster(level + 1, number_of_boosters);
                break;
            }
            if (k == 0) {
                for (vector<wEdge>::iterator e = vertexOf[theVertexNumber].edges.begin(); e != vertexOf[theVertexNumber].edges.end(); ++e)
                    if (vertexOf[theVertexNumber].pressure - e->weight < Pmin) {
                        tag = true;
                        break;
                    }
            }
        }
        if (tag == true || k == 1) {
            boosterHere[level] = true;
            ++number_of_boosters;
            vertexOf[theVertexNumber].pressure = Pmax;
            if (number_of_boosters >= numberOfBoosters)
                return;
            placeBooster(level + 1, number_of_boosters);
        }
        else {
            boosterHere[level] = false;
            placeBooster(level + 1, number_of_boosters);
        }
    }

    void solution1() {
        for (int i = 0; i < numberOfVertex; ++i) {
            int s = sequence[i];

        }
    }
};

#endif