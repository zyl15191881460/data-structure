#include<iostream>
using namespace std;

enum COLOR{ RED, BLACK };

template<class K, class V>
struct RBTreeNode
{
	RBTreeNode(const K& key, const V& value, const COLOR& color = RED)
	: _pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	, _key(key)
	, _value(value)
	, _color(color)
	{}

	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	COLOR _color;
};


template<class K, class V, class Ref, class Ptr>
class RBTreeIterator
{
	typedef RBTreeNode<K, V> Node;
	typedef RBTreeIterator<K, V, Ref, Ptr> Self;
public:
	RBTreeIterator()
		: _pNode(NULL)
	{}

	RBTreeIterator(Node* pNode)
		: _pNode(pNode)
	{}

	RBTreeIterator(const Self& s)
		: _pNode(s._pNode)
	{}

	Self& operator++()  //前置++
	{
		Increase(_pNode);
		return *this;
	}
	Self operator++(int)  //后置++
	{
		Self tmp(*this);
		Increase(_pNode);
		return tmp;
	}

	Self& operator--()		//前置--
	{
		Decrease(_pNode);
		return *this;
	}
	Self operator--(int)     //后置--
	{
		Self tmp(*this);
		Decrease(_pNode);
		return tmp;
	}

	Ref operator*()
	{
		return (*_pNode)._key;
	}

	Ptr operator->()
	{
		return &(operator*());
	}
	bool operator!=(const Self& s)
	{
		return _pNode != s._pNode;
	}

	bool operator==(const Self& s)
	{
		return !(_pNode != s->_pNode);
	}

protected:
	Node* Increase(Node* pNode)
	{
		if (pNode->_pRight)
		{
			pNode = pNode->_pRight;
			while (pNode->_pLeft)
				pNode = pNode->_pLeft;
		}
		else
		{
			Node* parent = pNode->_pParent;
			while (parent->_pRight == pNode)
			{
				pNode = parent;
				parent = pNode->_pParent;
			}
			if (pNode->_pRight != parent)
				pNode = parent;
		}
		_pNode = pNode;
		return _pNode;
	}

	Node* Decrease(Node* pNode)
	{
		if (pNode->_pParent->_pParent == pNode && pNode->_color == RED)
			pNode = pNode->_pRight;
		else if (pNode->_pLeft)
		{
			pNode = pNode->_pLeft;
			while (pNode->_pRight)
				pNode = pNode->_pRight;
		}
		else
		{
			Node* parent = pNode->_pParent;
			if (pNode == parent->_pLeft)
			{
				pNode = parent;
				parent = pNode->_pParent;
			}
			pNode = parent;
		}
		_pNode = pNode;
		return _pNode;
	}
private:
	Node* _pNode;
};


template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	typedef RBTreeIterator<K, V, K&, K*> Iterator;
public:
	RBTree()
	{
		_pHead = new Node(0, 0);
		_pHead->_pLeft = _pHead;
		_pHead->_pRight = _pHead;
		_pHead->_pParent = NULL;
		_pHead->_color = RED;
	}

	bool Insert(const K& key, const V& value)
	{
		Node* _pRoot = GetRoot();
		if (_pRoot == NULL)      //寻找插入的地方
		{
			_pRoot = new Node(key, value);
			_pRoot->_color = BLACK;
			_pRoot->_pParent = _pHead;
			_pHead->_pParent = _pRoot;
		}
		else
		{
			Node* pCur = _pRoot;
			Node* parent = NULL;
			while (pCur)
			{
				if (key < pCur->_key)
				{
					parent = pCur;
					pCur = pCur->_pLeft;
				}
				else if (key > pCur->_key)
				{
					parent = pCur;
					pCur = pCur->_pRight;
				}
				else
				{
					cout << "该结点已经存在" << endl;
					return false;
				}
			}

			Node* NewNode = new Node(key, value);  //把结点插入合适的地方
			if (key < parent->_key)
			{
				parent->_pLeft = NewNode;
			}
			else
			{
				parent->_pRight = NewNode;
			}
			NewNode->_pParent = parent;

			////////////////////////////////////////////处理是否满足红黑树////////////////////////////////////////
			while (NewNode != _pRoot && RED == parent->_color)  //进行红黑树的调整,它所能出现的情况都是parent->_color都为红
			{
				Node* Grandfather = parent->_pParent;
				if (parent == Grandfather->_pLeft)//第一种情况，全部在左边 ??
				{
					Node* Uncle = Grandfather->_pRight;
					if (Uncle && RED == Uncle->_color)//情况1
					{
						parent->_color = Uncle->_color = BLACK;
						Grandfather->_color = RED;

						NewNode = Grandfather;
						parent = Grandfather->_pParent;
					}
					else
					{
						if (NewNode == parent->_pRight)  //情况3,因为情况3处理完还要处理情况2 ，所以先处理3中的旋转；
						{
							_RotateL(parent);
							std::swap(parent, NewNode);
						}
						_RotateR(Grandfather);  //情况2
						std::swap(parent->_color, Grandfather->_color);
					}
				}
				else
				{
					Node* Uncle = Grandfather->_pLeft;
					if (Uncle && Uncle->_color == RED)
					{
						parent->_color = Uncle->_color = BLACK;
						Grandfather->_color = RED;

						NewNode = Grandfather;
						parent = Grandfather->_pParent;
					}
					else
					{
						if (NewNode == parent->_pLeft)
						{
							_RotateR(parent);
							std::swap(NewNode, parent);
						}
						_RotateL(Grandfather);
						std::swap(parent->_color, Grandfather->_color);
					}
				}

				_pRoot->_color = BLACK;
			}
		}
		_Size++;
		_pHead->_pLeft = GetMinNode(_pRoot);
		_pHead->_pRight = GetMaxNode(_pRoot);

	}

	void InOrder()
	{
		Node* _pRoot = GetRoot();
		cout << " InOrder: ";
		_InOrder(_pRoot);
		cout << endl;
	}

	bool CheckRBTree()
	{
		Node* _pRoot = GetRoot();
		if (_pRoot == NULL)
			return true;
		if (_pRoot->_color == RED)
			return false;

		Node* pCur = _pRoot;
		size_t BlackCount = 0;
		while (pCur)
		{
			if (BLACK == pCur->_color)
				BlackCount++;
			pCur = pCur->_pLeft;
		}
		return _CheckRBTree(_pRoot, BlackCount, 0);
	}
	Iterator Begin()
	{
		return Iterator(_pHead->_pLeft);
	}
	Iterator End()
	{
		return Iterator(_pHead);
	}
	bool Empty()
	{
		if (NULL == _pHead->_pParent)
			return true;
		return false;
	}

	size_t Size()
	{
		return _Size;
	}

