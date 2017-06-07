#include"HashTable.hpp"

void Test()
{
	int a[] = { 37, 25, 14, 36, 49, 68, 57, 11 };
	HashTable<int, int> ht;
	for (size_t index = 0; index < sizeof(a) / sizeof(a[0]); ++index)
	{
		ht.Insert(a[index], index);
	}
	ht.Insert(22, 9);
	ht.Insert(44, 10);
	ht.Insert(66, 11);
}

int main()
{
	Test();
	return 0;
}

