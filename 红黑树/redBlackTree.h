#ifndef _RBTREE_H_
#define _RBTREE_H_
#define RED 0
#define BLACK 1
#include <iostream>
#include <stack>
using namespace std;

template <class K, class E>
struct RBNode {
	K key;
	E value;
	bool color;
	RBNode<K, E> *leftChild;
	RBNode<K, E> *rightChild;
	RBNode<K, E> *parent;
	RBNode(K theKey = 0, E theValue = 0, bool c = RED, RBNode<K, E> *p = nullptr): key(theKey), value(theValue), color(c), leftChild(nullptr), rightChild(nullptr), parent(p) {};
	~RBNode() {};
	bool operator>(const RBNode& _right) { return key < _right.key; };
	bool operator<(const RBNode& _right) { return key > _right.key; };
	bool operator==(const RBNode& _right) { return key == _right.key; };
	void swap(RBNode<K, E> &another);
};

template<class K, class E>
class RBTree {
public:
	RBTree(): _root(nullptr), _size(0) {};
	~RBTree() { RBTreefree(_root); };
	RBTree(const RBTree<K, E> &);
	RBTree<K, E>& operator=(const RBTree<K, E> &);
	RBNode<K, E> *find(const K &) const;
	void insert(const K &, const E &);
	void erase(const K &);
	RBNode<K, E> *root() const { return _root; }
	int size() const { return _size; };
	bool empty() const { return _size == 0; };
	int isRBTree();
	void inOrderTraverse();
private:
	RBNode<K, E> *_root;
	int _size;
	// 判断是否是红黑树, 返回黑节点一列的个数
	int isRBTree(RBNode<K, E> *);
	// 判断是否是二叉搜索树
	bool isBinarySearchTree(RBNode<K, E> *);
	// 左旋
	void leftRotate(RBNode<K, E> *);
	// 右旋
	void rightRotate(RBNode<K, E> *);
	// 新增点后的再平衡
	void insertReBalance(RBNode<K, E> *);
	// 传参带有两个黑色属性
	void eraseReBalance(RBNode<K, E> *, RBNode<K, E> *);
	// 释放二叉树
	void RBTreefree(RBNode<K, E> *);
	// 建立二叉树
	RBNode<K, E> *createTree(RBNode<K, E> *);
};

template <class K, class E>
RBTree<K, E>::RBTree(const RBTree<K, E> &theTree) {
	RBTreefree(_root);
	_root = createTree(theTree.root());
	_size = theTree.size();
}

template <class K, class E>
RBTree<K, E> &RBTree<K, E>::operator=(const RBTree<K, E> &theTree) {
	if (this != &theTree) {
		RBTreefree(_root);
		_root = createTree(theTree.root());
		_size = theTree.size();
	}
	return *this;
}

template<class K, class E>
RBNode<K, E> *RBTree<K, E>::find(const K &theKey) const {
	RBNode<K, E> *cur = _root;
	while (cur != nullptr) {
		if (cur->key == theKey)
			break;
		if (cur->key > theKey)
			cur = cur->leftChild;
		else 
			cur = cur->rightChild;
	}
	// 未找到自然返回nullptr
	return cur;
}

template<class K, class E>
void RBTree<K, E>::insert(const K &theKey, const E &theElement) {
	RBNode<K, E> *pre = nullptr;
	RBNode<K, E> *cur = _root;
	while (cur != nullptr) {
		pre = cur;
		if (cur->key > theKey)
			cur = cur->leftChild;
		else if (cur->key < theKey)
			cur = cur->rightChild;
		else
			return;
	}
	// 新插入节点
	++_size;
	RBNode<K, E> *newRBNode = new RBNode<K, E>(theKey, theElement);
	// 根节点
	if (pre == nullptr) {
		newRBNode->color = BLACK;
		_root = newRBNode;
		return;
	}
	if (pre->key > theKey)
		pre->leftChild = newRBNode;
	else
		pre->rightChild = newRBNode;
	newRBNode->parent = pre;
	insertReBalance(newRBNode);
}

