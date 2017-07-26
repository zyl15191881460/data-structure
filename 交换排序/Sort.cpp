#include<iostream>
using namespace std;
#include<assert.h>
#include<stack>


//3��ð������
void BulleSort1(int* array, size_t size)  //���� ʱ�临�Ӷ�ΪO(N^2)
{
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size - i - 1; j++)
		{
			if (array[j] > array[j + 1])
				swap(array[j], array[j + 1]);
		}
	}
}

void BulleSort2(int* array, size_t size)
{
	int  j, k;
	bool flag;
	flag = true;
	k = size;
	while (flag)
	{
		flag = false;
		for (j = 1; j < size; j++)
		{
			if (array[j] < array[j - 1])
			{
				swap(array[j], array[j - 1]);
				flag = true;
			}
		}
		k--;
	}
}

void BulleSort3(int *array, size_t size)
{
	int j, k;
	int flag = size;
	while (flag > 0)
	{
		k = flag;
		flag = 0;
		for (j = 1; j < size; j++)
		{
			if (array[j] < array[j - 1])
			{
				swap(array[j], array[j - 1]);
				flag = j;
			}
		}
	}
}

void Print(int *array, size_t size)
{
	for (int i = 0; i < size; i++)
	{
		cout << array[i] << " ";
	}
	cout << endl;
}

int GetMidIndex(int* array, int left, int right);


//����ָ�뷨 
int PortSort1(int *array, int left, int right)
{
	int mid = GetMidIndex(array, left, right);
	swap(array[mid], array[right]);
	assert(array);
	int key = right;
	while (left < right)
	{
		while ((left < right) && (array[left] <= array[key]))
			++left;
		while ((left < right) && (array[right] >= array[key]))
			--right;
		if (array[left] != array[right])
			swap(array[left], array[right]);
	}

	swap(array[left], array[key]);
	return left;
}

//�ڿӷ�
int PortSort2(int* array, int left, int right)
{
	int mid = GetMidIndex(array, left, right);
	swap(array[mid], array[right]);
	int hole = array[right];
	while (left < right)
	{
		while (left < right && array[left] <= hole)
		{
			left++;
		}
		if (left < right)
			array[right--] = array[left];
		while (left < right && array[right] >= hole)
		{
			right--;
		}
		if (left < right)
			array[left++] = array[right];
	}
	array[left] = hole;
	return left;
}

//ǰ��ָ�뷨
int PortSort3(int *array, int left, int right)
{
	int mid = GetMidIndex(array, left, right);
	swap(array[mid], array[right]);
	int pcur = left;
	int prev = pcur - 1;
	int key = array[right];
	while (pcur != right)
	{
		if (array[pcur] < key && array[++prev] != array[pcur])
		{
			swap(array[pcur], array[prev]);
		}
		++pcur;
	}
	swap(array[++prev], array[pcur]);
	return prev;
}

//�Ż�1�� ����ȡ�з�,����ʲô����£�����ȥ�Ķ��Ǵ�С���е�ֵ
int GetMidIndex(int* array, int left, int right)
{
	int mid = left + ((right - left) >> 1);
	if (array[left] < array[mid])
	{
		if (array[mid] < array[right])
			return mid;
		else if (array[left] < array[right])
			return right;
		else
			return left;
	}
	else
	{
		if (array[mid] > array[right])
			return mid;
		else if (array[left] > array[right])
			return right;
		else
			return left;
	}
}
//�Ż�2�� �����ֵ������к�С��ʱ��(һ����ΪС��13��Ԫ������ʱ)��������ʹ�ÿ����������ЩС�������򷴶�����ֱ�Ӳ��������Ч��\
��Ϊ���������������л���������һ�Ŷ�����һ����������С��13��Ԫ��ʱ������ʹ�ÿ��ŵĻ����൱�������˶���������󼸲�Ľ����Ŀ��\
�����˵ݹ�Ĵ��������������ڵ�������С��13��Ԫ�ص�ʱ��͸���ֱ�Ӳ�������������Щ�����н�������

void InsertSort(int *array, int n)
{
	int end = 0;
	for (size_t i = 1; i < n; i++)
	{
		int tmp = array[i];
		end = i - 1;
		while (end >= 0)
		{
			if (array[end] > tmp)
			{
				array[end + 1] = array[end];
				--end;
			}
		}
		array[end + 1] = tmp;
	}
}

//�Ż�3�� ������̫��ʱ������ɵݹ�����̫��Ч�ʾ�̫�ͣ����Կ�������ջ��ʵ��
void QuickSortNodeR(int* array, int left, int right)
{
	stack<int> s;
	s.push(right);
	s.push(left);

	while (!s.empty())
	{
		int start = s.top();
		s.pop();
		int end = s.top();
		s.pop();

		if (end > start)
		{
			int div = PortSort1(array, start, end);
			s.push(div - 1);
			s.push(start);

			s.push(right);
			s.push(div + 1);
		}

	}
}
 
void TestQuickSort(int* array, int left, int right)
{
	if (left < right)
	{
		int ret = PortSort1(array, left, right);
		TestQuickSort(array, left, ret - 1);
		TestQuickSort(array, ret + 1, right);
		//int ret = PortSort2(array, left, right);
		//TestQuickSort(array, left, ret - 1);
		//TestQuickSort(array, ret + 1, right);
		//int ret = PortSort3(array, left, right);
		//TestQuickSort(array, left, ret - 1);
		//TestQuickSort(array, ret + 1, right);
	}
}






int main()
{
	int array[] = { 1, 3, 0, 7, 9, 2, 4, 6, 8, 5 };
	Print(array, sizeof(array) / sizeof(array[0]));
	BulleSort1(array, sizeof(array) / sizeof(array[0]));
//	BulleSort2(array, sizeof(array) / sizeof(array[0]));
//	BulleSort3(array, sizeof(array) / sizeof(array[0]));
//	TestQuickSort(array, 0, sizeof(array) / sizeof(array[0]) - 1);
//	QuickSortNodeR(array, 0, sizeof(array) / sizeof(array[0]) - 1);
	Print(array, sizeof(array) / sizeof(array[0]));
	return 0;
}