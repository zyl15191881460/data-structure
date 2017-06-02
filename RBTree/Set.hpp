#include "RBTree-Iterator.hpp"

template<class K>
class Set
{
	typedef RBTree<K, K> RBType;
public:
	typename typedef RBTreeIterator<K, K, K&, K*> Iterator;
public:
	Set()
		: s()
	{}

	Iterator Begin()
	{
		return s.Begin();
	}
	Iterator End()
	{
		return s.End();
	}

	Iterator rBegin()
	{
		return s.End();
	}
	Iterator rEnd()
	{
		return s.Begin();
	}

	bool Empty()
	{
		return s.Empty();
	}
	size_t Size()
	{
		return s.Size();
	}
	bool Insert(const K& key)
	{
		return s.Insert(key, key);
	}
private:
	RBType s;
};