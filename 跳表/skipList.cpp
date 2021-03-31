#include "../exception.h"
#include "skipList.h"
#include <sstream>
#include <iomanip>

template <class T>
skipList<T>::skipList(T largeKey, int maxPairs, float prob) {
    // 设置各项参数
    cutOff = prob * RAND_MAX;
    maxLevel = (int)floor(logf((float)maxPairs) / logf(1/prob));
    curMaxLevel = 0;
	compareCount = 0;
    _size = 0;
    tailKey = largeKey;

    // 创建headerNode, tailNode, last
    headerNode = new skipNode<T>(INT_MIN, maxLevel + 1);
    tailNode = new skipNode<T>(tailKey, 0);
    last = new skipNode<T> *[maxLevel + 1];

    // 链表为空,任意级数链表头指向尾
	for (int i = 0; i <= maxLevel; i++)
		headerNode->next[i] = tailNode; 
}

template <class T>
skipList<T>::~skipList() {
	skipNode<T> *preNode = headerNode, *curNode = headerNode->next[0];
	while (curNode != tailNode) {
		delete preNode;
		preNode = curNode;
		curNode = curNode->next[0];
	}
	delete tailNode;
	delete[] last;
}

template <class T>
skipNode<T> *skipList<T>::find(const T &theKey) {
	// 查找对应关键字为theKey的数对,如果不存在返回NULL
	// 查找思路: 从最高级链表开始搜索, 直到0级链表
	compareCount = 0;
	if (theKey >= tailKey)
		return NULL;
	
	skipNode<T> *beforeNode = headerNode;
	for (int i = curMaxLevel; i >= 0; i--)
		// 比较次数需要将每一次比较就加上
		while (++compareCount && beforeNode->next[i]->element < theKey)
			beforeNode = beforeNode->next[i];

	// 检查下一个pair关键字是不是theKey
	if (beforeNode->next[0]->element == theKey)
		return beforeNode->next[0];
	
	return NULL;
}

template <class T>
void skipList<T>::erase(const T &theKey) {
	// 根据theKey关键字查找对应元素, 然后删除
	compareCount = 0;
	if (theKey >= tailKey)
		return;

	skipNode<T> *theNode = search(theKey);
	if (theNode->element != theKey)
		return;
	// 删除theKey对应的节点
	for (int i = 0; i <= curMaxLevel && last[i]->next[i] == theNode; i++)
		last[i]->next[i] = theNode->next[i];
	// 更新链表级数
	while (curMaxLevel >= 0 && headerNode->next[curMaxLevel] == tailNode)
		curMaxLevel--;
	
	delete theNode;
	_size--;
}

template <class T>
void skipList<T>::insert(const T &theElement) {
	compareCount = 0;
	// 插入theElement, 覆盖相同的pairNode
	if (theElement >= tailKey) {
		// 关键字太大, 抛出异常 
		ostringstream s;
		s << "Key = " << theElement << " Must be < " << tailKey;
		throw illegalParameterValue(s.str());
	}
	// test if it is existed
	skipNode<T> *theNode = search(theElement);
	// if (theNode->element == theElement)
		// return;
	
	// non-existed
	int theLevel = level();
	if (theLevel > curMaxLevel) {
		theLevel = ++curMaxLevel;
		last[theLevel] = headerNode;
	}
	
	skipNode<T> *newNode = new skipNode<T>(theElement, maxLevel + 1);
	for (int i = 0; i <= theLevel; i++) {
		// 设置newNode的next数组
		// 更新last数组
		newNode->next[i] = last[i]->next[i];
		last[i]->next[i] = newNode;
	}
	
	_size++;
}

template <class T>
int skipList<T>::removeMin() {
	compareCount = 0;
	// 当前没有节点
	if (_size == 0)
		return -1;
    int theValue = headerNode->next[0]->element;
    erase(theValue);
    return theValue;
}

