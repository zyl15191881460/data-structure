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
//		size_t Hashaddress = Hash_Fun(key, i++);//˫ɢ��
		size_t Hashaddress = Hash_Fun(key);		//����

		size_t index = Hashaddress;
		while (_ht[index]._s == EXIST)
		{
			HashNode<K, V>& elem = _ht[index];
			if (elem._hn.first == key)
				return false;
			index++;
//		index = Hash_Fun(key, index++);
		if (index == _ht.size())//�����غ����ӵĴ��ڣ�������Զ�����������ԾͲ����ǳ�����ѭ�������
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
//		size_t Hashaddress = Hash_Fun(key, i++); //����̽��
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
			if (index == _ht.size())//���������һȦ�ص�ԭ������
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
	size_t Hash_Fun(K key)					//����̽���HashFun
	{
		return key%_ht.size();
	}
//	size_t Hash_Fun(size_t Size, size_t i)	//����̽��ʱ��HashFun
//	{
//		return (Size + (size_t)pow(i, 2)) % (_ht.size());
//	}
//	size_t Hash_Fun(K key, size_t i)		//˫ɢ�з���HashFun	
//	{
//		return(key % _ht.size() + (size_t)pow(i, 2)) % (_ht.size());
//	}
	void CheckCapacity()
	{
		if (_size == _ht.size())
			_ht.resize(SIZE);
		else if (_size * 10 / _ht.size() == 7) //�غ�����С��0.7����ͻ�ĸ��ʾͺ�С��
		{
			size_t NewSize = Prime_num(_ht.size() * 2);//���¿��ٵĿռ�Ҫ����������������������
			HashTable<K, V> ht(NewSize);
			for (size_t i = 0; i < _ht.size(); ++i)
			{
				if (_ht[i]._s == EXIST)//ֻ���ƴ��ڵ����Ϳ�����
					ht.Insert(_ht[i]._hn.first, _ht[i]._hn.second);
			}
//			swap(ht, _ht); //vector�⺯���������Ƚϴ�
			ht._ht.swap(_ht);//vector����д���ĺ����������Ƚ�С
		}
	}
	//�����������������
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

	bool Is_prime(size_t size)//�ж��Ƿ�Ϊ����
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
