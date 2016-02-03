#include "Allocator.h"

#define HEADER sizeof(uint)

//Creating allocator with capacity of
//size * sizeof(double) bites
Allocator::Allocator(size_t size, size_t BlockSize)
{
	this->size = size * sizeof(uint);
	block = new uint[size];
	empty = true;
	usedSpace = 2 * HEADER;
	this->BlockSize = BlockSize;

	block[0] = this->size;
	block[size - 1] = this->size;
}


Allocator::~Allocator()
{
	delete[] block;
}


char* Allocator::MyMalloc(const int memSize)
{
	if (memSize <= 0)
		return NULL;

	//Calculate adjustment needed to keep object correctly aligned 
	size_t adjustment = memSize % BlockSize ? 1 : 0;
	adjustment += 2;

	size_t totalMemory = memSize / BlockSize + adjustment;

	//Check if there is enough memory 
	if (totalMemory * sizeof(uint) > size - usedSpace)
		return NULL;

	size_t indexMem = size / HEADER;

	//Try to find a block to fit the allocation
	size_t i = 0;
	while (i < indexMem - 2)
	{
		if (isAllocated(&block[i]))
		{
			i += (size_t)block[i] / HEADER;
		}
		else
		{
			if (block[i] < totalMemory)
				i += (size_t)block[i] / HEADER;
			else
			{
				uint oldMem = block[i];
				block[i] = totalMemory * HEADER + 1;
				block[i + totalMemory - 1] = totalMemory * HEADER + 1;

				block[i + totalMemory] = oldMem - totalMemory * HEADER;
				block[i + oldMem / HEADER - 1] = oldMem - totalMemory * HEADER;
				return (char*)(&block[i + 1]);
			}
		}
	}

	return NULL;
}


void Allocator::MyFree(void* p)
{
	throw "360 no skope";
}


inline bool Allocator::isAllocated(uint* p)
{
	if (*p & 1)
		return true;
	else
		return false;
}


inline bool Allocator::isEmpty()
{
	if (empty)
		return true;
	else
		return false;
}
