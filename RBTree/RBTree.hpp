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


template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		: _pRoot(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (_pRoot == NULL)      //寻找插入的地方
		{
			_pRoot = new Node(key, value);
			_pRoot->_color = BLACK;
			return true;
		}
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

	void InOrder()
	{
		cout << " InOrder: ";
		_InOrder(_pRoot);
		cout << endl;
	}

	bool CheckRBTree()
	{
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

protected:
	void _RotateL(Node* pParent)
	{
		Node* pSubR = pParent->_pRight;
		Node* pSubRL = pSubR->_pLeft;

		pParent->_pRight = pSubRL;
		if (pSubRL)
			pSubRL->_pParent = pParent;

		Node* PPparent = pParent->_pParent;
		pSubR->_pLeft = pParent;
		pParent->_pParent = pSubR;
		pSubR->_pParent = PPparent;

		if (NULL == PPparent)
			_pRoot = pSubR;
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
		Node* pSubL = pParent->_pLeft;
		Node* pSubLR = pSubL->_pRight;

		pParent->_pLeft = pSubLR;
		if (pSubLR)
			pSubLR->_pParent = pParent;

		Node* PPparent = pParent->_pParent;
		pSubL->_pRight = pParent;
		pParent->_pParent = pSubL;

		pSubL->_pParent = PPparent;
		if (NULL == PPparent)
			_pRoot = pSubL;
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

protected:
	Node* _pRoot;
};


