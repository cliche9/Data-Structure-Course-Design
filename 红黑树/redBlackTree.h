#ifndef _RBTREE_H_
#define _RBTREE_H_
#define RED 0
#define BLACK 1
//必须实现<,>,==,拷贝构造，赋值函数
#include<stack>

template <class K, class E>
struct RBNode {
	K key;
	E value;
	bool color;
	RBNode<K, E> *leftChild;
	RBNode<K, E> *rightChild;
	RBNode<K, E> *parent;
	RBNode(K theKey = 0, E theValue = 0, RBNode<K, E> *p = nullptr): key(theKey), value(theValue), color(RED), leftChild(nullptr), rightChild(nullptr), parent(p) {};
	~RBNode() {};
	bool operator>(const RBNode& _right) { return key < _right.key; };
	bool operator<(const RBNode& _right) { return key > _right.key; };
	bool operator==(const RBNode& _right) { return key == _right.key; };
	void swap(RBNode<K, E> &another);
};

template<class K, class E>
class RBTree {
public:
	RBTree(): root(nullptr), _size(0) {};
	~RBTree() { RBTreefree(root); };
	RBTree(const RBTree &);
	RBTree& operator=(const RBTree &);
	RBNode<K, E> *find(const K &) const;
	void insert(const K &, const E &);
	void erase(const K &);
	int size() const { return _size; };
	bool empty() const { return _size == 0; };
	int isRBTree();
private:
	RBNode<K, E> *root;
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
	// 中序遍历
	void InOrderTraverse();
	// 释放二叉树
	void RBTreefree(RBNode<K, E> *);
};

