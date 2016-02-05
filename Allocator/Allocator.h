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
	bool full;
public:
	Allocator(size_t size, size_t BlockSize = 8);
	virtual ~Allocator();

	//if invalid argument or no space
	//return NULL 
	void* MyMalloc(const int memSize);

	//if invalid argument 
	//undefined behavior
	void MyFree(void* p);

	inline bool isEmpty();
	inline bool isFromMe(void* p);
	inline bool isFull();
private:
	inline bool isAllocated(uint* p);
};

