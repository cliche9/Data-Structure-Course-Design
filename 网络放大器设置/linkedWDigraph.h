#ifndef _LINKED_W_DI_GRAPH_H_
#define _LINKED_W_DI_GRAPH_H_

#include "../exception.h"
#include <vector>
#include <queue>

// 有权边
struct wEdge {
    int to;                     // 目的点
    int weight;                 // 权值 - 消耗
    wEdge(int target = 0, int theWeight = 0): to(target), weight(theWeight) {}
};
// 顶点
struct vertexNode {
    int number;                 // 编号
    int inDegree;               // 节点的入度, 用于拓扑序
    int pressure;               // 压力值
    vector<wEdge> edges;        // 起点为this的边集
    vertexNode(int theNumber = 0, int p = 0): number(theNumber), pressure(p), inDegree(0) {
        edges.clear();
    }
};

// 分支定界用子集树节点
struct subNode {
    subNode *parent;            // 父节点
    int level;                  // 所在子集树层级
    bool boosterHere;           // 是否放放大器, 分支
    int pressure;               // 油压值
    int numberOfBoosters;       // 优先级
    subNode(subNode *p, int lev, bool tag, int press, int num): parent(p), level(lev), boosterHere(tag), pressure(press), numberOfBoosters(num) {}
};

// 指针有序
struct cmp {
    bool operator() (const subNode *a,const subNode *b) {
        return a->numberOfBoosters > b->numberOfBoosters;
    }
};

