#pragma once
#include <iostream>

#ifdef ALLOCATOR_EXPORTS
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

class EXPORT Allocator
{
private:
	char* block;
	size_t size;
	size_t CurrSize;
	bool empty;
public:
	Allocator();
	virtual ~Allocator();

	char* MyMalloc(int memSize);
	void MyFree(void* p);
private:
	bool isAllocated(char* p);
public:
	bool isEmpty();
};

