#include<iostream>
using namespace std;
#define _THROW_BAD_ALLOC bad_alloc


template<int inst>
class _Malloc_alloc_
{
private:
	static void* oom_malloc(size_t);
	static void* oom_realloc(void *, size_t);
	static void(*_malloc_alloc_oom_handler) ();  //用户自定义的一个处理函数(把申请的但没有用的空间释放掉)

public:
	static void* allocate(size_t n)
	{
		void *result = malloc(n);		//一级空间配置器直接调用malloc
		if (0 == result)
			result = oom_malloc(n);		//在申请失败后调用特殊函数进行处理；(解决了原来申请失败直接返回的问题)
		return result;
	}

	static void deallocate(void *p, size_t /* n */)
	{
		free(p);  //一级空间配置器直接调用free
	}

	static void* reallocate(void* p, size_t /* oldsize */, size_t new_size)
	{
		void *result = realloc(p, new_sz);
		if (0 == result)
			result = oom_realloc(p, new_size);
		return result;
	}

	static void (* set_malloc_handler(void (*f)()))()
	{
		void(*old)() = _malloc_alloc_oom_handler;
		_malloc_alloc_oom_handler = f;
		return old;
	}
};
template<int inst>
void(*_Malloc_alloc_<inst>::_malloc_alloc_oom_handler)() = 0;

template<int inst>
void *_Malloc_alloc_<inst>::oom_malloc(size_t n)
{
	void(*my_malloc_handler)();
	void* result;
	for (;;)  //死循环式的申请空间
	{
		my_malloc_handler = _malloc_alloc_oom_handler;
		if (0 == my_malloc_handler)
		{
			_THROW_BAD_ALLOC;
		}
		(*my_malloc_handler)(); //这个函数是用户自己实现的，把已经申请但没有用的空间释放掉，重新进行配置
		result = malloc(n);
		if (result) 
			return result;
	}
}

template<int inst>
void *_Malloc_alloc_<inst>::oom_realloc(void* p, size_t n)
{
	void (* my_malloc_handler)();
	void* result;
	for (;;)
	{
		my_malloc_handler = _malloc_alloc_oom_handler;
		if (0 == my_malloc_handler)
		{
			_THROW_BAD_ALLOC;
		}
		(*my_malloc_handler)();
		result = realloc(p, n);
		if (result)
			return result;
	}
}


enum {_ALIGN = 8};						//小型区块的上调边界
enum {_MAX_BYTES = 128};				//小型区块的上限
enum {_NFREELISTS = _MAX_BYTES/_ALIGN}; //free-list个数

template<bool threads, int inst>
class _Default_alloc_
{
public:
	static size_t ROUND_UP(size_t bytes)				//将bytes提升至8的倍数
	{
		return (((bytes)+_ALIGN - 1) & ~(_ALIGN - 1));
	}

private:
	union obj											//free_list的节点构造
	{
		union obj* free_list_link;
		char client_data[1];
	};
private:
	//每8个字节一个单位 8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128  16个free_lists
	static obj* volatile free_list[_NFREELISTS];
	static size_t FREELIST_INDEX(size_t bytes)			//根据区块的大小，决定使用第n号free_list。n从0开始
	{
		return (((bytes)+_ALIGN - 1) / _ALIGN - 1);
	}
	static void* refill(size_t n);						//返回一个用户所申请的空间，然后把从内存池中多拿的挂接在free_list中
	static char* chunk_alloc(size_t size, int& nobjs);	//内存池的大小影响nobjs的大小，所以nobjs必须是&
	static char* start_free;							//内存池的起始位置，只在chunk_alloc中变化
	static char* end_free;								//内存池的结束位置，只在chunk_alloc中变化
	static size_t heap_size;							//堆中已用申请元素的大小
public:
	static void* allocate(size_t n)
	{
		obj * volatile * my_free_list;
		obj * result;

		if (n > 128)
			return (_Malloc_alloc_<0>::allocate(n));
		my_free_list = free_list + FREELIST_INDEX(n);
		result = *my_free_list;
		if (result == 0)
		{
			void * r = refill(ROUND_UP(n));//由于free_list中没有数据，就由free_list向内存池中发出请求
			return r;
		}
		*my_free_list = result->free_list_link;
		return (result);
	}
	static void* deallocate(void* p, size_t size)
	{
		obj *q = (obj *)p;
		obj * volatile * my_free_list;
		if (n > (size_t _MAX_BYTES))					//大于128直接交由一级空间配置器解决；
	 	{
			_Malloc_alloc_<0>::deallocate(p, n);
			return;
		}
		my_free_list = free_list + FREELIST_INDEX(n);	//找到p所对应的free_list
		q->free_list_link = *my_free_list;				//用头插法把空间返回给free_list
		*my_free_list = q;
	}
	static void* reallocate(void* p, size_t old_sz, size_t new_sz){}
};
template<bool threads, int inst>
char *_Default_alloc_<threads, inst>::start_free = 0;

