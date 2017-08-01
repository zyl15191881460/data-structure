#pragma once

struct InputIteratorTag
{};
struct OutPutIteratorTag
{};

struct ForwardIteratorTag :public InputIteratorTag
{};

struct BidirectionalIteratorTag :public ForwardIteratorTag
{};

struct RandomAccessIteratorTag :public BidirectionalIteratorTag
{};

template<class Iterator>
struct IteratorTraits
{
	typename typedef Iterator::ValueType ValueType;		//对象的类型
	typename typedef Iterator::DifferenceType DifferenceType;		//迭代器类型
	typename typedef Iterator::Reference Reference;
	typename typedef Iterator::Pointer Pointer;
	typename typedef Iterator::IteratorCategory IteratorCategory;
};


template<class T>
struct IteratorTraits<T*>
{
	typedef T ValueType;
	typedef T& Reference;
	typedef T* Pointer;
	typedef int DifferenceType;
	typedef RandomAccessIteratorTag IteratorCategory;
};


template<class T>
struct IteratorTraits<const T*>
{
	typedef T ValueType;
	typedef const T& Reference;
	typedef const T* Pointer;
	typedef int DifferenceType;
	typedef RandomAccessIteratorTag IteratorCategory;
};


template<class Iterator>  //内置类型的指针可以直接返回他们直接的距离
int _Distance(Iterator first, Iterator last, RandomAccessIteratorTag)
{
	return last - first;
}

template<class InputIterator>	//自定义指针需要遍历才能返回大小
int _Distance(InputIterator first, InputIterator last, InputIteratorTag)
{
	int count = 0; 
	while (first != last)
	{
		cout++;
		first++;
	}
	return count;
}

template<class InputIterator>
int Distance(InputIterator first, InputIterator last)
{
	return _Distance(first, last, IteratorTraits<InputIterator>::IteratorCategory());
}


template<class Iterator>
class ReverseIterator
{
public:
	typename typedef Iterator::ValueType ValueType;
	typename typedef Iterator::DifferenceType DifferenceType;
	typename typedef Iterator::Reference Reference;
	typename typedef Iterator::Pointer Pointer;
	typename typedef Iterator::IteratorCategory IteratorCategory;
	
	typedef ReverseIterator<Iterator> Self;

public:
	ReverseIterator()
	{}
	ReverseIterator(const cur)
		: _cur(cur)
	{}
	ReverseIterator(const Self& s)
		: _cur(s._cur)
	{}

	Reference operator*()
	{
		Iterator it(_cur);
		return *(--it);
	}

	Pointer operator->()
	{
		return _cur.operator->();
	}

	Self& operator++()
	{
		_cur.operator--();
		return *this;
	}
	Self operator++(int)
	{
		Self tmp(*this);
		_cur.operator--();
		return tmp;
	}
	Self& operator--()
	{
		_cur.operator++();
		return *this;
	}
	Self operator--(int)
	{
		Self tmp(*this);
		_cur.operator++();
		return tmp;
	}

	bool operator==(const Self& s)
	{
		return _cur == s._cur;
	}
	bool operator!=(const Self& s)
	{
		return _cur != s._cur;
	}
private:
	Iterator _cur;
};






