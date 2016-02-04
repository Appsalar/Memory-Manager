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
	if ((memSize / BlockSize) * sizeof(uint) > size - usedSpace)
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
				if (empty)
					empty = false;

				if (block[i] - totalMemory * HEADER <= 2 * HEADER)
				{
					usedSpace += block[i] - 2 * HEADER;
					++block[i];
					++block[i + totalMemory - 1];
				}
				else
				{
					usedSpace += totalMemory * HEADER;
					uint oldMem = block[i];
					block[i] = totalMemory * HEADER + 1;
					block[i + totalMemory - 1] = totalMemory * HEADER + 1;

					block[i + totalMemory] = oldMem - totalMemory * HEADER;
					block[i + oldMem / HEADER - 1] = oldMem - totalMemory * HEADER;
				}
				return (char*)(&block[i + 1]);
			}
		}
	}

	return NULL;
}


void Allocator::MyFree(void* p)
{
	uint* foo = (uint*)p;
	if (!p)
		return;

	if (!isAllocated(foo - 1) || !isAllocated(foo - 1 + *(foo - 1) / sizeof(uint)-1))
		throw "360 no skope";

	*(foo - 1) -= 1;
	*(foo - 1 + *(foo - 1) / sizeof(uint)-1) -= 1;
	usedSpace -= *(foo - 1) - 2 * HEADER;

	uint* bar = foo;

	if (!isAllocated(foo - 2) && foo - 1 != block)
	{
		//Put the size of the new block at the start of it
		*(foo - *(foo - 2) / sizeof(uint)-1) += *(foo - 1);

		//Put the size of the new block at the end of it
		*(foo - 1 + *(foo - 1) / sizeof(uint)-1) = *(foo - *(foo - 2) / sizeof(uint)-1);

		//Get a pointer to the new start
		bar = foo - *(foo - 2) / sizeof(uint)-1;
		usedSpace -= 2 * HEADER;
	}

	if (!isAllocated(foo - 1 + *(foo - 1) / sizeof(uint))
		&& foo - 1 + *(foo - 1) / sizeof(uint)-1 != block + size - 1)
	{
		//Get the size of the next empty block
		uint newTail = *(foo - 1 + *(foo - 1) / sizeof(uint)) / sizeof(uint);

		//Put the size of the new block at the end of it
		*(foo - 1 + *(foo - 1) / sizeof(uint)+newTail - 1) += *bar;

		//Put the size of the new block at the start of it
		*bar = *(foo - 1 + *(foo - 1) / sizeof(uint)+newTail - 1);
		usedSpace -= 2 * HEADER;
	}

	if (usedSpace == 2 * HEADER)
		empty = true;
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
