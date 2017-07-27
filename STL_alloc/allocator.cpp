#include<iostream>
using namespace std;
#define _THROW_BAD_ALLOC bad_alloc


template<int inst>
class _Malloc_alloc_
{
private:
	static void* oom_malloc(size_t);
	static void* oom_realloc(void *, size_t);
	static void(*_malloc_alloc_oom_handler) ();  //�û��Զ����һ��������(������ĵ�û���õĿռ��ͷŵ�)

public:
	static void* allocate(size_t n)
	{
		void *result = malloc(n);		//һ���ռ�������ֱ�ӵ���malloc
		if (0 == result)
			result = oom_malloc(n);		//������ʧ�ܺ�������⺯�����д���(�����ԭ������ʧ��ֱ�ӷ��ص�����)
		return result;
	}

	static void deallocate(void *p, size_t /* n */)
	{
		free(p);  //һ���ռ�������ֱ�ӵ���free
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
	for (;;)  //��ѭ��ʽ������ռ�
	{
		my_malloc_handler = _malloc_alloc_oom_handler;
		if (0 == my_malloc_handler)
		{
			_THROW_BAD_ALLOC;
		}
		(*my_malloc_handler)(); //����������û��Լ�ʵ�ֵģ����Ѿ����뵫û���õĿռ��ͷŵ������½�������
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


enum {_ALIGN = 8};						//С��������ϵ��߽�
enum {_MAX_BYTES = 128};				//С�����������
enum {_NFREELISTS = _MAX_BYTES/_ALIGN}; //free-list����

template<bool threads, int inst>
class _Default_alloc_
{
public:
	static size_t ROUND_UP(size_t bytes)				//��bytes������8�ı���
	{
		return (((bytes)+_ALIGN - 1) & ~(_ALIGN - 1));
	}

private:
	union obj											//free_list�Ľڵ㹹��
	{
		union obj* free_list_link;
		char client_data[1];
	};
private:
	//ÿ8���ֽ�һ����λ 8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128  16��free_lists
	static obj* volatile free_list[_NFREELISTS];
	static size_t FREELIST_INDEX(size_t bytes)			//��������Ĵ�С������ʹ�õ�n��free_list��n��0��ʼ
	{
		return (((bytes)+_ALIGN - 1) / _ALIGN - 1);
	}
	static void* refill(size_t n);						//����һ���û�������Ŀռ䣬Ȼ��Ѵ��ڴ���ж��õĹҽ���free_list��
	static char* chunk_alloc(size_t size, int& nobjs);	//�ڴ�صĴ�СӰ��nobjs�Ĵ�С������nobjs������&
	static char* start_free;							//�ڴ�ص���ʼλ�ã�ֻ��chunk_alloc�б仯
	static char* end_free;								//�ڴ�صĽ���λ�ã�ֻ��chunk_alloc�б仯
	static size_t heap_size;							//������������Ԫ�صĴ�С
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
			void * r = refill(ROUND_UP(n));//����free_list��û�����ݣ�����free_list���ڴ���з�������
			return r;
		}
		*my_free_list = result->free_list_link;
		return (result);
	}
	static void* deallocate(void* p, size_t size)
	{
		obj *q = (obj *)p;
		obj * volatile * my_free_list;
		if (n > (size_t _MAX_BYTES))					//����128ֱ�ӽ���һ���ռ������������
	 	{
			_Malloc_alloc_<0>::deallocate(p, n);
			return;
		}
		my_free_list = free_list + FREELIST_INDEX(n);	//�ҵ�p����Ӧ��free_list
		q->free_list_link = *my_free_list;				//��ͷ�巨�ѿռ䷵�ظ�free_list
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
	char* chunk = chunk_alloc(n, nobjs);			//���ڴ����ȥȡ
	obj* volatile * my_free_list;
	obj* result;
	obj* current_obj, *next_obj;
	int i;
	if (1 == nobjs)									//ֻ��һ��Ļ���ֱ�ӷ��ظ��û�������Ҫ��free_list�в���ڵ�
		return chunk;
	my_free_list = free_list + FREELIST_INDEX(n);	//����free_list׼�������½ڵ�
	result = (obj*)chunk;							//����һ�鷵�ظ��û�
	*my_free_list = next_obj = (obj*)(chunk + n);	//��ȡ���ڴ���е�Ԫ��ָ���·���Ŀռ�
	for (i = 1;; i++)								//��1��ʼ��0���ظ��ͻ�
	{
		current_obj = next_obj;
		next_obj = (obj*)((char*)next_obj + n);		//ÿ��next_obj�ĵ����Ǹ���n��ƫ������������
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
	size_t bytes_left = end_free - start_free;		//�ڴ�ʣ��ռ�
	
	if (bytes_left >= total_bytes)					//�ڴ����ȫ��������
	{
		result = start_free;
		start_free += total_bytes;
		return(result);
	}
	else if (bytes_left >= size)					// 1< �ڴ�ش�С<nobjs
	{
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return (result);
	}
	else											//�ڴ��ʣ��ռ�һ�������㲻��
	{
		size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
		if (bytes_left > 0)							//������ɵ�С�ڴ��޷�����size��Ҫ�������뵽free_list���ʵ�λ�ã���Ȼ��������ڴ�й©
		{
			obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
			((obj *)start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj *)start_free;
		}
		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free)						//Heap��Ҳû�пռ��ˣ���ͺ������ˣ�
		{
			int i;
			obj  * volatile * my_free_list, *p;
			for (i = size; i <= _MAX_BYTES; i += _ALIGN)   //�ڴ���size��free_list�����Ƿ���������˵�û���õĿռ�
			{
				my_free_list = free_list + FREELIST_INDEX(i);
				p = *my_free_list;
				if (0 != p)									//free_list������δʹ�õĿռ�
				{
					*my_free_list = p->free_list_link;		//����free_list�ͷų�δ�õ�����
					start_free = (char*)p;
					end_free = start_free + i;
					return (chunk_alloc(size, nobjs));		//�ݹ�����Լ���Ϊ���޸�nobjs;
					//ע�⣬�κ�ʣ��Ĳ������������ʣ��ռ���ں��ʵ�free_list�������ڴ�й©
				}
			}
			end_free = 0;	//???
			start_free = (char *)_Malloc_alloc_<inst>::allocate(bytes_to_get);//����һ���ռ�������������my_malloc_handler�����ҿռ�
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return (chunk_alloc(size, nobjs));			//�ݹ�����Լ������� nobjs
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
