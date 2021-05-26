#ifndef _RBTREE_H_
#define _RBTREE_H_
#define RED 0
#define BLACK 1
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stack>
using namespace std;

template <class K, class V>
struct RBNode {
	K key;
	V value;
	bool color;
	RBNode<K, V> *leftChild;
	RBNode<K, V> *rightChild;
	RBNode<K, V> *parent;
	RBNode(K theKey = 0, V theValue = 0, bool c = RED, RBNode<K, V> *p = nullptr): key(theKey), value(theValue), color(c), leftChild(nullptr), rightChild(nullptr), parent(p) {};
	~RBNode() {};
	bool operator>(const RBNode& _right) { return key < _right.key; };
	bool operator<(const RBNode& _right) { return key > _right.key; };
	bool operator==(const RBNode& _right) { return key == _right.key; };
	void swap(RBNode<K, V> *another) {
		K k1 = another->key;
		V e1 = another->value;
		another->key = key;
		another->value = value;
		key = k1;
		value = e1;
	};
};
/*
template<class K, class V>
class RBTreeIterator {
public:
	RBTreeIterator(RBNode<K, V> *pNode = nullptr): _pNode(pNode) {};
	RBTreeIterator(const RBTreeIterator<K, V>& t): _pNode(t._pNode) {};
	pair<K, V> &operator*() { return pair<K, V>(_pNode->key, _pNode->value); }
	pair<K, V> *operator->() { return &(operator*()); }
	RBTreeIterator<K, V> &operator++() {
		RBTreeIteratorIncrement();
		return *this;
	}
	RBTreeIterator<K, V> operator++(int) {
		RBTreeIterator<K, V> temp(*this);
		RBTreeIteratorIncrement();
		return temp;
	}
	RBTreeIterator<K, V>& operator--() {
		RBTreeIteratorDecrement();
		return *this;
	}
	RBTreeIterator<K, V> operator--(int) {
		RBTreeIterator<K, V> temp(*this);
		RBTreeIteratorDecrement();
		return temp;
	}
	bool operator==(const RBTreeIterator<K, V> &t) {
		return _pNode == t._pNode;
	}
	bool operator!=(const RBTreeIterator<K, V> &t) {
		return _pNode != t._pNode;
	}
private:
	void RBTreeIteratorIncrement() {
		if (_pNode->rightChild) {
			_pNode = _pNode->rightChild;
			while (_pNode->leftChild)
				_pNode = _pNode->leftChild;
		}
		else {
			RBNode<K, V> pParent = _pNode->parent;
			while (pParent != nullptr && pParent->rightChild == _pNode) {
				_pNode = pParent;
				pParent = _pNode->parent;
			}
			_pNode = pParent;
		}
	}
	void RBTreeIteratorDecrement() {
		if (_pNode->leftChild) {
			_pNode = _pNode->leftChild;
			while (_pNode->rightChild)
				_pNode = _pNode->rightChild;
		}
		else {
			RBNode<K, V> pParent = _pNode->parent;
			while (pParent != nullptr && pParent->leftChild == _pNode) {
				_pNode = pParent;
				pParent = _pNode->parent;
			}
			if (pParent)
				_pNode = pParent;
		}
	}
private:
	RBNode<K, V> *_pNode;
};
*/
template<class K, class V>
class RBTree {
public:
	RBTree(RBNode<K, V> *root = nullptr, int size = 0): _root(root), _size(size), compareCount(0), rotateCount(0) {};
	~RBTree() { RBTreefree(_root); };
	RBTree(const RBTree<K, V> &);
	RBTree<K, V>& operator=(const RBTree<K, V> &);
	// RBTreeIterator<K, V> begin() { return RBTreeIterator<K, V>(_root); };
	// RBTreeIterator<K, V> end() { return RBTreeIterator<K, V>(nullptr); };
	RBNode<K, V> *find(const K &);
	pair<RBNode<K, V> *, bool> insert(const K &, const V &);
	void erase(const K &);
	RBNode<K, V> *root() const { return _root; }
	int size() const { return _size; };
	bool empty() const { return _size == 0; };
	int isRBTree() const;
	// 红黑树的合并
	static RBTree<K, V> *merge(RBTree<K, V> *, RBTree<K, V> *);
	void visual(string name);
	// 中序遍历-升序
	void inOrderTraverse() const;	
	int numberOfRotation() const { return rotateCount; };
	int numberOfComparasion() const { return compareCount; }
private:
	RBNode<K, V> *_root;
	int _size;
	int rotateCount;
	int compareCount;
	ofstream outfile;
	// 判断是否是红黑树, 返回黑节点一列的个数
	int isRBTree(RBNode<K, V> *) const;
	// 判断是否是二叉搜索树
	bool isBinarySearchTree(RBNode<K, V> *) const;
	// 左旋
	void leftRotate(RBNode<K, V> *);
	// 右旋
	void rightRotate(RBNode<K, V> *);
	// 新增点后的再平衡
	void insertReBalance(RBNode<K, V> *);
	// 传参带有两个黑色属性
	void eraseReBalance(RBNode<K, V> *, RBNode<K, V> *);
	// 释放二叉树
	void RBTreefree(RBNode<K, V> *);
	// 建立二叉树
	static RBNode<K, V> *createTree(RBNode<K, V> *);
	// 基于有序数组建立二叉树
	static RBNode<K, V> *createTree(pair<K, V> *, int, int, bool, RBNode<K, V> *);
	// 可视化树
	void printTree(RBNode<K, V> *);
	// 可视化节点
	void printNode(RBNode<K, V> *);
	// 根据节点数获取树高度
	static int height(int);
};

