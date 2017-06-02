#include "RBTree-Iterator.hpp"

template<class K, class V>
class Map
{
	typedef	RBTree<K, V> RBType;
public:
	typename typedef RBTreeIterator<K, V, K&, K*> Iterator;
	//typename 让编译器知道Iterator是一个类型而非成员变量
	//用于指出模板声明(或定义)中的非独立名称(dependent names)是类型名，而非变量名。
public:
	Map()
		: t()
	{}

	Iterator Begin()
	{
		return t.Begin();
	}

	Iterator End()
	{
		return t.End();
	}
	Iterator rBegin()
	{
		return t.End(); //
	}
	Iterator rEnd()
	{
		return t.Begin();
	}
	size_t Size()
	{
		return t.Size();
	}
	bool  Insert(const K& key)
	{
		return t.Insert(key, key);
	}


private:
	RBType t;
};

