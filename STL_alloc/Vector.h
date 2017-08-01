#pragma once
#include "allocator.h"
#include "Construct.h"
#include <iostream>
#include <string>
using namespace std;

template<class T, class Alloc = _Alloc>
class Vector
{
	typedef Simple_alloc<T, Alloc> dataAlloc;
public:
	typedef T* Iterator;
	typedef ReverseIterator<Iterator> ReverseIterator;
	typedef T ValueType;
public:
	Vector()
		: _start(NULL)
		, _finsh(NULL)
		, _endofStorage(NULL)
	{}
	Vector(size_t n, const T& data)
	{
		_start = dataAlloc::allocate(n);
		for (size_t i = 0; i < n; i++)
			Construct(_start + i, data);
		_finsh = _start + n;
		_endofStorage = _finsh;
	}
	~Vector()
	{
		Clear();
		dataAlloc::deallocate(_start, size());
	}

	void PushBack(const T& data)
	{
		_CheckCapacity();
		Construct(_finsh, data);
		++_finsh;
	}

	void PopBack()
	{
		assert(_start == _finsh);
		Destroy(_finsh - 1);
		--_finsh;
	}

	void Insert(Iterator pos, const T& data)
	{
		assert(!(pos >= Begin() && pos <= End()));
		_CheckCapacity();
		for (Iterator it = _finsh; it > pos; --it)
			*it = *(it - 1);
		*pos = data;
		_finish++;
		return pos;
	}
	Iterator Erase(Iterator pos)
	{
		assert(!(pos >= Begin() && pos <= End()));
		for (Iterator it = pos; it < _finsh - 1; ++it)
			*it = *(it + 1);
		Destroy(_finsh - 1);
		_finish--;
		return pos;
	}

	void Clear()
	{
		Destroy(_start, _finsh);
		_finsh = _start;
	}

	size_t Size()
	{
		return _finsh - _start;
	}
	size_t Capacity()
	{
		return _endofStorage - _start;
	}
	bool Empty()const
	{
		return NULL == _start;
	}
	void ReSize(size_t NewSize, const T& data = T())
	{
		if (NewSize < Size())
		{
			Destroy(_start + NewSize, _finsh);
			_finsh = _start + NewSize;
		}
		else if (_start + NewSize < _endofStorage)
		{
			for (size_t i = Size(); i < NewSize; ++i)
				Construct(_start + idx, data);
		}
		else
		{
			Iterator tmp = dataAlloc::allocate(NewSize);
			size_t OldSize = Size();

			for (size_t i = 0; i < OldSize; i++)
			{
				Construct(tmp + i, _start + i);
			}
			Destroy(_start, _finish);
			dataAlloc::deallocate(_start, Capacity());

			for (size_t i = OldSize; i < NewSize; i++)
				Construct(tmp + i, data);

			_start = tmp;
			_finsh = _start + NewSize;
			_endofStorage = _finsh;
		}
	}

	Iterator Begin()
	{
		return _start;
	}
	Iterator End()
	{
		return _finish;
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
	void _CheckCapacity()
	{
		if (_finsh == _endofStorage)
		{
			size_t OldSize = Size();
			size_t NewSize = OldSize * 2 + 3;
			Iterator tmp = dataAlloc::allocate(NewSize);

			for (size_t i = 0; i < OldSize; i++)
				Construct(tmp + i, _start[i]);

			if (_start)
			{
				Destroy(_start, _finsh);
				dataAlloc::deallocate(_start, OldSize);
			}

			_start = tmp;
			_finsh = tmp + OldSize;
			_endofStorage = tmp + NewSize;
		}
	}
private:
	Iterator _start;
	Iterator _finsh;
	Iterator _endofStorage;
};