#ifndef _LINKED_W_DI_GRAPH_H_
#define _LINKED_W_DI_GRAPH_H_

#include "../exception.h"
#include <sstream>
#include <list>

// 有权边
template <class T>
struct wEdge {
   int vertex;     // 目的点
   T weight;       // 权值 - 消耗
   wEdge(int theVertex = 0, T theWeight = 0): vertex(theVertex), weight(theWeight) {}
   operator int() const { return vertex; }
};

// 重载 << 
template <class T>
ostream &operator<<(ostream &out, const wEdge<T> &x) {
   out << x.vertex << " " << x.weight << " ";
   return out;
}

// 邻接链表
template <class T>
struct graphChain {
   list<T> chain;
   bool existsEdge(int v) {
      if (find(chain.begin(), chain.end(), v) == chain.end())
         return false;
      return true;
   }
   T *erase(const T &theNode) {
      // 找到了, 删除, 返回指向下一个元素的指针
      for (typename list<T>::iterator iter = chain.begin(); iter != chain.end(); ++iter)
         if (*iter == theNode)
            return chain.erase(iter);
      // 未找到, 返回nullptr
      return nullptr;
   }
   void output(ostream &out) const {
      for (typename list<T>::iterator iter = chain.begin(); iter != chain.end(); ++iter)
         out << *iter << ' ';
   }
};

// 重载 << 
template <class T>
ostream &operator<<(ostream &out, const graphChain<T> &theChain) {
   theChain.output(out);
   return out;
}

// 加权有向图 - 邻接链表
template <class T>
class linkedWDigraph {
public:
   // 构造函数
   linkedWDigraph(int numberOfVertices = 0): n(numberOfVertices), e(0) {
      if (numberOfVertices < 0)
         throw illegalParameterValue("number of vertices must be >= 0");
      // 生成n个点对应的邻接链表
      aList = new list<wEdge<T> > [n + 1];
   }
   // 析构函数
   ~linkedWDigraph() { delete [] aList; }
   // 简单的返回值函数
   int numberOfVertices() const {return n;}
   int numberOfEdges() const {return e;}
   bool isDirected() const {return true;}
   bool isWeighted() const {return true;}
   // 检查点i到j的边是否存在
   bool existsEdge(int i, int j) const;
   // 插入新边
   void insertEdge(int u, int v, T theWeight);
   // 删除边(i, j)
   void eraseEdge(int i, int j);
   // 检查端点是否合法
   void checkVertex(int theVertex) const;
   // 出度
   int outDegree(int theVertex) const;
   // 入度
   int inDegree(int theVertex) const;
   // 输出
   void output(ostream& out) const;
protected:
   int n;                         // number of vertices
   int e;                         // number of edges
   graphChain<wEdge<T> > *aList;  // adjacency lists
};

// 检查点i到j的边是否存在
template <class T>
bool linkedWDigraph<T>::existsEdge(int i, int j) const {
   if (i < 1 || j < 1 || i > n || j > n || !aList[i].existsEdge(wEdge<T>(j)))
      return false;
   else
      return true;
}

// 插入新边
template <class T>
void linkedWDigraph<T>::insertEdge(int u, int v, T theWeight) {
   if (u < 1 || v < 1 || u > n || v > n || u == v) {
      ostringstream s;
      s << "(" << u << "," << v << ") is not a permissible edge";
      throw illegalParameterValue(s.str());
   }
   // 判断新边 - 是则插入
   if (!aList[u].existsEdge(wEdge<T>(v))) {
      // 尾插 / 头插 ?
      aList[u].emplace_back(v, theWeight);
      e++;
   }
}

// 删除边
template <class T>
void linkedWDigraph<T>::eraseEdge(int i, int j) {
   if (i >= 1 && j >= 1 && i <= n && j <= n) {
      wEdge<T> *v = aList[i].erase(wEdge<T>(j));
      if (v != nullptr)  // 边(u,v)确实存在
         e--;
   }
}

// 检查端点是否合法
template <class T>
void linkedWDigraph<T>::checkVertex(int theVertex) const {
   if (theVertex < 1 || theVertex > n) {
      ostringstream s;
      s << "no vertex " << theVertex;
      throw illegalParameterValue(s.str());
   }
}

// 出度
template <class T>
int linkedWDigraph<T>::outDegree(int theVertex) const {
   checkVertex(theVertex);
   return aList[theVertex].size();
}

// 入度
template <class T>
int linkedWDigraph<T>::inDegree(int theVertex) const {
   checkVertex(theVertex);

   // 遍历整个邻接链表
   int sum = 0;
   for (int i = 1; i <= n; i++)
      if (aList[i].existsEdge(wEdge<T>(theVertex)))
         sum++;

   return sum;
}

// 输出
template <class T>
void linkedWDigraph<T>::output(ostream &out) const {
   for (int i = 1; i <= n; i++)
      out << aList[i] << endl;
}
// overload <<
template <class T>
ostream &operator<<(ostream &out, const linkedWDigraph<T> &x) {
   x.output(out);
   return out;
}

#endif