template <class K, class V>
RBTree<K, V>::RBTree(const RBTree<K, V> &theTree) {
	RBTreefree(_root);
	_root = createTree(theTree.root());
	_size = theTree.size();
}

template <class K, class V>
RBTree<K, V> &RBTree<K, V>::operator=(const RBTree<K, V> &theTree) {
	if (this != &theTree) {
		RBTreefree(_root);
		_root = createTree(theTree.root());
		_size = theTree.size();
	}
	return *this;
}

template <class K, class V>
RBNode<K, V> *RBTree<K, V>::find(const K &theKey) {
	compareCount = 0;
	RBNode<K, V> *cur = _root;
	while (cur != nullptr) {
		if (cur->key == theKey)
			break;
		if (cur->key > theKey)
			cur = cur->leftChild;
		else 
			cur = cur->rightChild;
		++compareCount;
	}
	// 未找到自然返回nullptr
	return cur;
}

template <class K, class V>
pair<RBNode<K, V> *, bool> RBTree<K, V>::insert(const K &theKey, const V &theValue) {
	compareCount = 0;
	RBNode<K, V> *pre = nullptr;
	RBNode<K, V> *cur = _root;
	while (cur != nullptr) {
		pre = cur;
		if (cur->key > theKey)
			cur = cur->leftChild;
		else if (cur->key < theKey)
			cur = cur->rightChild;
		else
			return pair<RBNode<K, V> *, bool>(cur, false);
		++compareCount;
	}
	// 新插入节点
	++_size;
	RBNode<K, V> *newRBNode = new RBNode<K, V>(theKey, theValue);
	// 根节点
	if (pre == nullptr) {
		newRBNode->color = BLACK;
		_root = newRBNode;
		return pair<RBNode<K, V> *, bool>(_root, true);
	}
	if (pre->key > theKey)
		pre->leftChild = newRBNode;
	else
		pre->rightChild = newRBNode;
	newRBNode->parent = pre;
	insertReBalance(newRBNode);
	return pair<RBNode<K, V> *, bool>(newRBNode, true);
}

