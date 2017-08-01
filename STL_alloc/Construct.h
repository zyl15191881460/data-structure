#include "IteratorTraits.h"
#include "TypeTraits.h"
#include <iostream>
using namespace std;

template <class T1, class T2>
inline void Construct(T1* p, const T2& value)
{
	new(p)T1(vlaue);		//��λnew���ʽ
}

template <class T>
inline void Destroy(T* pointer)
{
	if (pointer)
		pointer->~T();
}


template<class Iterator>
void _Destroy(Iterator first, Iterator lase, TrueType)
{
	cout << "O(1)" << endl;
}

template<class Iterator>
void _Destroy(Iterator first, Iterator last, FalseType)
{
	cout << "O(N)" << endl;
	while (first != last)
	{
		Destroy(&(*first));
		first++;
	}
}


template<class Iterator>
void Destroy(Iterator first, Iterator last)
{
	TypeTraits<IteratorTraits<Iterator>::ValueType>::hasTrivialDestructor Del;  //��'IteratorTraits<Iterator>::ValueType'�п���ȡ����������
	_Destroy(first, last, Del);
}
























