#pragma once
#ifndef _RBTREE_H_
#define _RBTREE_H_
#define RED 0
#define BLACK 1
//key 必须实现<,>,==,拷贝构造，赋值函数
#include<stack>
namespace fmxstd {
	template<class key>
	class node {
	public:
		key _key;
		bool color;
		node<key> *lchild;
		node<key> *rchild;
		node<key> *parent;
	public:
		node() = default;
		node(key _key) :_key(_key), lchild(__nullptr), rchild(__nullptr), parent(nullptr), color(RED) {};
		~node() {};
		bool operator>(const node& _right);
		bool operator<(const node& _right);
		bool operator==(const node& _right);
	};

	template<class key>
	class RBTree {
	private:
		node<key>* root;//���ڵ��parent=nullptr��
		unsigned long long int number;//�ڵ����
	public:
		RBTree() :root(nullptr), number(0) {};
		~RBTree();
		RBTree(const RBTree&) = delete;
		RBTree& operator=(const RBTree&) = delete;
		void insert(key _key);
		void remove(key _key);
		bool find(key _key) const;
		size_t size()const;
		bool empty()const;

		//判断是否是红黑树，test
		int isRBTree();
	private:
		//判断是否是红黑树,返回黑节点一列的个数
		int isRBTree(node<key>*);
		//判断是否是二叉搜索树
		bool isBinarySearchTree(node<key>*);
		//左旋
		void leftRotate(node<key>*);
		//右旋
		void rightRotate(node<key>*);
		//新增点后的再平衡
		void insertReBalance(node<key>*);
		//传参带有两个黑色属性
		void removeReBalance(node<key>*, node<key>*);
		//中序遍历
		void InOrderTraverse();
		//释放二叉树
		void RBTreefree(node<key>*);
	};



	template<class key>
	inline RBTree<key>::~RBTree()
	{
		free(root);
	}

	template<class key>
	void RBTree<key>::insert(key _key)
	{
		node<key>* p = nullptr;
		node<key>* c = root;
		while (c != nullptr) {
			p = c;
			if (c->_key > _key) {
				c = c->lchild;
				continue;
			}
			if (c->_key < _key) {
				c = c->rchild;
				continue;
			}
			if (c->_key == _key) {
				return;
			}
		}
		//数量+1
		++number;
		node<key>* newnode = new node<key>(_key);
		if (p == nullptr) {
			root = newnode;
		}
		else {
			if (p->_key > _key) {
				p->lchild = newnode;
				newnode->parent = p;
			}
			else {
				p->rchild = newnode;
				newnode->parent = p;
			}
		}
		insertReBalance(newnode);
	}

	template<class key>
	void RBTree<key>::remove(key _key)
	{
		node<key>* c = root;
		while (c != nullptr) {
			if (c->_key > _key) {
				c = c->lchild;
				continue;
			}
			if (c->_key < _key) {
				c = c->rchild;
				continue;
			}
			if (c->_key == _key) {
				break;
			}
		}
		if (c == nullptr)//没找到
			return;
		number--;
		if (c->lchild != nullptr&&c->rchild != nullptr) {
			//寻找后继，暂定右子树的最小节点
			node<key>* houji = c->rchild;
			while (houji->lchild != nullptr)
				houji = houji->lchild;
			//交换key
			key tem = houji->_key;
			houji->_key = c->_key;
			c->_key = tem;
			c = houji;
		}
		//c必定是叶子节点
		if (c->lchild == nullptr&&c->rchild == nullptr) {//处理孤立节点的情况
			if (c == root) {
				root = nullptr;
				delete c;
				c = nullptr;
				return;
			}
			if (c->parent->lchild == c) {
				c->parent->lchild = nullptr;
			}
			else {
				c->parent->rchild = nullptr;
			}
			if (c->color == BLACK) {
				removeReBalance(nullptr, c->parent);
			}
			delete c;
			c = nullptr;
			return;
		}
		if (c->lchild == nullptr&&c->rchild != nullptr) {//处理有一个子树的情况
			if (c == root) {
				root = c->rchild;
				root->color = BLACK;
				delete c;
				c = nullptr;
				return;
			}
			if (c->parent->lchild == c) {
				c->parent->lchild = c->rchild;
			}
			else {
				c->parent->rchild = c->rchild;
			}
			c->rchild->parent = c->parent;
			removeReBalance(c->rchild, c->parent);
		}
		else {
			if (c == root) {
				root = c->lchild;
				root->color = BLACK;
				delete c;
				c = nullptr;
				return;
			}
			if (c->parent->lchild == c) {
				c->parent->lchild = c->lchild;
			}
			else {
				c->parent->rchild = c->lchild;
			}
			c->lchild->parent = c->parent;
			removeReBalance(c->lchild, c->parent);
		}
		delete c;
		c = nullptr;
		return;
	}