class DAG {
private:
    int numberOfVertex;         // 节点个数
    int numberOfEdges;          // 边的个数
    int Pmin, Pmax;             // 压力最小值 & 加压后的压力
    int numberOfBoosters;       // 加压器个数
    vector<vertexNode> vertexOf;        // 节点集
    subNode *extensiveNode;             // 扩展节点
    vector<subNode *> nodes;            // 用于delete
    // 队列最小堆, 用于分支定界
    priority_queue<subNode *, vector<subNode *>, cmp> minHeap;
    vector<int> sequence;               // 节点访问顺序
    vector<bool> boosterHere;           // 是否放石油放大器
    vector<bool> boosterHereFinal;      // 最终结果
public:
    DAG() {
        cin >> numberOfVertex >> numberOfEdges >> Pmax;
        numberOfBoosters = numberOfVertex;
        Pmin = 0;
        // 初始化元素
        vertexOf.resize(numberOfVertex + 1);
        sequence.resize(numberOfVertex + 1);
        boosterHere.resize(numberOfVertex + 1);
        boosterHereFinal.resize(numberOfVertex + 1);
        for (int i = 1; i <= numberOfVertex; ++i) {
            vertexOf[i].number = i;
            boosterHere[i] = false;
        }
        // 读入m条边
        for (int i = 0; i < numberOfEdges; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            ++vertexOf[v].inDegree;
            vertexOf[u].edges.push_back(wEdge(v, w));
        }
        // 求出节点访问顺序
        topoSort();
        // 设置源点初始变量
        vertexOf[1].pressure = Pmax;
        extensiveNode = new subNode(nullptr, 2, false, Pmax, 0);
        nodes.push_back(extensiveNode);
    }
    // 析构
    ~DAG() {
        for (vector<subNode *>::iterator ptr = nodes.begin(); ptr != nodes.end(); ++ptr) {
            delete *ptr;
            *ptr = nullptr;
        }
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
            sequence[++now] = v;
            for (vector<wEdge>::iterator e = vertexOf[v].edges.begin(); e != vertexOf[v].edges.end(); ++e)
                if ((--inDegreeOf[e->to]) == 0)
                    q.push(e->to);
        }
    }
    // 回溯(dfs)放置放大器
    void placeBooster(int level, int number_of_boosters) {
        // 标识是否需要放放大器
        bool tag = false;
        // 判断是否结束
        if (level >= numberOfVertex) {
            // 到达决策树底层, 可以返回了
            if (number_of_boosters < numberOfBoosters) {
                numberOfBoosters = number_of_boosters;
                boosterHereFinal = boosterHere;
            }
            return;
        }
        // 找到该层决策的节点
        int theVertexNumber = sequence[level];
        vertexOf[theVertexNumber].pressure = 0;
        // 特殊处理源点
        if (level == 1) {
            vertexOf[theVertexNumber].pressure = Pmax;
            boosterHere[level] = false;
            placeBooster(2, 0);
            return;
        }
        // 求theVertexNumber处的油压
        for (int i = 1; i < level; ++i)
            for (vector<wEdge>::iterator e = vertexOf[sequence[i]].edges.begin(); e != vertexOf[sequence[i]].edges.end(); ++e)
                if (e->to == theVertexNumber)
                    // 需要求出到达该点的最大压力
                    vertexOf[theVertexNumber].pressure = max(vertexOf[theVertexNumber].pressure, vertexOf[sequence[i]].pressure - e->weight);

        if (vertexOf[theVertexNumber].pressure <= Pmin)
            // 从前面的点无法到达该点/到达的压力 <= 0, 必须放一个放大器
            tag = true;
        if (!tag) {
            for (vector<wEdge>::iterator e = vertexOf[theVertexNumber].edges.begin(); e != vertexOf[theVertexNumber].edges.end(); ++e) {
                if (vertexOf[theVertexNumber].pressure - e->weight < Pmin) {
                    // 此处tag标识是否有边无法通过, 如果有说明需要加放大器
                    tag = true;
                    break;
                }
            }
        }
        // 如果必须要加放大器加放大器
        if (tag) {
            boosterHere[theVertexNumber] = true;
            vertexOf[theVertexNumber].pressure = Pmax;
            if (number_of_boosters + 1 >= numberOfBoosters)
                return;
            placeBooster(level + 1, number_of_boosters + 1);
        }
        // 所有边都可以通过, 加/不加放大器
        else {
            if (number_of_boosters + 1 >= numberOfBoosters) {
                boosterHere[theVertexNumber] = false;
                placeBooster(level + 1, number_of_boosters);
            }
            else {
                // 加放大器
                int pressure = vertexOf[theVertexNumber].pressure;
                boosterHere[theVertexNumber] = true;
                vertexOf[theVertexNumber].pressure = Pmax;
                placeBooster(level + 1, number_of_boosters + 1);
                // 不加放大器
                boosterHere[theVertexNumber] = false;
                vertexOf[theVertexNumber].pressure = pressure;
                placeBooster(level + 1, number_of_boosters);
            }
        }
    }
    // 分支定界(priority_queue)放置放大器
    void branchJudge() {
        // level记录的是该处理第几层的节点了, 比如level = 2, 说明该部分结果下, 下一个要处理的是sequence[2]
        int level = extensiveNode->level;
        while (level < numberOfVertex) {
            // 当前扩展节点油压值
            int theVertexNumber = sequence[level];
            int pressure = 0;
            // 求扩展节点油压初值
            for (int i = 1; i < level; ++i) {
                for (vector<wEdge>::iterator e = vertexOf[sequence[i]].edges.begin(); e != vertexOf[sequence[i]].edges.end(); ++e) {
                    if (e->to == theVertexNumber) {
                        subNode *curExtensiveNode = extensiveNode;
                        // 找到该路线下, 前面保存结果的油压值
                        for (int j = level - 1; j > i; --j)
                            curExtensiveNode = curExtensiveNode->parent;
                        // 需要求出到达该点的最大压力
                        pressure = max(pressure, curExtensiveNode->pressure - e->weight);
                    }
                }
            }
            bool tag = false;
            // 检查运输到子节点后油压是否 < Pmin
            for (vector<wEdge>::iterator e = vertexOf[theVertexNumber].edges.begin(); e != vertexOf[theVertexNumber].edges.end(); ++e) {
                if (pressure - e->weight < Pmin) {
                    tag = true;
                    break;
                }
            }
            subNode *temp = nullptr;
            // 油压均符合条件
            if (!tag) {
                // 优先不放放大器
                temp = new subNode(extensiveNode, level + 1, false, pressure, extensiveNode->numberOfBoosters);
                nodes.push_back(temp);
                minHeap.push(temp);
                temp = new subNode(extensiveNode, level + 1, true, Pmax, extensiveNode->numberOfBoosters + 1);
                nodes.push_back(temp);
                minHeap.push(temp);
            }
            else {
                temp = new subNode(extensiveNode, level + 1, true, Pmax, extensiveNode->numberOfBoosters + 1);
                nodes.push_back(temp);
                minHeap.push(temp);
            }
            extensiveNode = minHeap.top();
            level = extensiveNode->level;
            minHeap.pop();
        }

        numberOfBoosters = extensiveNode->numberOfBoosters;
        for (int i = numberOfVertex - 1; i > 1; --i) {
            boosterHereFinal[sequence[extensiveNode->level - 1]] = extensiveNode->boosterHere;
            extensiveNode = extensiveNode->parent;
        }
    }
    // 输出结果
    void output() const {
        cout << numberOfBoosters;
    }
    // 可视化
    void visual() {
        // 获取每个点的压力
        for (int i = 2; i <= numberOfVertex; ++i) {
            int theVertexNumber = sequence[i];
            if (boosterHereFinal[theVertexNumber]) {
                vertexOf[theVertexNumber].pressure = Pmax;
                continue;
            }
            vertexOf[theVertexNumber].pressure = 0;
            for (int j = 1; j < i; ++j) {
                for (vector<wEdge>::iterator e = vertexOf[sequence[j]].edges.begin(); e != vertexOf[sequence[j]].edges.end(); ++e)
                    if (e->to == theVertexNumber)
                        // 需要求出到达该点的最大压力
                        vertexOf[theVertexNumber].pressure = max(vertexOf[theVertexNumber].pressure, vertexOf[sequence[j]].pressure - e->weight);
            }
        }
        cout << "digraph g {\n";
        for (int i = 1; i <= numberOfVertex; ++i) {
            cout << i;
            if (boosterHereFinal[i])
                cout << "[label=\"node" << i << ": pre=" << vertexOf[i].pressure << "\",style=filled, fillcolor=red];\n";
            else
                cout << "[label=\"node" << i << ": pre=" << vertexOf[i].pressure << "\",style=filled, fillcolor=white];\n";
        }
        for (auto i = 1; i <= numberOfVertex; ++i) {
            for (auto& edge : vertexOf[i].edges) {
                cout << i << "->" << edge.to << "[label=\"cost=" << edge.weight << "\"];\n";
            }
        }
        cout << "}\n";
    }
};

#endif