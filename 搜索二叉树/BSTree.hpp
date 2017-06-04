#include<iostream>
using namespace std;


template<class K, class V>
struct BSTNode
{
	BSTNode(const K& key, const V& value)
	: _pLeft(NULL)
	, _pRight(NULL)
	, _key(key)
	, _value(value)
	{}

	BSTNode<K, V>* _pLeft;
	BSTNode<K, V>* _pParent;
	BSTNode<K, V>* _pRight;
	K _key;
	V _value;
};


template<class K, class V>
class BinarySearchTree
{
	typedef BSTNode<K, V> Node;
	typedef BinarySearchTree<K, V> Self;

public:

	BinarySearchTree()
		: _pRoot(NULL)
	{}

	BinarySearchTree(const Self& bst)
	{
		_pRoot = _CopyBinarySearchTree(bst._pRoot);
	}
	Self& operator=(const Self& bst)
	{
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


	// ���ҵݹ�ͷǵݹ�
	bool Find_Nor(const K& key)
	{
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

	// ����ݹ�ͷǵݹ�
	bool Insert_Nor(const K& key, const V& value)
	{
		Node* _node = new Node(key, value);
		if (NULL == _pRoot)
		{
			_pRoot = _node;
			return true;
		}
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
				cout << "�����Ѵ���" << endl;
				return false;
			}
		}
		if (key < pParent->_key)
		{
			pParent->_pLeft = _node;
			return true;
		}
		else if (key > pParent->_key)
		{
			pParent->_pRight = _node;
			return true;
		}
		//else
		//	return false;
	}


	bool Insert(const K& key, const V& value)
	{
		return _Insert(_pRoot, key, value);
	}

	// ɾ���ݹ�ͷǵݹ�
	bool Remove_Nor(const K& key) //??????????????????????????????????
	{
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
			}
			else if (key > pDel->_key)
			{
				pParent = pDel;
				pDel = pDel->_pRight;
			}
			else
				break;
		}
		if (pDel)
		{
			if (pDel->_pLeft == NULL) //û��������
			{
				if (pDel == _pRoot)//�ҵ�ֻ
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
			else if (pDel->_pRight == NULL) //û��������
			{
				if (pDel == _pRoot) //��ֻ
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
				//�ҵ�����������С�Ľ�� //??????????????????????????????
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
			cout << "û�������" << endl;
			return false;
		}
	}
	bool Remove(const K& key)
	{
		return _Remove(_pRoot, key);
	}

	void InOrder()
	{
		cout << "InOrder:";
		_InOrder(_pRoot);
		cout << endl;
	}

	Node* TreeToList()
	{
		Node* prev = NULL;
		return _TreeToList(_pRoot, prev);
	}

	~BinarySearchTree()
	{
		if (_pRoot)
		{
			delete _pRoot;
			_pRoot = NULL;
		}
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
			return _Insert(pRoot->_pLeft, key, value);//return ʲôʱ��ʹ�ã�
		else if (key > pRoot->_key)
			return _Insert(pRoot->_pRight, key, value);
		else
		{
			cout << "�����Ѵ���" << endl;
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
				_Remove(pRoot->_pRight, key);//���ﲻ�ܴ���ʱ����
			}
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
	Node* _TreeToList(Node* pRoot, Node*& prev)
	{
		Node* cur = pRoot;
		if (NULL == cur)  //�����Ϊ�գ��˳�
			return NULL;
		_TreeToList(cur->_pLeft, prev);
		cur->_pLeft = prev;
		if (prev)
		{
			prev->_pRight = cur;
		}
		prev = cur;
		_TreeToList(cur->_pRight, prev);

	}
private:
	Node* _pRoot;
};