template<class K, class E>
void RBTree<K, E>::erase(const K &theKey) {
	RBNode<K, E> *cur = _root;
	while (cur != nullptr) {
		if (cur->key == theKey)
			break;
		if (cur->key > theKey)
			cur = cur->leftChild;
		else
			cur = cur->rightChild;
	}
	if (cur == nullptr)
		return;
	--_size;
	if (cur->leftChild != nullptr && cur->rightChild != nullptr) {
		// 双子树, 寻找后继节点, 暂定右子树的最小节点
		RBNode<K, E> *successor = cur->rightChild;
		while (successor->leftChild != nullptr)
			successor = successor->leftChild;
		// 交换successor和cur
		cur->swap(*successor);
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
		RBNode<K, E> *chilchildode = (cur->leftChild == nullptr) ? cur->rightChild : cur->leftChild;
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

template<class K, class E>
int RBTree<K, E>::isRBTree() {
	if (isBinarySearchTree(_root))
		return isRBTree(_root);
	return 0;
}

template<class K, class E>
void RBTree<K, E>::inOrderTraverse() {
	if (_root == nullptr)
		return;
	stack<RBNode<K, E> *> s;
	RBNode<K, E> *cur = _root->leftChild;    // 指向当前要检查的节点    
	s.push(_root);
	while (cur != nullptr || !s.empty()) {
		while (cur != nullptr) {
			// 一直往左走
			s.push(cur);
			cur = cur->leftChild;
		}
		cur = s.top();
		s.pop();
		cout << "RBTree[" << cur->key << "] = " << cur->value << ",\n";
		cur = cur->rightChild;
	}
}

template<class K, class E>
int RBTree<K, E>::isRBTree(RBNode<K, E> *x) {
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

template<class K, class E>
bool RBTree<K, E>::isBinarySearchTree(RBNode<K, E> *x) {
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

template<class K, class E>
void RBTree<K, E>::leftRotate(RBNode<K, E> *x) {
	/*
	*	 px                              px
	*    /                               /
	*	x                               y
	*  / \      --(左旋)-->			   / \
	* lx   y                          x  ry
	*	  / \						 / \
	*    ly   ry                   lx  ly
	*/
	RBNode<K, E> *y = x->rightChild;
	if (y == nullptr)
		return;
	RBNode<K, E> *ly = y->leftChild;
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

template<class K, class E>
void RBTree<K, E>::rightRotate(RBNode<K, E> *x) {
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
	RBNode<K, E> *y = x->leftChild;
	if (y == nullptr)
		return;
	RBNode<K, E> *ry = y->rightChild;
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

template<class K, class E>
void RBTree<K, E>::insertReBalance(RBNode<K, E> *newRBNode) {
	RBNode<K, E> *x = newRBNode;
	while (x != _root && x->parent->color == RED) {
		// 当前节点不为根, 且父节点为红, 递归向上
		if (x->parent == x->parent->parent->leftChild) {
			// 不存在祖父节点怎么办?
			// 父节点为祖父节点之左子节点
			RBNode<K, E> *uncle = x->parent->parent->rightChild;
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
			}
			x->parent->color = BLACK;     			// 改变颜色  
			x->parent->parent->color = RED;
			rightRotate(x->parent->parent);    		// 右旋实现平衡 
		}
		else {
			// 父节点为祖父节点之右子节点
			RBNode<K, E> *uncle = x->parent->parent->leftChild;
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
			}
			x->parent->color = BLACK;     			// 改变颜色  
			x->parent->parent->color = RED;
			leftRotate(x->parent->parent);    		// 左旋实现平衡 
		}
	}
	_root->color = BLACK;
}

template<class K, class E>
void RBTree<K, E>::eraseReBalance(RBNode<K, E> *theNode, RBNode<K, E> *parent) {
	RBNode<K, E> *slibing;

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
			theNode = _root;
		}
	}
	// 将递归向上的P节点处理掉/处理_root
	if (theNode != nullptr)
		theNode->color = BLACK;
}

template<class K, class E>
void RBTree<K, E>::RBTreefree(RBNode<K, E> *x) {
	if (x == nullptr)
		return;
	int freeCount = 0;
	RBNode<K, E> *cur = x;
	RBNode<K, E> *temp = nullptr;
	stack<RBNode<K, E> *> s;
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

template <class K, class E>
RBNode<K, E> *RBTree<K, E>::createTree(RBNode<K, E> *root) {
	if (root == nullptr)
		return nullptr;
	RBNode<K, E> *theRoot = new RBNode<K, E>(root->key, root->value, root->color);
	theRoot->leftChild = createTree(root->leftChild);
	theRoot->rightChild = createTree(root->rightChild);
	return theRoot;
}

#endif