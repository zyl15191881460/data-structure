#include<iostream>
using namespace std;


template<class K, class V>
struct BSTNode
{
	BSTNode(const K& key, const V& value)
	: _pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	, _key(key)
	, _value(value)
	{}

	BSTNode<K, V>* _pLeft;
	BSTNode<K, V>* _pRight;
	BSTNode<K, V>* _pParent;
	K _key;
	V _value;
};

template<class K, class V, class Ref, class Ptr>
class BSTreeIterator
{
	typedef BSTNode<K, V> Node;
	typedef BSTreeIterator<K, V, Ref, Ptr> Self;
public:
	BSTreeIterator()
		: _pNode(NULL)
	{}

	BSTreeIterator(Node* pNode)
		: _pNode(pNode)
	{}

	BSTreeIterator(const Self& s)
		: _pNode(s._pNode)
	{}

	Self& operator++()
	{
		Increase(_pNode);
		return *this;
	}
	Self operator++(int)
	{
		Self tmp(*this);
		Increase(_pNode);
		return tmp;
	}

	Self& operator--()
	{
		Decrease(_pNode);
		return *this;
	}
	Self operator--(int)
	{
		Self tmp(*this);
		Decrease(_pNode);
		return tmp;
	}

	Ref operator*()
	{
		return _pNode->_key;
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
		//return !(*this != s);
		return !(this != &s);
	}
protected:
	Node* Increase(Node* pNode)
	{
		if (pNode->_pRight) //如果pNode的右子树存在，就直接找到右子树中最小的结点；
		{
			pNode = pNode->_pRight;
			while (pNode->_pLeft)
				pNode = pNode->_pLeft;
		}
		else //pNode没有右子树，获取的是它的根结点
		{
			Node* pParent = pNode->_pParent;
			while (pParent->_pRight == pNode)
			{
				pNode = pParent;
				pParent = pNode->_pParent;
			}
			if (pNode->_pRight != pParent)  
				pNode = pParent;
		}
		_pNode = pNode;
		return _pNode;
	}

	Node* Decrease(Node* pNode)
	{
		if (pNode->_pParent->_pParent == pNode && pNode == pHead) //如果是头结点的话，那么头结点的右子树便是结果
			pNode = pNode->_pRight;
		else if (pNode->_pLeft) //如果左子树存在就找左子树中最大的
		{
			pNode = pNode->_pLeft;
			while (pNode->_pRight)
				pNode = pNode->_pRight;
		}
		else//左子树不存在就找它所在的根结点
		{
			Node* pParent = pNode->_pParent;
			if (pParent->_pLeft == pNode)
			{
				pNode = pParent;
				pParent = pNode->_pParent;
			}
			pNode = pParent;
		}
		_pNode = pNode;
		return _pNode;
	}
private:
	Node* _pNode;
};

template<class K, class V>
class BinarySearchTree
{
	typedef BSTNode<K, V> Node;
	typedef BinarySearchTree<K, V> Self;
public:
	typedef BSTreeIterator<K, V, K&, K*> Iterator;	
public:
	BinarySearchTree(const K& key = K(), const V& value = V())
	{
		_pHead = new Node(key, value);
		_pHead->_pLeft = _pHead;
		_pHead->_pRight = _pHead;
		_pHead->_pParent = NULL;
	}

	BinarySearchTree(const Self& bst)
	{
		Node* _pRoot = GreatRoot();
		_pHead->_pParent = _CopyBinarySearchTree(bst._pRoot);
	}
	Self& operator=(const Self& bst)
	{
		Node* _pRoot = GreatRoot();
		if (this != &bst)
		{
			if (_pRoot)
			{
				delete _pRoot;
				_pRoot = _CopyBinarySearchTree(bst._pRoot);
			}
		}
		return *this;
	}


	// 查找递归和非递归
	bool Find_Nor(const K& key)
	{
		Node* _pRoot = GreatRoot();
		Node* pCur = _pRoot;
		if (NULL == _pRoot)
			return false;
		while (pCur)
		{
			if (key < pCur->_key)
				pCur = pCur->_pLeft;
			else if (key > pCur->_key)
				pCur = pCur->_pRight;
			else
				return true;
		}
		return false;
	}
	bool Find(const K& key)
	{
		return _Find(_pRoot, key);
	}

	// 插入递归和非递归
	void Insert_Nor(const K& key, const V& value)
	{
		Node* _pRoot = GreatRoot();
		Node* _node = new Node(key, value);
		if (NULL == _pRoot)
		{
			_pRoot = _node;
			_pRoot->_pParent = _pHead;
			_pHead->_pParent = _pRoot;
		}
		else
		{
			Node* pParent = NULL;
			Node* pCur = _pRoot;
			while (pCur)
			{
				if (key < pCur->_key)
				{
					pParent = pCur;
					pCur = pCur->_pLeft;
				}
				else if (key > pCur->_key)
				{
					pParent = pCur;
					pCur = pCur->_pRight;
				}
				else
				{
					cout << "数据已存在" << endl;
					break;
				}
			}

			if (key < pParent->_key)
			{
				pParent->_pLeft = _node;
				pParent->_pLeft->_pParent = pParent;
			}
			else if (key > pParent->_key)
			{
				pParent->_pRight = _node;
				pParent->_pRight->_pParent = pParent;
			}
			//else
			//	return false;
		}
		_pHead->_pLeft = GetMinNode(_pHead);
		_pHead->_pRight = GetMaxNode(_pHead);
	}


	bool Insert(const K& key, const V& value)
	{
		Node* _pRoot = GreatRoot();
		return _Insert(_pRoot, key, value);
	}

