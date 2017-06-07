#pragma once
#include<iostream>
using namespace std;
#include<vector>

enum State
{
	EMPTY,EXIST,DELETE
};

template<class K, class V>
struct HashNode
{
	pair<K, V> _hn;
	State _s;
	HashNode()
		:_s(EMPTY)
	{}
};

template<class K, class V, size_t SIZE = 11>
class HashTable
{
	typedef pair<HashNode<K, V>, bool> Pair;
public:
	HashTable()
		: _size(0)
	{}
	HashTable(size_t size)
		: _size(0)
	{
		_ht.resize(size);
	}

	bool Insert(const K& key, const V& value)
	{
		size_t i = 0;
		CheckCapacity();
//		size_t Hashaddress = Hash_Fun(key, i++);//双散列
		size_t Hashaddress = Hash_Fun(key);		//线性

		size_t index = Hashaddress;
		while (_ht[index]._s == EXIST)
		{
			HashNode<K, V>& elem = _ht[index];
			if (elem._hn.first == key)
				return false;
			index++;
//		index = Hash_Fun(key, index++);
		if (index == _ht.size())//有了载荷因子的存在，数据永远不会满，所以就不考虑出现死循环的情况
			index = 0;
		}
		_ht[index]._hn.first = key;
		_ht[index]._hn.second = value;
		_ht[index]._s = EXIST;
		_size++;
	}

	Pair Find(const K& key)
	{
		size_t i = 0;
//		size_t Hashaddress = Hash_Fun(key, i++); //线性探测
		size_t Hashaddress = Hash_Fun(key);

		size_t index = Hashaddress;
		while (_ht[index]._s != EMPTY)
		{
			HashNode<K, V>& elem = _ht[index];
			if (elem._hn.first == key)
			{
				if (elem._s == EXIST)
					return Pair(&elem, true);
				else
					return Pair(NULL, false);
			}
			index++;  
//			index = Hash_Fun(key, i++);
			if (index == _ht.size())//不会出现找一圈回到原点的情况
				index = 0;
		}
		return Pair(NULL, false);
	}

	bool Remove(const K& key)
	{
		Pair hd = Find(key);
		if (hd.second)
		{
			hd.first._s = DELETE;
			_size--;
			return true;
		}
		return false;
	}
private:
	size_t Hash_Fun(K key)					//线性探查的HashFun
	{
		return key%_ht.size();
	}
//	size_t Hash_Fun(size_t Size, size_t i)	//二次探测时的HashFun
//	{
//		return (Size + (size_t)pow(i, 2)) % (_ht.size());
//	}
//	size_t Hash_Fun(K key, size_t i)		//双散列法求HashFun	
//	{
//		return(key % _ht.size() + (size_t)pow(i, 2)) % (_ht.size());
//	}
	void CheckCapacity()
	{
		if (_size == _ht.size())
			_ht.resize(SIZE);
		else if (_size * 10 / _ht.size() == 7) //载荷因子小于0.7，冲突的概率就很小；
		{
			size_t NewSize = Prime_num(_ht.size() * 2);//重新开辟的空间要是质数，调用质数函数；
			HashTable<K, V> ht(NewSize);
			for (size_t i = 0; i < _ht.size(); ++i)
			{
				if (_ht[i]._s == EXIST)//只搬移存在的数就可以了
					ht.Insert(_ht[i]._hn.first, _ht[i]._hn.second);
			}
//			swap(ht, _ht); //vector外函数，开销比较大
			ht._ht.swap(_ht);//vector内重写过的函数，开销比较小
		}
	}
	//求离容量最近的质数
	size_t Prime_num(size_t size)
	{
		size_t Size = size;
		while (1)
		{
			if (Is_prime(Size))
				return Size;
			++Size;
		}
	}

	bool Is_prime(size_t size)//判断是否为质数
	{
		size_t i = 2;
		while (i < sqrt(size))
		{
			if (size%i == 0)
				return false;
			else
				i++;
		}
		return true;
	}

private:
	size_t _size;
	vector<HashNode<K, V> > _ht;
};
