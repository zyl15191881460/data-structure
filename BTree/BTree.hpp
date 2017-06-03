#pragma once
#include <iostream>
using namespace std;
#include <assert.h>

template<class K, size_t M>
struct BTreeNode
{
	K _keys[M]; //�ؼ��뼯��  ��������Ӧ��ֻ��M-1��Ԫ�أ������ڰ��Ƶ�ʱ��Ϊ���ܹ��ö�һ��Ԫ�ز���,����ʱ���㣬���ԾͶ��һ��λ��
	BTreeNode<K, M>* _pParent;	//˫��
	BTreeNode<K, M>* _pSub[M + 1];		//	���Ӽ��ϣ����ӽڵ�һ���ȹؼ����һ��
	size_t _size;						//��ЧԪ�صĸ���


	BTreeNode()							//��ʼ������ָ���������Ԫ�س�ʼ��ΪNULL;
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
		//�ҵ������λ��
		if (NULL == _pRoot)
		{
			_pRoot = new Node;
			_pRoot->_keys[0] = key;
			_pRoot->_size++;
			return true;
		}
		pair<Node*, int> ret = Find(key);
		if (ret.second > -1)		//������ֵ��Second��ֵ==-1ʱ����˵�����ҵ��ĸýڵ㣬���ԾͲ��ò����ˣ�
			return false;
		K k = key;
		
		Node* pCur = ret.first;
		Node* pSub = NULL;
		while (1)
		{
			//����һ���ڵ㣬����ʵ�ַ���
			_InsertKey(pCur, k, pSub);
			if (pCur->_size < M)
				return true;
			//ʵ�ַ���
			size_t mid = M / 2;
			Node* pNewNode = new Node;
			//		size_t index = 0;
			
			for (size_t idx = mid + 1; idx < pCur->_size; ++idx) //���½����з��Ѻ�ĸ�ֵ
			{
				pNewNode->_keys[pNewNode->_size++] = pCur->_keys[idx];
			}
			pCur->_size = pCur->_size - pNewNode->_size - 1; //ԭ���ʣ���Ԫ�ظ���

			for (size_t j = 0; j < pNewNode->_size + 1; j++)	//����˫���Ժ�˫�ײ������Ҫ���ѵĻ������Ѻ󣬶�ԭ����(M+1)�����ӽڵ�ҲҪ�������µĸ�����ȷ��˫��
			{
				pNewNode->_pSub[j] = pCur->_pSub[mid + 1 + j];
				if (pNewNode->_pSub[j])
					pNewNode->_pSub[j]->_pParent = pNewNode;
				pCur->_pSub[mid + 1 + j] = NULL;
			}

			if (pCur->_pParent == NULL)			//˫�ײ�����
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
	pair<Node*, int> Find(const K& key)  //���ҽڵ�
	{
		Node* pCur = _pRoot;
		Node* pParent = NULL;
		while (pCur)
		{
			size_t idx = 0;
			while (idx < pCur->_size)
			{
				if (key < pCur->_keys[idx])		//keyС��pCur�Ļ���ֱ�������ߵ����ĺ��ӽڵ���
					break;
				else if (key > pCur->_keys[idx])	//key����pCur�Ļ�,ֱ���ߵ�������һ��ָ������
					idx++;
				else
					return make_pair(pCur, idx);//��ȵĻ�ֱ�Ӿͷ��ؼ�ֵ��,(����ָ���M������ָ�ĵڼ���)
			}
			pParent = pCur;  //Ҫ��¼����˫�ף���Ϊ�ǿյĻ������ص�������˫�ף�����Ҫ��������
			pCur = pCur->_pSub[idx];
		}
		return make_pair(pParent, -1);			//û���ҵ���ֱ�ӷ���-1���Լ�����˫��
	}

private:
	void _InsertKey(Node* pCur, const K& key, Node* pSub)		//����Ԫ�أ���Ϊ�ؼ���������ģ�����ÿ������Ĺؼ��붼Ҫ����
	{
		assert(pCur);
		size_t j = 0;
		int end = pCur->_size - 1;
		while (end >= 0)
		{
			if (pCur->_keys[end] > key)				//������ڵĻ��Ͱ��Ƶ�����ȥ��
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


