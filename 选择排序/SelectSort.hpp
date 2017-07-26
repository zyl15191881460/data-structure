#include<iostream>
using namespace std;
#include<assert.h>

void Print(int* array, size_t size)
{
	for (size_t i = 0; i < size; i++)
		cout << array[i] << " ";
	cout << endl;
}

void SelectSort1(int* array, size_t size) // 单项选择排序
{
	for (size_t i = 0; i < size; i++)
	{
		int min = i;
		for (size_t j = i + 1; j < size; j++)
		{
			if (array[min] > array[j])
			{
				min = j;
			}
		}
		swap(array[i], array[min]);
	}
}

//void SelectSort2(int* array, size_t size)   //存在问题的
//{
//	int left = 0;
//	int right = size - 1;
//	while (left < right)
//	{
//		int min = left;
//		int max = right;
//		for (int i = left; i <= right; i++)
//		{
//			if (array[i] < array[min])
//				min = i;
//			if (array[i] > array[max])
//				max = i;
//		}
//		swap(array[min], array[left]);
//		swap(array[max], array[right]);
//		left++;
//		right--;
//	}
//}

void AdjustDown(int *array, size_t size, size_t pos)
{
	assert(array);
	size_t parent = pos;
	size_t child = parent * 2 + 1; //左孩子下标

	while (child < size)
	{
		if ((child + 1 < size) && array[child + 1] > array[child])
			child += 1;
		if (array[child] > array[parent])
			swap(array[child], array[parent]);
		parent = child;
		child = parent * 2 + 1;
	}
}


void HeapSort(int *array, int size)
{
	assert(array);
	//创建一个大堆
	for (int i = (size - 2) >> 1; i >= 0; i--)
	{
		AdjustDown(array, size, i);
	}

	for (int i = size - 1; i > 0; i--)
	{
		swap(array[0], array[i]);
		AdjustDown(array, i, 0);
	}
}


void Test()
{
	int array[] = { 1, 3, 5, 7, 9, 2, 4, 6, 8, 0 };
	Print(array, sizeof(array) / sizeof(array[0]));
//	SelectSort1(array, sizeof(array) / sizeof(array[0]));
//	SelectSort2(array, sizeof(array) / sizeof(array[0]));
	HeapSort(array, sizeof(array) / sizeof(array[0]));
	Print(array, sizeof(array) / sizeof(array[0]));
}