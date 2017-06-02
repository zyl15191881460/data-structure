//#include "RBTree.hpp"
//#include "RBTree-Iterator.hpp"
//#include "Map.hpp"
#include "Set.hpp"


//void Test1()
//{
//	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
//	RBTree<int, int> t;
//	for (int idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
//		t.Insert(a[idx], idx);
//
//	t.InOrder();
//
//	t.CheckRBTree();
//}



//void Test2()
//{
//	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
//	RBTree<int, int> t;
//	for (int idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
//		t.Insert(a[idx], idx);
//	t.InOrder();
//
//	RBTree<int, int>::Iterator it;
//	it = t.Begin();
//	while (it != t.End())
//	{
//		cout << *it << " ";
//		++it;
//	}
//	cout << endl;
//
//}


//void Test3()
//{
//	Map<int, int> m;
//	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
//	for (size_t idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
//		m.Insert(a[idx]);
//	Map<int, int>::Iterator it = m.Begin();
//	Map<int, int>::Iterator it1 = m.End();
//	while (it != it1)
//	{
//		cout << *it << " ";
//		it++;
//	}
//	cout << endl;
//	it1--;
//	cout << *it1 << endl;
//
//	cout << m.Size() << endl;
//
//	Map<int, int>::Iterator it2 = m.rBegin();
//	Map<int, int>::Iterator it3 = m.rEnd();
//	while (it2 != it3)
//	{
//		it2--;
//		cout << *it2 << " ";
//	}
//
//}

void Test4()
{
	Set<int> s;
	s.Insert(1);
	s.Insert(2);
	s.Insert(3);
	s.Insert(4);
	s.Insert(5);
	s.Insert(6);

	Set<int>::Iterator it = s.Begin();
	Set<int>::Iterator it1 = s.End();

	while (it != it1)
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	it1--;
	cout << *it1 << endl;
	if (s.Empty())
		cout << "Пе" << endl;
	else
		cout << "Тњ" << endl;

	Set<int>::Iterator it2 = s.rBegin();
	Set<int>::Iterator it3 = s.rEnd();
	it2--;
	cout << *(it2) << endl;
	cout << *(it3) << endl;
}

int main()
{
//	Test1();
//	Test2();
//	Test3();
	Test4();
	return 0;
}