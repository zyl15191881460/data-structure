#include<iostream>
using namespace std;
#include <stack>

template<class T>
struct BinaryTreeNode 
{
	T _data;
	BinaryTreeNode<T>* _pLeft;
	BinaryTreeNode<T>* _pRight;

	BinaryTreeNode(const T& data)
		: _data(data)
		, _pLeft(NULL)
		, _pRight(NULL)
	{}
};

template<class T>
class BinaryTree
{
	typedef BinaryTreeNode<T> Node;
public:
	BinaryTree(T* Pre, T* Mid, size_t size)
	{
		_pRoot = _ReBulidTree(Pre, Pre + size - 1, Mid, Mid + size - 1);
	}

	void PreOrder() //����ݹ����
	{
		cout << "PreOrder:" << endl;
		_PreOrder(_pRoot);
		cout << endl;
	}

	void PreOrder_Nor() //����ǵݹ����
	{
		cout << "PreOrder_Nor:" << endl;
		stack<Node*> s;
		Node* pCur = _pRoot;
		while (pCur || !s.empty())
		{
			while (pCur)
			{
				cout << pCur->_data << " ";
				s.push(pCur);
				pCur = pCur->_pLeft;
			}
			Node* top = s.top();
			s.pop();

			pCur = top->_pRight;
		}
		cout << endl;
	}

	void InOrder() //����ݹ����
	{
		cout << "InOrder:" << endl;
		_InOrder(_pRoot);
		cout << endl;
	}

	void InOrder_Nor() //����ǵݹ����
	{
		cout << "InOrder_Nor:" << endl;
		stack<Node*> s;
		Node* pCur = _pRoot;
		while (pCur || !s.empty())
		{
			while (pCur)
			{
				s.push(pCur);
				pCur = pCur->_pLeft;
			}
			Node* top = s.top();
			s.pop();
			cout << top->_data << " ";

			pCur = top->_pRight;
		}
		cout << endl;
	}

private:
	Node* _ReBulidTree(T* PreStart, T* PreEnd, T* MidStart, T* MidEnd)
	{
		Node* pRoot; 
		pRoot = new Node(*PreStart);
		if (PreStart == PreEnd && MidStart == MidEnd)//���ֻ��һ��pRoot���ͽ�����
			return pRoot;

		T* pCur = MidStart;
		while (pCur <= MidEnd && *pCur != *PreStart) //���������ҵ������
			++pCur;
		size_t lenth = pCur - MidStart; //�������ĳ��ȣ�
		T* LeftTreeEnd = PreStart + lenth; //��������β���

		if (lenth > 0)//���������ڵĻ�������������
			pRoot->_pLeft = _ReBulidTree(PreStart + 1, LeftTreeEnd, MidStart, pCur - 1); //�������ݹ�Ĵ���

		T* RightTreeStart = LeftTreeEnd + 1; //������������ĸ����

		if (PreEnd - PreStart > lenth) //������������ڣ��ݹ�Ĵ���������
			pRoot->_pRight = _ReBulidTree(RightTreeStart, PreEnd, pCur + 1, MidEnd);
		return pRoot;
	}

	void _PreOrder(Node* pRoot)
	{
		if (pRoot)
		{
			cout << pRoot->_data << " ";
			_PreOrder(pRoot->_pLeft);
			_PreOrder(pRoot->_pRight);
		}
	}

	void _InOrder(Node* pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_data << " ";
			_InOrder(pRoot->_pRight);
		}
	}


private:
	BinaryTreeNode<T>* _pRoot;
};

int main()
{
	int pre[] = { 1, 2, 4, 3, 5, 6 };
	int mid[] = { 4, 2, 1, 5, 3, 6 };

	BinaryTree<int> b(pre, mid, 6);
	b.PreOrder();
	b.PreOrder_Nor();
	b.InOrder();
	b.InOrder_Nor();
	return 0;
}