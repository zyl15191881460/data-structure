//#include"BSTree.hpp"
#include "BSTree-Iterator.hpp"

// 测试非递归的三种情况
//void Test1()
//{
//	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
//	BinarySearchTree<int, int> bs;`
//	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
//	{
//		bs.Insert_Nor(a[idx], a[idx]);
//	}
////	BinarySearchTree<int, int> bc(bs);
//	bs.InOrder();
//	cout << bs.Find_Nor(7) << endl;
//	bs.Remove_Nor(3);
//	bs.InOrder();
//}

//测试递归的三种情况
//void Test2()
//{
//	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
//	BinarySearchTree<int, int>bs;
//	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
//	{
//		bs.Insert(a[idx], a[idx]);
//	}
//	bs.InOrder();
//	cout << bs.Find(4) << endl;
//	bs.Remove(5);
//	bs.InOrder();
//}

void Test3()
{
	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	BinarySearchTree<int, int>bs;
	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); idx++)
	{
		bs.Insert_Nor(a[idx], a[idx]);
	}

	BinarySearchTree<int, int>::Iterator it;
	it = bs.Begin();
	while (it != bs.End())
	{
		cout << it.operator*() << " ";
		++it;
		//it++;
	}
}

//int Test4()
//{
//	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
//	BinarySearchTree<int, int> bs;
//	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); idx++)
//	{
//		bs.Insert_Nor(a[idx], a[idx]);
//	}
//	bs.TreeToList();
//	return 0;
//}

int main()
{
//	Test1();
//	Test2();
	Test3();
//	Test4();
	return 0;
}