protected:
	void _RotateL(Node* pParent)
	{
		Node* _pRoot = GetRoot();
		Node* pSubR = pParent->_pRight;
		Node* pSubRL = pSubR->_pLeft;

		pParent->_pRight = pSubRL;
		if (pSubRL)
			pSubRL->_pParent = pParent;

		Node* PPparent = pParent->_pParent;
		pSubR->_pLeft = pParent;
		pParent->_pParent = pSubR;
		pSubR->_pParent = PPparent;

		if (_pHead == PPparent)   //有头结点的树之后，应该和_pHead相比较
		{
			_pRoot = pSubR;
			_pHead->_pParent = pSubR;	//把头结点与根节点相连
		}
		else
		{
			if (PPparent->_pLeft == pParent)
				PPparent->_pLeft = pSubR;
			else
				PPparent->_pRight = pSubR;
		}
	}
	void _RotateR(Node* pParent)
	{
		Node* _pRoot = GetRoot();
		Node* pSubL = pParent->_pLeft;
		Node* pSubLR = pSubL->_pRight;

		pParent->_pLeft = pSubLR;
		if (pSubLR)
			pSubLR->_pParent = pParent;

		Node* PPparent = pParent->_pParent;
		pSubL->_pRight = pParent;
		pParent->_pParent = pSubL;

		pSubL->_pParent = PPparent;
		if (_pHead == PPparent)		//这里和没有头结点的计算是不一样的；
		{
			_pRoot = pSubL;
			_pHead->_pParent = pSubL;	//链接双亲
		}
		else
		{
			if (PPparent->_pLeft == pParent)
				PPparent->_pLeft = pSubL;
			else
				PPparent->_pRight = pSubL;
		}
	}
	void _InOrder(Node* pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}
	bool _CheckRBTree(Node* pRoot, const size_t blackCount, size_t k)
	{
		if (pRoot == NULL)
			return true;
		if (pRoot->_pParent && pRoot->_color == RED && pRoot->_pParent->_color == RED)  //有连续两个红色的结点就不是红黑树
		{
			cout << "有连续两个红色结点，不满足红黑树的性质" << endl;
			return false;
		}
		if (pRoot->_color == BLACK)
			k++;
		if (pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
		{
			if (k == blackCount)   //每个分支上的黑色结点要相等
				return true;
			else
			{
				cout << "每个分支上的黑色结点不相等，不满足红黑树的性质" << endl;
				return false;
			}
		}
		return _CheckRBTree(pRoot->_pLeft, blackCount, k) && _CheckRBTree(pRoot->_pRight, blackCount, k);
	}

	Node* GetRoot()
	{
		return _pHead->_pParent;
	}
	Node* GetMinNode(Node* pHead)
	{
		while (pHead->_pLeft)
			pHead = pHead->_pLeft;
		return pHead;
	}
	Node* GetMaxNode(Node* pHead)
	{
		while (pHead->_pRight)
			pHead = pHead->_pRight;
		return pHead;
	}

protected:
	size_t _Size;
	Node* _pHead;
};
