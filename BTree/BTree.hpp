#pragma once
#include <iostream>
using namespace std;
#include <assert.h>

template<class K, size_t M>
struct BTreeNode
{
	K _keys[M]; //关键码集合  本来里面应该只有M-1个元素，但是在搬移的时候为了能够让多一个元素插入,搬移时方便，所以就多给一个位置
	BTreeNode<K, M>* _pParent;	//双亲
	BTreeNode<K, M>* _pSub[M + 1];		//	孩子集合，孩子节点一定比关键码多一个
	size_t _size;						//有效元素的个数


	BTreeNode()							//初始化，将指针数组里的元素初始化为NULL;
		: _size(0)
		, _pParent(NULL)
	{
		for (size_t idx = 0; idx < M; idx++)
			_pSub[idx] = NULL;
	}
};


template<class K, size_t M>
class BTree
{
	typedef BTreeNode<K, M> Node;
public:
	BTree()
		: _pRoot(NULL)
	{}

	bool Insert(const K& key)
	{
		//找到插入的位置
		if (NULL == _pRoot)
		{
			_pRoot = new Node;
			_pRoot->_keys[0] = key;
			_pRoot->_size++;
			return true;
		}
		pair<Node*, int> ret = Find(key);
		if (ret.second > -1)		//当返回值的Second的值==-1时，就说明他找到的该节点，所以就不用插入了；
			return false;
		K k = key;
		
		Node* pCur = ret.first;
		Node* pSub = NULL;
		while (1)
		{
			//插入一个节点，不用实现分裂
			_InsertKey(pCur, k, pSub);
			if (pCur->_size < M)
				return true;
			//实现分裂
			size_t mid = M / 2;
			Node* pNewNode = new Node;
			//		size_t index = 0;
			
			for (size_t idx = mid + 1; idx < pCur->_size; ++idx) //给新结点进行分裂后的赋值
			{
				pNewNode->_keys[pNewNode->_size++] = pCur->_keys[idx];
			}
			pCur->_size = pCur->_size - pNewNode->_size - 1; //原结点剩余的元素个数

			for (size_t j = 0; j < pNewNode->_size + 1; j++)	//进入双层以后，双亲层如果需要分裂的话，分裂后，对原来的(M+1)个孩子节点也要进行重新的赋给正确的双亲
			{
				pNewNode->_pSub[j] = pCur->_pSub[mid + 1 + j];
				if (pNewNode->_pSub[j])
					pNewNode->_pSub[j]->_pParent = pNewNode;
				pCur->_pSub[mid + 1 + j] = NULL;
			}

			if (pCur->_pParent == NULL)			//双亲不存在
			{
				_pRoot = new Node;
				_pRoot->_keys[0] = pCur->_keys[mid];
				_pRoot->_pSub[0] = pCur;
				pCur->_pParent = _pRoot;
				_pRoot->_pSub[1] = pNewNode;
				pNewNode->_pParent = _pRoot;
				_pRoot->_size = 1;
				return true;
			}
			else
			{
				k = pCur->_keys[mid];
				pCur = pCur->_pParent;
				pSub = pNewNode;
			}
		}
	}

	void InOrder()
	{
		_InOrder(_pRoot);
		cout << endl;
	}
	pair<Node*, int> Find(const K& key)  //查找节点
	{
		Node* pCur = _pRoot;
		Node* pParent = NULL;
		while (pCur)
		{
			size_t idx = 0;
			while (idx < pCur->_size)
			{
				if (key < pCur->_keys[idx])		//key小于pCur的话，直接跳出走到他的孩子节点中
					break;
				else if (key > pCur->_keys[idx])	//key大于pCur的话,直接走到他的下一个指针域中
					idx++;
				else
					return make_pair(pCur, idx);//相等的话直接就返回键值对,(他的指针和M阶中所指的第几个)
			}
			pParent = pCur;  //要记录他的双亲，因为是空的话，返回的是他的双亲，所以要保存起来
			pCur = pCur->_pSub[idx];
		}
		return make_pair(pParent, -1);			//没有找到就直接返回-1，以及他的双亲
	}

private:
	void _InsertKey(Node* pCur, const K& key, Node* pSub)		//搬移元素，因为关键码是有序的，所以每个插入的关键码都要排序
	{
		assert(pCur);
		size_t j = 0;
		int end = pCur->_size - 1;
		while (end >= 0)
		{
			if (pCur->_keys[end] > key)				//如果大于的话就搬移到后面去；
			{
				pCur->_keys[end + 1] = pCur->_keys[end];
				pCur->_pSub[end + 2] = pCur->_pSub[end + 1];
			}
			else
				break;
			end--;
		}

		pCur->_keys[end + 1] = key;
			pCur->_size++;
		pCur->_pSub[end + 2] = pSub;		//
		if (pSub)
			pSub->_pParent = pCur;
	}

	void _InOrder(Node* pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pSub[0]);
			for (size_t idx = 0; idx < pRoot->_size; ++idx)
			{
				cout << pRoot->_keys[idx] << " ";
			}
			_InOrder(pRoot->_pSub[pRoot->_size]);
		}
	}

private:
	Node* _pRoot;
};


