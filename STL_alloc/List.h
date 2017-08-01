#pragma once
#include "allocator.h"
#include "Construct.h"

#include <iostream>
using namespace std;
#include <string>

template<class T>
struct ListNode
{
	ListNode(const T& data = T())
	: _pNext(NULL)
	, _pPre(NULL)
	, _data(data)
	{}
	ListNode<T>* _pNext;
	ListNode<T>* _pPre;
	T            _data
};

template<class T, class Ref, class Ptr>
class _ListIterator_
{
	typedef _ListIterator_<T, Ref, Ptr> Self;
public:
	typedef T ValueType;
	typedef T& Reference;
	typedef T* Pointer;
	typedef int DifferenceType;
public:
	_ListIterator_()
		: _pNode(NULL)
	{}
	_ListIterator_(ListNode<T>* s)
		: _pNode(s._pNode)
	{}
	_ListIterator_(const Self& s)
		: _pNode(s._pNode)
	{}

	Ref operator*()
	{
		return _pNode->_data;
	}

	Pointer operator->()
	{
		return &(operator*());
	}

	Self& operator++()
	{
		_pNode = _pNode->_pNext;
		return *this;
	}

	Self operator++(int)
	{
		Self tmp(*this);
		_pNode = _pNode->_pNext;
		return tmp;
	}

	Self& operator--()
	{
		_pNode = _pNode->_pNext;
		return *this;
	}

	Self operator--(int)
	{
		Self tmp(*this);
		_pNode = _pNode->_pPre;
		return tmp;
	}

	bool operator!=(const Self& s)
	{
		return _pNode != s._pNode;
	}

	bool operator==(const Self& s)
	{
		return _pNode == s._pNode;
	}
private:
	ListNode<T>* _pNode;
};


template<class T, class Alloc = _Alloc>
class List
{
	typedef simple_alloc<ListNode<T>, Alloc> NodeAlloc;
public:
	typedef _ListIterator_<T, T&, T*> Iterator;
	typedef _ListIterator_<T, T&, T*> ConstIterator;

public:
	List()
	{
		_pHead = CreateNode();
		_pHead->_pNext = _pHead;
		_pHead->_pPre = _pHead;
	}

	void Clear()
	{
		Destroy(Begin(), End());
	}

	~List()
	{
		Clear();
		Destroy(&(_pHead)->_data);
	}

	void PushBack(const T& data)
	{
		ListNode<T>* NewNode = CreateNode(data);
		ListNode<T>* TailNode = _pHead->_pPre;

		TailNode->_pPre->_pNext = NewNode;
		NewNode->_pNext = _pHead;
		NewNode->_pPre = TailNode;
		_pHead->_pPre = NewNode;
	}

	void PopBack()
	{
		ListNode<T>* TailNode = _pHead->_pPre;
		if (TailNode->_pNext != _pHead)
		{
			TailNode->_pPre->_pNext = _pHead;
			_pHead->_pPre = TailNode->_pPre;
		}
		Destroy(TailNode);
	}

	void PushFront(const T& data)
	{
		ListNode<T>* NewNode = CreateNode(data);
		NewNode->_pNext = _pHead->_pNext;
		_pHead->_pNext = NewNode;
		NewNode->_pNext->_pPre = NewNode;
		NewNode->_pPre = _pHead;
	}

	void PopFront()
	{
		ListNode<T>* DelNode = _pHead->_pNext;
		if (DelNode != _pHead)
		{
			_pHead->_pNext = DelNode->_pNext;
			_pHead->_pNext->_pPre = _pHead;
			Destroy(DelNode);
		}
	}

	Iterator Begin()
	{
		return Iterator(_pHead->_pNext);
	}
	Iterator End()
	{
		return Iterator(_phead);
	}
	ReverseIterator RBegin()
	{
		return ReverseIterator(End());
	}

	ReverseIterator REnd()
	{
		return ReverseIterator(Begin());
	}
private:
	ListNode<T>* CreateNode(const T& data = T())
	{
		ListNode<T>* NewNode = NodeAlloc::allocate();
		Construct(NewNode, data);
		return NewNode;
	}
	void Destroy(ListNode<T>* p)
	{
		Destroy(&(p->_data));			//先清理数据所占的空间
		NodeAlloc::Destroy(p);			//再清理结点所占的空间
	}
private:
	ListNode<T>* _pHead;
};

void FunTest1()
{
	List<int> L;
	L.PushBack(1);
	L.PushBack(2);
	L.PushBack(3);
	L.PushBack(4);

	List<int>::Iterator it = L.Begin();
	while (it != L.End())
	{
		cout << *it << "->";
		++it;
	}
	cout << "NULL" << endl;

	//List<int>::ReverseIterator Rit = L.RBegin();
	//while (Rit != L.REnd())
	//{
	//	cout << *Rit << " ";
	//	++Rit;
	//}

	//cout << endl;

	//Distance(L.Begin(), L.End());
}

void FunTest2()
{
	List<string> L;
	L.PushBack("1111");
	L.PushBack("2222");
	L.PushBack("3333");
	L.PushBack("4444");

	List<string>::Iterator it = L.Begin();
	while (it != L.End())
	{
		cout << *it << "->";
		++it;
	}
	cout << "NULL" << endl;
}