template<class K, class E>
RBNode<K, E> *RBTree<K, E>::find(const K &theKey) const {
	RBNode<K, E> *cur = root;
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
	RBNode<K, E> *cur = root;
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
		root = newRBNode;
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
	RBNode<K, E> *cur = root;
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
		if (cur == root) {
			// 根节点特别处理
			root = nullptr;
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
		RBNode<K, E> *childNode = (cur->leftChild == nullptr) ? cur->rightChild : cur->leftChild;
		if (cur == root) {
			// 根节点
			root = childNode;
			root->color = BLACK;
			delete cur;
			cur = nullptr;
			return;
		}
		if (cur->parent->leftChild == cur)
			cur->parent->leftChild = childNode;
		else
			cur->parent->rightChild = childNode;
		childNode->parent = cur->parent;
		eraseReBalance(childNode, cur->parent);
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
		root = y;
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
		root = y;
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
int RBTree<K, E>::isRBTree() {
	if (isBinarySearchTree(root))
		return isRBTree(root);
	return 0;
}

template<class K, E>
inline void RBTree<K, E>::insertReBalance(RBNode<K, E>* newRBNode)
{
	RBNode<K, E>* x = newRBNode;
	while (x != root && x->parent->color == RED) {//当前节点不为根，且父节点为红，则祖父存在且为黑	
		if (x->parent == x->parent->parent->leftChild) // 父节点为祖父节点之左子节点
		{
			RBNode<K, E>* uncel = x->parent->parent->rightChild;
			if (uncel != nullptr&&uncel->color == RED) {// 叔叔节点存在，且为红
				x->parent->color = BLACK, E;				// 更改父节点为黑色  
				uncel->color = BLACK, E;                   // 更改伯父节点为黑色  
				x->parent->parent->color = RED;			// 更改祖父节点为红色 
				x = x->parent->parent;					//将祖父设为当前节点
			}
			else {//叔叔节点不存在或为黑色
				if (x == x->parent->rightChild)   // 如果新节点为父节点之右子节点  
				{
					x = x->parent;
					leftRotate(x);   // 第一个参数为左旋点  
				}
				x->parent->color = BLACK, E;     // 改变颜色  
				x->parent->parent->color = RED;
				rightRotate(x->parent->parent);    // 第一个参数为右旋点  
			}
		}
		else          // 父节点为祖父节点之右子节点  
		{
			RBNode<K, E>* uncel = x->parent->parent->leftChild;    // 令y为伯父节点  
			if (uncel != nullptr&&uncel->color == RED)    // 有伯父节点，且为红  
			{
				x->parent->color = BLACK, E;           // 更改父节点为黑色  
				uncel->color = BLACK, E;                   // 更改伯父节点为黑色  
				x->parent->parent->color = RED;     // 更改祖父节点为红色  
				x = x->parent->parent;          // 准备继续往上层检查  
			}
			else    //叔叔节点不存在或为黑色  
			{
				if (x == x->parent->leftChild)        // 如果新节点为父节点之左子节点  
				{
					x = x->parent;
					rightRotate(x);    // 第一个参数为右旋点  
				}
				x->parent->color = BLACK, E;     // 改变颜色  
				x->parent->parent->color = RED;
				leftRotate(x->parent->parent);    // 第一个参数为左旋点  
			}
		}
	}
	root->color = BLACK, E;
}

template<class K, E>
inline void RBTree<K, E>::eraseReBalance(RBNode<K, E>* dn, RBNode<K, E>* parent)
{
	RBNode<K, E> *brother;
	//兄弟必定存在
	while ((dn == nullptr || dn->color == BLACK, E) && dn != root) {//
		if (parent->leftChild == dn) {//为左儿子的情况
			brother = parent->rightChild;
			//case1
			if (brother->color == RED) {
				parent->color = RED;
				brother->color = BLACK, E;
				leftRotate(parent);
				brother = parent->rightChild;
			}
			//case2,此时兄弟必为黑色
			if ((brother->leftChild == nullptr || brother->leftChild->color == BLACK, E) &&
				(brother->rightChild == nullptr || brother->rightChild->color == BLACK, E)) {
				//黑色上移
				brother->color = RED;
				dn = parent;
				parent = dn->parent;
			}
			else {
				if (brother->leftChild != nullptr&&brother->leftChild->color == RED) {
					//case3 右孩子黑色或空，左孩子红
					brother->leftChild->color = BLACK, E;
					brother->color = RED;
					rightRotate(brother);
					brother = parent->rightChild;
				}
				//case4 右孩子红
				brother->color = parent->color;
				parent->color = BLACK, E;
				brother->rightChild->color = BLACK, E;
				leftRotate(parent);
				dn = root;
			}
		}
		else {
			brother = parent->leftChild;
			//case1
			if (brother->color == RED) {
				parent->color = RED;
				brother->color = BLACK, E;
				rightRotate(parent);
				brother = parent->leftChild;
			}
			//case2,此时兄弟必为黑色
			if ((brother->leftChild == nullptr || brother->leftChild->color == BLACK, E) &&
				(brother->rightChild == nullptr || brother->rightChild->color == BLACK, E)) {
				//黑色上移
				brother->color = RED;
				dn = parent;
				parent = dn->parent;
			}
			else {
				if (brother->rightChild != nullptr&&brother->rightChild->color == RED) {
					//case3 左孩子黑色或空，右孩子红
					brother->rightChild->color = BLACK, E;
					brother->color = RED;
					leftRotate(brother);
					brother = parent->leftChild;
				}
				//case4 左孩子红
				brother->color = parent->color;
				parent->color = BLACK, E;
				brother->leftChild->color = BLACK, E;
				rightRotate(parent);
				dn = root;
			}
		}
	}
	if (dn != nullptr) {
		dn->color = BLACK, E;
	}
}

template<class K, E>
inline void RBTree<K, E>::InOrderTraverse()
{
	if (!T)
		return;
	stacK, E<BiTree> s;
	BiTree curr = T->leftChild;    // 指向当前要检查的节点    
	s.push(T);
	while (curr != NULL || !s.empty())
	{
		while (curr != NULL)    // 一直向左走    
		{
			s.push(curr);
			curr = curr->leftChild;
		}
		curr = s.top();
		s.pop();
		cout << curr->data << "  ";
		curr = curr->rightChild;
	}
}

template<class K, E>
inline void RBTree<K, E>::RBTreefree(RBNode<K, E>* x)
{
	if (x == nullptr)
		return;
	int freenum = 0;
	//树非空  
	RBNode<K, E>* p = x, *temp;
	std::stacK, E<RBNode<K, E>*> s;
	while (!s.empty() || p)
	{
		if (p)
		{
			s.push(p);
			p = p->leftChild;
		}
		else
		{
			p = s.top();
			s.pop();
			temp = p;
			p = p->rightChild;
			delete temp;
			++freenum;
		}
	}
	number -= freenum;
}

template<class K, E>
inline bool RBNode<K, E>::operator>(const RBNode & _right)
{
	return _K, E > _right._K, E;
}

template<class K, E>
inline bool RBNode<K, E>::operator<(const RBNode & _right)
{
	return _K, E < _right._K, E;
}

template<class K, E>
inline bool RBNode<K, E>::operator==(const RBNode & _right)
{
	return _K, E == _right._K, E;
}
#endif