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
    vector<vertexNode> vertexOf;       // 节点集
    vector<int> sequence;              // 节点访问顺序
    vector<bool> boosterHere;          // 是否放石油放大器
public:
    DAG() {
        cin >> numberOfVertex >> numberOfEdges >> Pmax;
        numberOfBoosters = numberOfVertex;
        Pmin = 0;
        // 初始化元素
        vertexOf.resize(numberOfVertex + 1);
        sequence.resize(numberOfVertex);
        boosterHere.resize(numberOfVertex + 1);
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
        // 判断是否结束
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
                    // 需要求出到达该点的最大压力, 但是同时又要考虑是否能到达全部边, 这里需要考虑吗?
                    vertexOf[theVertexNumber].pressure = max(vertexOf[theVertexNumber].pressure, vertexOf[sequence[i]].pressure - e->weight);

        if (vertexOf[theVertexNumber].pressure <= Pmin) {
            // 从前面的点无法到达该点/到达的压力 <= 0, 必须放一个放大器
            boosterHere[level] = true;
            ++number_of_boosters;
            vertexOf[theVertexNumber].pressure = Pmax;
            if (number_of_boosters >= numberOfBoosters)
                // 此时如果已经 >= 最优结果了, 不需要再向下了, 剪枝
                return;
            placeBooster(level + 1, number_of_boosters);
        }
        else {
            for (vector<wEdge>::iterator e = vertexOf[theVertexNumber].edges.begin(); e != vertexOf[theVertexNumber].edges.end(); ++e)
                if (vertexOf[theVertexNumber].pressure - e->weight < Pmin) {
                    // 此处tag标识是否有边无法通过, 如果有说明需要加放大器
                    tag = true;
                    break;
                }
        }
        // 如果有边无法通过, 加放大器
        if (tag == true) {
            boosterHere[level] = true;
            ++number_of_boosters;
            vertexOf[theVertexNumber].pressure = Pmax;
            if (number_of_boosters >= numberOfBoosters)
                return;
            placeBooster(level + 1, number_of_boosters);
        }
        // 所有边都可以通过, 不需要加放大器
        else {
            boosterHere[level] = false;
            placeBooster(level + 1, number_of_boosters);
        }
    }
    // 输出结果
    void output() const {
        cout << numberOfBoosters << endl;
    }
};

#endif