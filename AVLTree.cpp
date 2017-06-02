#include <iostream>
using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	AVLTreeNode(const K& key, const V& value)
		: _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _key(key)
		, _value(value)
		, _bf(0)
	{}

	AVLTreeNode<K, V>* _pLeft;
	AVLTreeNode<K, V>* _pRight;
	AVLTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	int _bf;  //平衡因子
};


template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		: _pRoot(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
 		if (NULL == _pRoot)
		{
			_pRoot = new Node(key, value);
			return true;
		}
		Node* pCur = _pRoot; //_pRoot一直是开始给定的第一个值
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
				cout << "该key已存在" << endl;
				return false;
			}
		}
		Node* NewNode = new Node(key, value);
		if (key < parent->_key)
		{
			parent->_pLeft = NewNode;
			NewNode->_pParent = parent;
		}
		else
		{
			parent->_pRight = NewNode;
			NewNode->_pParent = parent;
		}

		while (parent)
		{
			if (parent->_pLeft == NewNode)
				parent->_bf--;
			else
				parent->_bf++;
			if (parent->_bf == 0)//当parent有左孩子或右孩子时，给他们添加结点不会影响该结点的平衡
				return true;
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				NewNode = parent;
				parent = parent->_pParent;
			}
			else								//parent->_bf的值只能是-2或者2了
			{
				if (parent->_bf == 2)			//右侧高
				{
					if (NewNode->_bf == 1)		//添加的结点在较高右子树的右侧，进行的是左单旋
						_RotateL(parent);
					else						//添加的结点在较高右子树的左侧，进行右左双旋
						_RotateRL(parent);
				}
				else							//parent->_bf == -2，即左侧高
				{
					if (NewNode->_bf == -1)		//添加的结点在较高左子树的左侧，进行右单旋
						_RotateR(parent);
					else						//添加的结点在较高左子树的右侧， 进行左右双旋
						_RotateLR(parent);
				}
				break;
			}
		}
		return true;
	}

	void InOrder()
	{
		cout << "InOrder: ";
		_InOrder(_pRoot);
		cout << endl;
	}

	bool IsBalance()
	{
		return _IsBalanceTree(_pRoot);
	}

private:
	bool _IsBalanceTree(Node* pRoot)
	{
		if (NULL == pRoot)
			return true;
		size_t LeftHight = _Height(pRoot->_pLeft);
		size_t RightHight = _Height(pRoot->_pRight);

		if (RightHight - LeftHight != pRoot->_bf || abs(pRoot->_bf) > 1) //判断是否为AVL树的关键条件，两个条件一定为||
		{
			cout << pRoot->_key << "-->" << pRoot->_bf << endl;
			return false;
		}
		return _IsBalanceTree(pRoot->_pLeft) && _IsBalanceTree(pRoot->_pRight); //递归的去判断每个结点是否满足AVL树的特性
	}

	size_t _Height(Node* pRoot)
	{
		if (NULL == pRoot)
			return 0;
		if (NULL == pRoot->_pLeft && NULL == pRoot->_pRight)
			return 1;
		size_t LeftHight = _Height(pRoot->_pLeft);
		size_t RightHight = _Height(pRoot->_pRight);
		return LeftHight > RightHight ? LeftHight + 1 : RightHight + 1;
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

	void _RotateL(Node* parent)  //左单旋
	{
		Node* pSubR = parent->_pRight;
		Node* pSubRL = pSubR->_pLeft;
		parent->_pRight = pSubRL;
		if (pSubRL)
			pSubRL->_pParent = parent;

		Node* PPparent = parent->_pParent;
		pSubR->_pLeft = parent;
		parent->_pParent = pSubR;

		if (PPparent)
		{
			if (PPparent->_pLeft == parent)
			{
				PPparent->_pLeft = pSubR;
				pSubR->_pParent = PPparent;
			}
			else
			{
				PPparent->_pRight = pSubR;
				pSubR->_pParent = PPparent;
			}
		}
		else
		{
			pSubR->_pParent = PPparent;
			_pRoot = pSubR;
		}

		parent->_bf = pSubR->_bf = 0;
	}
	void _RotateR(Node* parent)
	{
		Node* pSubL = parent->_pLeft;
		Node* pSubLR = pSubL->_pRight;
		parent->_pLeft = pSubLR;
		if (pSubLR)
			pSubLR->_pParent = parent;

		Node* PPparent = parent->_pParent;
		pSubL->_pRight = parent;
		parent->_pParent = pSubL;

		if (PPparent)
		{
			if (PPparent->_pLeft == parent)
			{
				PPparent->_pLeft = pSubL;
				pSubL->_pParent = PPparent;
			}
			else
			{
				PPparent->_pRight = pSubL;
				pSubL->_pParent = PPparent;
			}
		}
		else
		{
			pSubL->_pParent = PPparent;
			_pRoot = pSubL;
		}
		parent->_bf = pSubL->_bf = 0;
	}
	void _RotateRL(Node* parent)
	{
		Node* pSubR = parent->_pRight;
		Node* pSubRL = pSubR->_pLeft;
		int ret = pSubRL->_bf;
		_RotateR(pSubR);
		_RotateL(parent);

		if (ret == -1)
			pSubR->_bf = 1;
		else if (ret == 1) // ？？ 
			parent->_bf = -1;
	}
	void _RotateLR(Node* parent)
	{
		Node* pSubL = parent->_pLeft;
		Node* pSubLR = pSubL->_pRight;
		int ret = pSubLR->_bf;

		_RotateL(pSubL);
		_RotateR(parent);

		if (ret == -1)
			parent->_bf = 1;
		else if (ret == 1) // 这里要处理当他为0的时候的情况
			pSubL->_bf = -1; 
	}
private:
	Node* _pRoot;
};