#pragma once

#include "../Allocator/Allocator.h"

template <class T>
class TempAllocator
{
public:
	typedef size_t    size_type;
	typedef ptrdiff_t difference_type;
	typedef T*        pointer;
	typedef const T*  const_pointer;
	typedef T&        reference;
	typedef const T&  const_reference;
	typedef T         value_type;

public:
	TempAllocator()
	{
		pMem = new Allocator(4 * 1000 * 1000);
	}

	TempAllocator(size_type size)
	{
		pMem = new Allocator(size / 8);
	}

	TempAllocator(const TempAllocator& other)
	{
		pMem = other.pMem;
	}

	template <class U> TempAllocator(const TempAllocator<U>& other)
	{
		pMem = other.pMem;
	}

	~TempAllocator() {}

	pointer allocate(size_t size)
	{
		pointer ptr = (pointer)pMem->MyMalloc(size * sizeof(T));
		return ptr;
	}

	void deallocate(pointer p, size_type n)
	{
		pMem->MyFree((char*)p);
	}
public:
	Allocator* pMem;
};


template <class T1, class T2>
bool operator==(const TempAllocator<T1>&, const TempAllocator<T2>&)
{
	return false;
}

template <class T1, class T2>
bool operator!=(const TempAllocator<T1>&, const TempAllocator<T2>&)
{
	return true;
}