	template<class key>
	inline bool RBTree<key>::find(key _key) const
	{
		node<key>* now = root;
		while (now != nullptr) {
			if (now._key > _key) {
				now = now->lchild;
				continue;
			}
			if (now._key < _key) {
				now = now->rchild;
				continue;
			}
			if (now->_key == _key) {
				return 1;
			}
		}
		return 0;
	}

	template<class key>
	inline size_t RBTree<key>::size() const
	{
		return number;
	}

	template<class key>
	inline bool RBTree<key>::empty() const
	{
		return !bool(number);
	}


	template<class key>
	inline int RBTree<key>::isRBTree(node<key>* x)
	{
		//空节点当做黑节点
		if (x == nullptr)
			return 1;
		if (x->color == RED) {
			if (x->lchild != nullptr&&x->lchild->color == RED)
				return 0;
			if (x->rchild != nullptr&&x->rchild->color == RED)
				return 0;
		}
		int left = isRBTree(x->lchild);
		int right = isRBTree(x->rchild);
		if (left == 0 || right == 0 || left != right)
			return 0;//错误
		if (x->color == RED) {
			return left;
		}
		else {
			return left + 1;
		}

	}

	template<class key>
	inline void RBTree<key>::leftRotate(node<key>* x)
	{
		/*
		*	 px                              px
		*    /                               /
		*	x                               y
		*  / \      --(左旋)-->			   / \
		* lx   y                          x  ry
		*	  / \						 / \
		*    ly   ry                   lx  ly
		*/
		node<key>* y = x->rchild;
		if (y == nullptr)
			return;
		node<key> *ly = y->lchild;
		y->parent = x->parent;
		if (y->parent == nullptr)
			root = y;
		else {
			if (y->parent->lchild == x) {
				y->parent->lchild = y;
			}
			else {
				y->parent->rchild = y;
			}
		}
		x->parent = y;
		y->lchild = x;
		x->rchild = ly;
		if (ly != nullptr) {
			ly->parent = x;
		}
	}

	template<class key>
	inline void RBTree<key>::rightRotate(node<key>* x)
	{
		//右节点无，则无必要左旋。
		node<key> *y = x->lchild;
		if (y == nullptr)
			return;
		node<key> *ry = y->rchild;
		y->parent = x->parent;
		if (y->parent == nullptr)
			root = y;
		else {
			if (y->parent->lchild == x) {
				y->parent->lchild = y;
			}
			else {
				y->parent->rchild = y;
			}
		}
		x->parent = y;
		y->rchild = x;
		x->lchild = ry;
		if (ry != nullptr) {
			ry->parent = x;
		}
	}

	template<class key>
	inline bool RBTree<key>::isBinarySearchTree(node<key>* x)
	{
		if (x == nullptr)
			return 1;
		if (x->lchild != nullptr&&x->rchild != nullptr) {
			return (x->lchild->_key < x->_key) && (x->rchild->_key > x->_key) && isBinarySearchTree(x->lchild) && isBinarySearchTree(x->rchild);
		}
		if (x->lchild != nullptr&&x->rchild == nullptr) {
			return (x->lchild->_key < x->_key) && isBinarySearchTree(x->lchild);
		}
		if (x->lchild == nullptr&&x->rchild != nullptr) {
			return (x->rchild->_key > x->_key) && isBinarySearchTree(x->rchild);
		}
		//叶子节点
		return 1;
	}

	template<class key>
	inline int RBTree<key>::isRBTree()
	{
		if (isBinarySearchTree(root)) {
			return isRBTree(root);
		}
		return 0;
	}



	template<class key>
	inline void RBTree<key>::insertReBalance(node<key>* newnode)
	{
		node<key>* x = newnode;
		while (x != root && x->parent->color == RED) {//当前节点不为根，且父节点为红，则祖父存在且为黑	
			if (x->parent == x->parent->parent->lchild) // 父节点为祖父节点之左子节点
			{
				node<key>* uncel = x->parent->parent->rchild;
				if (uncel != nullptr&&uncel->color == RED) {// 叔叔节点存在，且为红
					x->parent->color = BLACK;				// 更改父节点为黑色  
					uncel->color = BLACK;                   // 更改伯父节点为黑色  
					x->parent->parent->color = RED;			// 更改祖父节点为红色 
					x = x->parent->parent;					//将祖父设为当前节点
				}
				else {//叔叔节点不存在或为黑色
					if (x == x->parent->rchild)   // 如果新节点为父节点之右子节点  
					{
						x = x->parent;
						leftRotate(x);   // 第一个参数为左旋点  
					}
					x->parent->color = BLACK;     // 改变颜色  
					x->parent->parent->color = RED;
					rightRotate(x->parent->parent);    // 第一个参数为右旋点  
				}
			}
			else          // 父节点为祖父节点之右子节点  
			{
				node<key>* uncel = x->parent->parent->lchild;    // 令y为伯父节点  
				if (uncel != nullptr&&uncel->color == RED)    // 有伯父节点，且为红  
				{
					x->parent->color = BLACK;           // 更改父节点为黑色  
					uncel->color = BLACK;                   // 更改伯父节点为黑色  
					x->parent->parent->color = RED;     // 更改祖父节点为红色  
					x = x->parent->parent;          // 准备继续往上层检查  
				}
				else    //叔叔节点不存在或为黑色  
				{
					if (x == x->parent->lchild)        // 如果新节点为父节点之左子节点  
					{
						x = x->parent;
						rightRotate(x);    // 第一个参数为右旋点  
					}
					x->parent->color = BLACK;     // 改变颜色  
					x->parent->parent->color = RED;
					leftRotate(x->parent->parent);    // 第一个参数为左旋点  
				}
			}
		}
		root->color = BLACK;
	}

