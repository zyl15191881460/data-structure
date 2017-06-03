#include "BTree.hpp"


//void Test1()
//{
//	BTree<int, 3> t;
//	t.Insert(10);
//	t.Insert(20);
//	t.Insert(30);
//	t.Insert(40);
//	t.Insert(50);
//	t.Insert(35);
//	t.Insert(38);
//	t.InOrder();
//}

void Test2()
{
	BTree<int, 3> t;
	t.Insert(53);
	t.Insert(75);
	t.Insert(139);
	t.Insert(49);
	t.Insert(145);
	t.Insert(36);
	t.Insert(101);
	t.InOrder();
}

int main()
{
	Test1();
	Test2();
	return 0;
}