template <class K, class V>
void RBTree<K, V>::erase(const K &theKey) {
	compareCount = 0;
	RBNode<K, V> *cur = _root;
	while (cur != nullptr) {
		if (cur->key == theKey)
			break;
		if (cur->key > theKey)
			cur = cur->leftChild;
		else
			cur = cur->rightChild;
		++compareCount;
	}
	if (cur == nullptr)
		return;
	--_size;
	if (cur->leftChild != nullptr && cur->rightChild != nullptr) {
		// 双子树, 寻找后继节点, 暂定右子树的最小节点
		RBNode<K, V> *successor = cur->rightChild;
		while (successor->leftChild != nullptr) {
			successor = successor->leftChild;
			++compareCount;
		}
		// 交换successor和cur
		cur->swap(successor);
		cur = successor;
	}
	// 处理过后只剩两种情况
	if (cur->leftChild == nullptr && cur->rightChild == nullptr) {
		// 无子树情况
		if (cur == _root) {
			// 根节点特别处理
			_root = nullptr;
			delete cur;
			cur = nullptr;
			return;
		}
		if (cur->parent->leftChild == cur)
			cur->parent->leftChild = nullptr;
		else
			cur->parent->rightChild = nullptr;
		// 黑节点平衡
		if (cur->color == BLACK)
			eraseReBalance(nullptr, cur->parent);
		delete cur;
		cur = nullptr;
		return;
	}
	else {
		// 单子树情况
		RBNode<K, V> *chilchildode = (cur->leftChild == nullptr) ? cur->rightChild : cur->leftChild;
		if (cur == _root) {
			// 根节点
			_root = chilchildode;
			_root->color = BLACK;
			delete cur;
			cur = nullptr;
			return;
		}
		if (cur->parent->leftChild == cur)
			cur->parent->leftChild = chilchildode;
		else
			cur->parent->rightChild = chilchildode;
		chilchildode->parent = cur->parent;
		eraseReBalance(chilchildode, cur->parent);
	}
}

template <class K, class V>
int RBTree<K, V>::isRBTree() const{
	if (isBinarySearchTree(_root))
		return isRBTree(_root);
	return 0;
}

template <class K, class V>
RBTree<K, V> *RBTree<K, V>::merge(RBTree<K, V> *t1, RBTree<K, V> *t2) {
	if (t1->empty() && t2->empty())
		return nullptr;
	if (t1->empty())
		return t2;
	if (t2->empty())
		return t1;
	RBNode<K, V> *r1 = t1->root(), *r2 = t2->root();
	pair<K, V> rb1[t1->size() + 1], rb2[t2->size() + 1];
	pair<K, V> nodes[t1->size() + t2->size() + 1];
	stack<RBNode<K, V> *> s;
	int n = 0;
	// 将节点有序存到数组里, 方便归并
	s.push(r1);
	r1 = r1->leftChild;
	while (r1 != nullptr || !s.empty()) {
		while (r1 != nullptr) {
			s.push(r1);
			r1 = r1->leftChild;
		}
		r1 = s.top();
		s.pop();
		rb1[++n] = pair<K, V>(r1->key, r1->value);
		r1 = r1->rightChild;
	}
	n = 0;
	s.push(r2);
	r2 = r2->leftChild;
	while (r2 != nullptr || !s.empty()) {
		while (r2 != nullptr) {
			s.push(r2);
			r2 = r2->leftChild;
		}
		r2 = s.top();
		s.pop();
		rb2[++n] = pair<K, V>(r2->key, r2->value);
		r2 = r2->rightChild;
	}
	n = 0;
	// 归并到新数组
	int size_1 = t1->size(), size_2 = t2->size();
	int i = 1, j = 1;
	while (i <= size_1 && j <= size_2) {
		if (rb1[i].first == rb2[j].first) {
			nodes[++n] = pair<K, V>(rb1[i].first, rb1[i].second + rb2[j].second);
			++i;
			++j;
		} else if (rb1[i].first < rb2[j].first) {
			nodes[++n] = rb1[i];
			++i;
		} else {
			nodes[++n] = rb2[j];
			++j;
		}
	}
	while (i <= size_1)
		nodes[++n] = rb1[i++];
	while (j <= size_2)
		nodes[++n] = rb2[j++];
	t1->~RBTree();
	t2->~RBTree();
	// 奇数高度从红色开始涂
	bool color = ( (RBTree<K, V>::height(n) & 1) == 0 ) ? BLACK : RED;
	return new RBTree<K, V>(createTree(nodes, 1, n, color, nullptr), n);
}

template <class K, class V>
void RBTree<K, V>::visual(string name) {
	outfile.open("红黑树/data/" + name + ".dot");
	outfile << "digraph g {\nsplines=false;\n";
	printTree(_root);
	outfile << "}\n";
	outfile.close();
	string todo = "dot -Tjpg 红黑树/data/" + name + ".dot -o 红黑树/data/" + name + ".jpg";
	system(todo.c_str());
}

