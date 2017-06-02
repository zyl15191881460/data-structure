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
	int _bf;  //ƽ������
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
		Node* pCur = _pRoot; //_pRootһֱ�ǿ�ʼ�����ĵ�һ��ֵ
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
				cout << "��key�Ѵ���" << endl;
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
			if (parent->_bf == 0)//��parent�����ӻ��Һ���ʱ����������ӽ�㲻��Ӱ��ý���ƽ��
				return true;
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				NewNode = parent;
				parent = parent->_pParent;
			}
			else								//parent->_bf��ֵֻ����-2����2��
			{
				if (parent->_bf == 2)			//�Ҳ��
				{
					if (NewNode->_bf == 1)		//��ӵĽ���ڽϸ����������Ҳ࣬���е�������
						_RotateL(parent);
					else						//��ӵĽ���ڽϸ�����������࣬��������˫��
						_RotateRL(parent);
				}
				else							//parent->_bf == -2��������
				{
					if (NewNode->_bf == -1)		//��ӵĽ���ڽϸ�����������࣬�����ҵ���
						_RotateR(parent);
					else						//��ӵĽ���ڽϸ����������Ҳ࣬ ��������˫��
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

		if (RightHight - LeftHight != pRoot->_bf || abs(pRoot->_bf) > 1) //�ж��Ƿ�ΪAVL���Ĺؼ���������������һ��Ϊ||
		{
			cout << pRoot->_key << "-->" << pRoot->_bf << endl;
			return false;
		}
		return _IsBalanceTree(pRoot->_pLeft) && _IsBalanceTree(pRoot->_pRight); //�ݹ��ȥ�ж�ÿ������Ƿ�����AVL��������
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

	void _RotateL(Node* parent)  //����
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
		else if (ret == 1) // ���� 
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
		else if (ret == 1) // ����Ҫ������Ϊ0��ʱ������
			pSubL->_bf = -1; 
	}
private:
	Node* _pRoot;
};