	template<class key>
	inline void RBTree<key>::removeReBalance(node<key>* dn, node<key>* parent)
	{
		node<key> *brother;
		//兄弟必定存在
		while ((dn == nullptr || dn->color == BLACK) && dn != root) {//
			if (parent->lchild == dn) {//为左儿子的情况
				brother = parent->rchild;
				//case1
				if (brother->color == RED) {
					parent->color = RED;
					brother->color = BLACK;
					leftRotate(parent);
					brother = parent->rchild;
				}
				//case2,此时兄弟必为黑色
				if ((brother->lchild == nullptr || brother->lchild->color == BLACK) &&
					(brother->rchild == nullptr || brother->rchild->color == BLACK)) {
					//黑色上移
					brother->color = RED;
					dn = parent;
					parent = dn->parent;
				}
				else {
					if (brother->lchild != nullptr&&brother->lchild->color == RED) {
						//case3 右孩子黑色或空，左孩子红
						brother->lchild->color = BLACK;
						brother->color = RED;
						rightRotate(brother);
						brother = parent->rchild;
					}
					//case4 右孩子红
					brother->color = parent->color;
					parent->color = BLACK;
					brother->rchild->color = BLACK;
					leftRotate(parent);
					dn = root;
				}
			}
			else {
				brother = parent->lchild;
				//case1
				if (brother->color == RED) {
					parent->color = RED;
					brother->color = BLACK;
					rightRotate(parent);
					brother = parent->lchild;
				}
				//case2,此时兄弟必为黑色
				if ((brother->lchild == nullptr || brother->lchild->color == BLACK) &&
					(brother->rchild == nullptr || brother->rchild->color == BLACK)) {
					//黑色上移
					brother->color = RED;
					dn = parent;
					parent = dn->parent;
				}
				else {
					if (brother->rchild != nullptr&&brother->rchild->color == RED) {
						//case3 左孩子黑色或空，右孩子红
						brother->rchild->color = BLACK;
						brother->color = RED;
						leftRotate(brother);
						brother = parent->lchild;
					}
					//case4 左孩子红
					brother->color = parent->color;
					parent->color = BLACK;
					brother->lchild->color = BLACK;
					rightRotate(parent);
					dn = root;
				}
			}
		}
		if (dn != nullptr) {
			dn->color = BLACK;
		}
	}

	template<class key>
	inline void RBTree<key>::InOrderTraverse()
	{
		if (!T)
			return;
		stack<BiTree> s;
		BiTree curr = T->lchild;    // 指向当前要检查的节点    
		s.push(T);
		while (curr != NULL || !s.empty())
		{
			while (curr != NULL)    // 一直向左走    
			{
				s.push(curr);
				curr = curr->lchild;
			}
			curr = s.top();
			s.pop();
			cout << curr->data << "  ";
			curr = curr->rchild;
		}
	}

	template<class key>
	inline void RBTree<key>::RBTreefree(node<key>* x)
	{
		if (x == nullptr)
			return;
		int freenum = 0;
		//树非空  
		node<key>* p = x, *temp;
		std::stack<node<key>*> s;
		while (!s.empty() || p)
		{
			if (p)
			{
				s.push(p);
				p = p->lchild;
			}
			else
			{
				p = s.top();
				s.pop();
				temp = p;
				p = p->rchild;
				delete temp;
				++freenum;
			}
		}
		number -= freenum;
	}

	template<class key>
	inline bool node<key>::operator>(const node & _right)
	{
		return _key > _right._key;
	}

	template<class key>
	inline bool node<key>::operator<(const node & _right)
	{
		return _key < _right._key;
	}

	template<class key>
	inline bool node<key>::operator==(const node & _right)
	{
		return _key == _right._key;
	}
}
#endif