template <class K, class V>
void RBTree<K, V>::inOrderTraverse() const {
	if (_root == nullptr)
		return;
	stack<RBNode<K, V> *> s;
	RBNode<K, V> *cur = _root->leftChild;    // 指向当前要检查的节点    
	s.push(_root);
	while (cur != nullptr || !s.empty()) {
		while (cur != nullptr) {
			// 一直往左走
			s.push(cur);
			cur = cur->leftChild;
		}
		cur = s.top();
		s.pop();
		cout << "RBTree[" << cur->key << "]: " << cur->value << endl;
		cur = cur->rightChild;
	}
}

template <class K, class V>
int RBTree<K, V>::isRBTree(RBNode<K, V> *x) const {
	// 空节点当做黑节点
	if (x == nullptr)
		return 1;
	if (x->color == RED) {
		if (x->leftChild != nullptr && x->leftChild->color == RED)
			return 0;
		if (x->rightChild != nullptr && x->rightChild->color == RED)
			return 0;
	}
	int left = isRBTree(x->leftChild);
	int right = isRBTree(x->rightChild);
	// 返回0说明出现了错误, 违反了红黑树性质
	if (left == 0 || right == 0 || left != right)
		return 0;
	if (x->color == RED)
		return left;
	else
		return left + 1;
}

template <class K, class V>
bool RBTree<K, V>::isBinarySearchTree(RBNode<K, V> *x) const {
	if (x == nullptr)
		return true;
	if (x->leftChild != nullptr && x->rightChild != nullptr)
		return (x->leftChild->key < x->key) && (x->rightChild->key > x->key) && isBinarySearchTree(x->leftChild) && isBinarySearchTree(x->rightChild);
	if (x->leftChild != nullptr && x->rightChild == nullptr)
		return (x->leftChild->key < x->key) && isBinarySearchTree(x->leftChild);
	if (x->leftChild == nullptr && x->rightChild != nullptr)
		return (x->rightChild->key > x->key) && isBinarySearchTree(x->rightChild);
	// 叶子节点
	return true;
}

template <class K, class V>
void RBTree<K, V>::leftRotate(RBNode<K, V> *x) {
	/*
	*	 px                              px
	*    /                               /
	*	x                               y
	*  / \      --(左旋)-->			   / \
	* lx   y                          x  ry
	*	  / \						 / \
	*    ly   ry                   lx  ly
	*/
	RBNode<K, V> *y = x->rightChild;
	if (y == nullptr)
		return;
	RBNode<K, V> *ly = y->leftChild;
	y->parent = x->parent;
	if (y->parent == nullptr)
		_root = y;
	else {
		if (y->parent->leftChild == x)
			y->parent->leftChild = y;
		else
			y->parent->rightChild = y;
	}
	x->parent = y;
	y->leftChild = x;
	x->rightChild = ly;
	if (ly != nullptr)
		ly->parent = x;
}

template <class K, class V>
void RBTree<K, V>::rightRotate(RBNode<K, V> *x) {
	/*
	*	 	 px                           px
	*    	 /                            /
	*		x                            y
	*  	   / \      --(左旋)-->			/  \
	* 	  y   rx                      ly    x
	*	 / \						       / \
	*  ly   ry                            ry  rx
	*/
	// 左节点无，则无必要右旋
	RBNode<K, V> *y = x->leftChild;
	if (y == nullptr)
		return;
	RBNode<K, V> *ry = y->rightChild;
	y->parent = x->parent;
	if (y->parent == nullptr)
		_root = y;
	else {
		if (y->parent->leftChild == x)
			y->parent->leftChild = y;
		else
			y->parent->rightChild = y;
	}
	x->parent = y;
	y->rightChild = x;
	x->leftChild = ry;
	if (ry != nullptr)
		ry->parent = x;
}