template<bool threads, int inst>
char *_Default_alloc_<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t _Default_alloc_<threads, inst>::heap_size = 0;

template<bool threads, int inst>
_Default_alloc_<false, 0>::obj* volatile _Default_alloc_<threads, inst>::free_list[_NFREELISTS] = { 0 };


template<bool threads, int inst>
void* _Default_alloc_<threads, inst>::refill(size_t n)
{
	int nobjs = 20;
	char* chunk = chunk_alloc(n, nobjs);			//向内存池中去取
	obj* volatile * my_free_list;
	obj* result;
	obj* current_obj, *next_obj;
	int i;
	if (1 == nobjs)									//只有一块的话，直接返回给用户，不需要再free_list中插入节点
		return chunk;
	my_free_list = free_list + FREELIST_INDEX(n);	//调整free_list准备插入新节点
	result = (obj*)chunk;							//保存一块返回给用户
	*my_free_list = next_obj = (obj*)(chunk + n);	//把取自内存池中的元素指向新分配的空间
	for (i = 1;; i++)								//从1开始，0返回给客户
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);		//每次next_obj的调整是根据n的偏移量来调整的
		if (nobjs - 1 == i)							
		{
			current_obj->free_list_link = 0;
			break;
		}
		else
		{
			current_obj->free_list_link = next_obj;
		}
	}
	return result;
}

template<bool threads, int inst>
char* _Default_alloc_<threads, inst>::chunk_alloc(size_t size, int& nobjs)
{
	char * result;
	size_t total_bytes = size*nobjs;
	size_t bytes_left = end_free - start_free;		//内存剩余空间
	
	if (bytes_left >= total_bytes)					//内存池完全满足所需
	{
		result = start_free;
		start_free += total_bytes;
		return(result);
	}
	else if (bytes_left >= size)					// 1< 内存池大小<nobjs
	{
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else											//内存池剩余空间一个都满足不了
	{
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		if (bytes_left > 0)							//可能造成的小内存无法满足size，要将它插入到free_list合适的位置，不然可能造成内存泄漏
		{
			obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj *)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj *)start_free;
		}
		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free)						//Heap中也没有空间了，这就很尴尬了；
		{
			int i;
			obj  * volatile * my_free_list, *p;
			for (i = size; i <= _MAX_BYTES; i += _ALIGN)   //在大于size的free_list中找是否存在申请了但没有用的空间
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p)									//free_list中有尚未使用的空间
				{
					*my_free_list = p->free_list_link;		//调整free_list释放出未用的区块
					start_free = (char*)p;
					end_free = start_free + i;
					return (chunk_alloc(size, nobjs));		//递归调用自己，为了修改nobjs;
					//注意，任何剩余的不能满足需求的剩余空间存于合适的free_list，避免内存泄漏
				}
			}
			end_free = 0;	//???
			start_free = (char *)_Malloc_alloc_<inst>::allocate(bytes_to_get);//调用一级空间配置器，已用my_malloc_handler尝试找空间
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return (chunk_alloc(size, nobjs));			//递归调用自己，修正 nobjs
	}
}






//int main()
//{
//	_Malloc_alloc_<0> m;
//	void * p = NULL;
//	for (size_t i = 0; i < 10; i++)
//	{
//		p = m.allocate(1);
//		printf("%d->%p  ", i, p);
//	}
//	_Default_alloc_<false, 0> d;
//	void * p = NULL;
//	for (size_t i = 0; i < 10; i++)
//	{
//		p = d.allocate(4);
//		printf("%d->%p  ", i, p);
//	}
//	return 0;
//}
