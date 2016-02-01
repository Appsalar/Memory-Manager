#include "Allocator.h"


Allocator::Allocator()
{
	size = 1000 * sizeof(double);
	block = new char[size];
	empty = true;
	CurrSize = size;
}


Allocator::~Allocator()
{
	delete[] block;
}


char* Allocator::MyMalloc(int memSize)
{
	return NULL;
}


void Allocator::MyFree(void* p)
{
}


bool Allocator::isAllocated(char* p)
{
	return false;
}


bool Allocator::isEmpty()
{
	if (empty)
		return true;
	else
		return false;
}
