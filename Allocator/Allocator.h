#pragma once
#include <iostream>

#ifdef ALLOCATOR_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

typedef unsigned __int64 uint;

class EXPORT Allocator
{
private:
	uint* block;
	size_t size;
	size_t BlockSize;
	size_t usedSpace;
	bool empty;
public:
	Allocator(size_t size, size_t BlockSize = 8);
	virtual ~Allocator();

	char* MyMalloc(const int memSize);
	void MyFree(void* p);
private:
	inline bool isAllocated(uint* p);
public:
	inline bool isEmpty();
};