template <class T>
int skipList<T>::removeMax() {
	compareCount = 0;
	// 当前没有节点
	if (_size == 0)
		return -1;
	// 找最大，即为寻找小于tailKey的最大节点
	// search
	skipNode<T> *beforeNode = headerNode, *curNode = headerNode;
	for (int i = curMaxLevel; i >= 0; i--) {
		while (++compareCount && curNode->next[i]->element < tailKey)
			curNode = curNode->next[i];
		while (++compareCount && beforeNode->next[i]->element < tailKey && beforeNode->next[i]->next[i]->element < tailKey)
			beforeNode = beforeNode->next[i];
		last[i] = beforeNode;
	}
	// erase
	int theValue = curNode->element;
	
	for (int i = 0; i <= curMaxLevel && last[i]->next[i] == curNode; i++)
		last[i]->next[i] = curNode->next[i];
	while (curMaxLevel >= 0 && headerNode->next[curMaxLevel] == tailNode)
		curMaxLevel--;
	
	delete curNode;
	_size--;
    
	return theValue;
}

template <class T>
int skipList<T>::xorValue() {
    int res = 0;
    skipNode<T> *curNode = headerNode->next[0];

    while (curNode != tailNode) {
        res ^= curNode->element;
        curNode = curNode->next[0];
    }

    return res;
}

template <class T>
void skipList<T>::restruct() {
	curMaxLevel = _size <= 1 ? 0 : (int) floor(log2(_size));

	for (int i = 1; i <= curMaxLevel; i++) {
		// 从第1层开始，利用前一层的数据，每隔一个节点接一个节点
		skipNode<T> *curNode = headerNode;
		while (curNode != tailNode && curNode->next[i - 1] != tailNode) {
			curNode->next[i] = curNode->next[i - 1]->next[i - 1];
			curNode = curNode->next[i];
		}
		// 每一层都要对最后一个节点做判断，然后将其与tailNode连接起来
		if (curNode != tailNode)
			curNode->next[i] = tailNode;
	}
	// 更新暂未用到的更高层
	for (int i = curMaxLevel + 1; i <= maxLevel; i++)
		headerNode->next[i] = tailNode;
	cout << "# restruct : done!" << endl;
}

template <class T>
void skipList<T>::display() const {
	// 第0层正常输出
	cout << 1 << "~>";
	skipNode<T> * curNode = headerNode->next[0];
	while (curNode != tailNode) {
		cout << setw(6) << curNode->element << "~>";
		curNode = curNode->next[0];
	}
	cout << "~>#end" << endl;
	// 从第1层开始有变化
	for (int i = 1; i <= curMaxLevel; i++) {
		// 头标识符
		cout << i + 1 << "~>";
		// 遍历所有节点
		curNode = headerNode->next[i];
		skipNode<T> *tempNode = headerNode;
		while (curNode != tailNode) {
			while (curNode != tempNode->next[0]) {
				cout << "~~~~~~~~";
				tempNode = tempNode->next[0];
			}
			cout << setw(6) << curNode->element << "~>";
			tempNode = curNode;
			curNode = curNode->next[i];
		}
		// 尾节点
		while (tempNode->next[0] != tailNode) {
			cout << "~~~~~~~~";
			tempNode = tempNode->next[0];
		}
		cout << "~>#end" << endl; 
	}
}

template <class T>
int skipList<T>::level() const {
	// 返回一个链表级数，级数不大于maxLevel
	int lev = 0;
	while (rand() <= cutOff)
		lev++;
	return (lev <= maxLevel) ? lev : maxLevel;
}

template <class T>
skipNode<T> *skipList<T>::search(const T &theKey) {
	// 搜索,把每一级链表搜索遇到的最后一个节点指针用last[i]存起来
	// 返回包含theKey的节点
	// beforeNode是关键字为theKey的节点前最右边的节点 
	skipNode<T> *beforeNode = headerNode;
	for (int i = curMaxLevel; i >= 0; i--) {
	 	while (++compareCount && beforeNode->next[i]->element < theKey)
	 		beforeNode = beforeNode->next[i];
	 	last[i] = beforeNode;
	}
	return beforeNode->next[0];
}