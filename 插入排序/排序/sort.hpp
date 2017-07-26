#include <iostream>
using namespace std;

//version1 时间复杂度O(N^2)
void InsertSort(int *array, size_t size)
{
	for (size_t idx = 1; idx < size; ++idx)
	{
		int end = idx - 1;
		int key = array[idx];
		while (end >= 0 && (key < array[end]))
		{
			array[end + 1] = array[end];
			--end;
		}
		array[end + 1] = key;
	}
}

//version2 时间复杂度降低，利用二分查找来找位置
void InsertSort_OP(int *array, size_t size)
{
	for (size_t idx = 1; idx < size; ++idx)
	{
		int end = idx - 1;
		int left = 0;
		int right = idx;
		int key = array[idx];
		while (left <= right)
		{
			int mid = left + ((right - left) >> 1);
			if (key < array[mid])
				right = mid - 1;
			else
				left = mid + 1;
		}
		while (end >= left)
		{
			array[end + 1] = array[end];
			--end;
		}
		array[end + 1] = key;
	}
}

//希尔排序  时间复杂度在N^1.25～1.6N^1.25
void ShellSort(int *array, size_t size)
{
	int gap = size;
	while (gap > 1)
	{
		gap = gap / 3 + 1;
		for (size_t idx = gap; idx < size; ++idx)
		{
			int end = idx - gap;
			int key = array[idx];
			while (end >= 0 && (key < array[end]))
			{
				array[end + gap] = array[end];
				end -= gap;
			}
			array[end + gap] = key;
		}
	}
}
void Print(int *array, size_t size)
{
	for (size_t i = 0; i < size; i++)
		cout << array[i] << " ";
	cout << endl;
}

void Test()
{
	int array[] = { 2, 5, 4, 9, 3, 6, 8, 7, 1, 0 };
	Print(array, sizeof(array) / sizeof(array[0]));
//	InsertSort(array, sizeof(array) / sizeof(array[0]));
//	InsertSort_OP(array, sizeof(array) / sizeof(array[0]));
	ShellSort(array, sizeof(array) / sizeof(array[0]));

	Print(array, sizeof(array) / sizeof(array[0]));
}