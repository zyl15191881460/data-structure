#include "RBTree-Iterator.hpp"

template<class K, class V>
class Map
{
	typedef	RBTree<K, V> RBType;
public:
	typename typedef RBTreeIterator<K, V, K&, K*> Iterator;
	//typename �ñ�����֪��Iterator��һ�����Ͷ��ǳ�Ա����
	//����ָ��ģ������(����)�еķǶ�������(dependent names)�������������Ǳ�������
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