template <class K, class V>
void RBTree<K, V>::insertReBalance(RBNode<K, V> *newRBNode) {
	rotateCount = 0;
	RBNode<K, V> *x = newRBNode;
	while (x != _root && x->parent->color == RED) {
		// 当前节点不为根, 且父节点为红, 递归向上
		if (x->parent == x->parent->parent->leftChild) {
			// 不存在祖父节点怎么办?
			// 父节点为祖父节点之左子节点
			RBNode<K, V> *uncle = x->parent->parent->rightChild;
			if (uncle != nullptr && uncle->color == RED) {
				// 叔叔节点存在，且为红
				x->parent->color = BLACK;			// 更改父节点为黑色  
				uncle->color = BLACK;            	// 更改叔叔节点为黑色  
				x->parent->parent->color = RED;		// 更改祖父节点为红色 
				x = x->parent->parent;				// 将祖父设为当前节点
				continue;							// 递归向上
			}
			// 叔叔节点不存在或为黑色, 需要旋转, 单纯右旋或者先左旋后右旋
			if (x == x->parent->rightChild) {
				// 如果新节点为父节点之右子节点, 先左旋后右旋  
				x = x->parent;
				leftRotate(x);   					// 左旋调整
				++rotateCount;
			}
			x->parent->color = BLACK;     			// 改变颜色  
			x->parent->parent->color = RED;
			rightRotate(x->parent->parent);    		// 右旋实现平衡 
			++rotateCount;
		}
		else {
			// 父节点为祖父节点之右子节点
			RBNode<K, V> *uncle = x->parent->parent->leftChild;
			if (uncle != nullptr && uncle->color == RED) {
				// 叔叔节点存在, 且为红  
				x->parent->color = BLACK;           // 更改父节点为黑色  
				uncle->color = BLACK;               // 更改伯父节点为黑色  
				x->parent->parent->color = RED;     // 更改祖父节点为红色  
				x = x->parent->parent;          	// 准备继续往上层检查 
				continue;
			}
			if (x == x->parent->leftChild) {
				// 如果新节点为父节点之左子节点
				x = x->parent;
				rightRotate(x);    					// 右旋调整
				++rotateCount;
			}
			x->parent->color = BLACK;     			// 改变颜色  
			x->parent->parent->color = RED;
			leftRotate(x->parent->parent);    		// 左旋实现平衡 
			++rotateCount;
		}
	}
	_root->color = BLACK;
}

template <class K, class V>
void RBTree<K, V>::eraseReBalance(RBNode<K, V> *theNode, RBNode<K, V> *parent) {
	rotateCount = 0;
	RBNode<K, V> *slibing;
	while ((theNode == nullptr || theNode->color == BLACK) && theNode != _root) {
		// 循环, 直到theNode = _root或者theNode->color = RED
		if (parent->leftChild == theNode) {
			// 删除的节点位置为左儿子的情况
			slibing = parent->rightChild;
			// case1: S为红, S为右子, 以P左旋, 转换为S为黑的情况
			if (slibing->color == RED) {
				parent->color = RED;
				slibing->color = BLACK;
				leftRotate(parent);
				++rotateCount;
				slibing = parent->rightChild;
			}
			// case2: S为黑, SL, SR全黑, 将S涂红, 使S树符合条件, 然后看P 
			if ((slibing->leftChild == nullptr || slibing->leftChild->color == BLACK) &&
				(slibing->rightChild == nullptr || slibing->rightChild->color == BLACK)) {
				// S涂红
				slibing->color = RED;
				// 再看P是否符合条件
				theNode = parent;
				parent = theNode->parent;
				continue;
			}
			// case3: S为黑, SL为红
			if (slibing->leftChild != nullptr && slibing->leftChild->color == RED) {
				slibing->leftChild->color = BLACK;
				slibing->color = RED;
				rightRotate(slibing);				// 右旋调整
				++rotateCount;
				// 更新S
				slibing = parent->rightChild;
			}
			// case4: SR为红
			// 交换S和P的颜色
			slibing->color = parent->color;
			parent->color = BLACK;
			// 右孩子涂黑, 然后旋转
			slibing->rightChild->color = BLACK;
			leftRotate(parent);						// 左旋平衡
			++rotateCount;
			theNode = _root;
		}
		else {
			// 删除的节点位置为右孩子的情况
			slibing = parent->leftChild;
			// case1: S为红, S为左子, 以P右旋, 转换为S为黑的情况
			if (slibing->color == RED) {
				parent->color = RED;
				slibing->color = BLACK;
				rightRotate(parent);
				++rotateCount;
				slibing = parent->leftChild;
			}
			// case2: S为黑, SL, SR全黑, 将S涂红, 使S树符合条件, 然后看P 
			if ((slibing->leftChild == nullptr || slibing->leftChild->color == BLACK) &&
				(slibing->rightChild == nullptr || slibing->rightChild->color == BLACK)) {
				// S涂红
				slibing->color = RED;
				// 再看P是否符合条件, 递归处理P
				theNode = parent;
				parent = theNode->parent;
				continue;
			}
			// case3: S为黑, SR为红
			if (slibing->rightChild != nullptr && slibing->rightChild->color == RED) {
				slibing->rightChild->color = BLACK;
				slibing->color = RED;
				leftRotate(slibing);				// 左旋调整到SL为红
				++rotateCount;
				// 更新S
				slibing = parent->leftChild;
			}
			// case4: SL为红
			// 交换S和P的颜色
			slibing->color = parent->color;
			parent->color = BLACK;
			// 左孩子涂黑, 然后旋转
			slibing->leftChild->color = BLACK;
			rightRotate(parent);					// 右旋平衡
			++rotateCount;
			theNode = _root;
		}
	}
	// 将递归向上的P节点处理掉/处理_root
	if (theNode != nullptr)
		theNode->color = BLACK;
}