	// 删除递归和非递归
	bool Remove_Nor(const K& key) //??????????????????????????????????
	{
		Node* _pRoot = GreatRoot();
		if (NULL == _pRoot)
			return false;
		if (NULL == _pRoot->_pLeft && NULL == _pRoot->_pRight)
		{
			delete _pRoot;
			_pRoot = NULL;
			return true;
		}

		Node* pParent = NULL;
		Node* pDel = _pRoot;

		while (pDel)
		{
			if (key < pDel->_key)
			{
				pParent = pDel;
				pDel = pDel->_pLeft;
				pDel->_pParent = pParent;
			}
			else if (key > pDel->_key)
			{
				pParent = pDel;
				pDel = pDel->_pRight;
				pDel->_pParent = pParent;
			}
			else
				break;
		}
		if (pDel)
		{
			if (pDel->_pLeft == NULL) //没有左子树
			{
				if (pDel == _pRoot)//右单只
				{
					_pRoot = pDel->_pRight;
				}
				else
				{
					if (pParent->_pLeft == pDel)
						pParent->_pLeft = pDel->_pRight;
					else
						pParent->_pRight = pDel->_pRight;
				}
				delete pDel;
				pDel = NULL;
			}
			else if (pDel->_pRight == NULL) //没有右子树
			{
				if (pDel == _pRoot) //左单只
					_pRoot = pDel->_pLeft;
				else
				{
					if (pParent->_pLeft == pDel)
						pParent->_pLeft = pDel->_pLeft;
					else
						pParent->_pRight = pDel->_pLeft;
				}
				delete pDel;
				pDel = NULL;
			}
			else
			{
				//找到右子树中最小的结点 //??????????????????????????????
				Node* FirstInder = pDel->_pRight;
				Node* prve = pDel;
				if (FirstInder->_pLeft)
				{
					prve = FirstInder;
					FirstInder = FirstInder->_pLeft;
				}
				std::swap(FirstInder->_key, pDel->_key);
				std::swap(FirstInder->_value, pDel->_value);
				if (prve->_pRight == FirstInder)
					prve->_pRight = FirstInder->_pRight;
				else
					prve->_pLeft = FirstInder->_pRight;
				delete FirstInder;
				FirstInder = NULL;
			}
			return true;
		}
		else
		{
			cout << "没有这个数" << endl;
			return false;
		}
		_pHead->_pLeft = GetMinNode(_pHead);
		_pHead->_pRight = GetMaxNode(_pHead);
	}
	bool Remove(const K& key)
	{
		Node* _pRoot = GreatRoot();
		return _Remove(_pRoot, key);
	}

	void InOrder()
	{
		Node* _pRoot = GreatRoot();
		cout << "InOrder:";
		_InOrder(_pRoot);
		cout << endl;
	}

	~BinarySearchTree()
	{
		if (_pHead)
		{
			delete _pHead;
			_pHead = NULL;
		}
	}

	Node* Begin()
	{
		return _pHead->_pLeft;
	}
	Node* End()
	{
		return _pHead;
	}

private:
	Node* _CopyBinarySearchTree(Node* Root)
	{
		Node* NewNode = NULL;
		if (Root)
		{
			NewNode = new Node(Root->_key, Root->_value);
			NewNode->_pLeft = _CopyBinarySearchTree(Root->_pLeft);
			NewNode->_pRight = _CopyBinarySearchTree(Root->_pRight);
		}
		return NewNode;
	}
	bool _Find(Node* pRoot, const K& key)
	{
		if (NULL == pRoot)
			return false;
		if (key < pRoot->_key)
			return _Find(pRoot->_pLeft, key);
		else if (key > pRoot->_key)
			return _Find(pRoot->_pRight, key);
		else
			return true;
	}
	bool _Insert(Node* &pRoot, const K& key, const V& value)
	{
		if (NULL == pRoot)
		{
			pRoot = new Node(key, value);
			return true;
		}
		if (key < pRoot->_key)
			return _Insert(pRoot->_pLeft, key, value);//return 什么时候使用？
		else if (key > pRoot->_key)
			return _Insert(pRoot->_pRight, key, value);
		else
		{
			cout << "数据已存在" << endl;
			return false;
		}
	}
	bool _Remove(Node*& pRoot, const K& key)// ?????????????????????
	{
		if (NULL == pRoot)
			return false;
		if (key < pRoot->_key)
			return _Remove(pRoot->_pLeft, key);
		else if (key > pRoot->_key)
			return _Remove(pRoot->_pRight, key);
		else
		{
			Node* pDel = pRoot;
			if (pRoot->_pLeft == NULL)
			{
				pRoot = pRoot->_pRight;
				delete pDel;
				pDel = NULL;
			}
			else if (pRoot->_pRight == NULL)
			{
				pRoot = pRoot->_pLeft;
				delete pDel;
				pDel = NULL;
			}
			else
			{
				Node* FirstInder = pDel->_pRight;
				while (FirstInder->_pLeft)
					FirstInder = FirstInder->_pLeft;
				std::swap(FirstInder->_key, pDel->_key);
				std::swap(FirstInder->_value, pDel->_value);
				_Remove(pRoot->_pRight, key);//这里不能传临时变量
			}
		}

	}
	void _InOrder(Node* pRoot)
	{
		Node* _pRoot = GreatRoot();
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}
	Node* GreatRoot()
	{
		return _pHead->_pParent;
	}

	Node* GetMinNode(Node* pHead)
	{
		Node* pCur;
		pCur = GreatRoot();
		while (pCur->_pLeft)
			pCur = pCur->_pLeft;
		return pCur;
	}
	Node* GetMaxNode(Node* pHead)
	{
		Node* pCur;
		pCur = GreatRoot();
		while (pCur->_pRight)
			pCur = pCur->_pRight;
		return pCur;
	}
private:
	Node* _pHead;
};