template <class K, class V>
void RBTree<K, V>::RBTreefree(RBNode<K, V> *x) {
	if (x == nullptr)
		return;
	int freeCount = 0;
	RBNode<K, V> *cur = x;
	RBNode<K, V> *temp = nullptr;
	stack<RBNode<K, V> *> s;
	while (!s.empty() || cur) {
		if (cur) {
			s.push(cur);
			cur = cur->leftChild;
		}
		else {
			cur = s.top();
			s.pop();
			temp = cur;
			cur = cur->rightChild;
			delete temp;
			++freeCount;
		}
	}
	_size -= freeCount;
	x = nullptr;
}

template <class K, class V>
RBNode<K, V> *RBTree<K, V>::createTree(RBNode<K, V> *root) {
	if (root == nullptr)
		return nullptr;
	RBNode<K, V> *theRoot = new RBNode<K, V>(root->key, root->value, root->color);
	theRoot->leftChild = createTree(root->leftChild);
	theRoot->rightChild = createTree(root->rightChild);
	return theRoot;
}

template <class K, class V>
RBNode<K, V> *RBTree<K, V>::createTree(pair<K, V> *nodes, int start, int end, bool color, RBNode<K, V> *parent) {
	if (start > end)
		return nullptr;
	int mid = (start + end) / 2;
	RBNode<K, V> *root = new RBNode<K, V>(nodes[mid].first, nodes[mid].second, color, parent);
	root->leftChild = createTree(nodes, start, mid - 1, !color, root);
	root->rightChild = createTree(nodes, mid + 1, end, !color, root);
	return root;
}

template <class K, class V>
void RBTree<K, V>::printTree(RBNode<K, V> *root) {
	if (root == nullptr)
		return;
	if (root == _root)
		printNode(root);
	if (root->leftChild) {
		printNode(root->leftChild);
		outfile << root->key << ":f0:sw->" << root->leftChild->key << ":f1;\n";
	}
	if (root->rightChild) {
		printNode(root->rightChild);
		outfile << root->key << ":f2:se->" << root->rightChild->key << ":f1;\n";
	}
	printTree(root->leftChild);
	printTree(root->rightChild);
}

template <class K, class V>
void RBTree<K, V>::printNode(RBNode<K, V> *x) {
	if (x->color == BLACK)
		outfile << "node[shape=record, style=filled, fillcolor=black, fontcolor=white];\n";
	else
		outfile << "node[shape=record, style=filled, fillcolor=red, fontcolor=white];\n";
	outfile << x->key << "[label=\"<f0> | <f1> " << x->key << " | <f2> \"];\n";
}

template <class K, class V>
int RBTree<K, V>::height(int size) {
	int n = size, h = 0;
	while (n != 0) {
		++h;
		n >>= 1;
	}
	return h;
}

#endif