#include "Allocator.h"
#include <assert.h>

#define HEADER sizeof(uint)

//Creating allocator with capacity of
//size * sizeof(double) bites
Allocator::Allocator(size_t size, size_t BlockSize)
{
	this->size = size * sizeof(uint);
	block = new uint[size];
	empty = true;
	full = false;
	usedSpace = 2 * HEADER;
	this->BlockSize = BlockSize;

	//Set the header and the footer
	block[0] = this->size;
	block[size - 1] = this->size;
}


Allocator::~Allocator()
{
	delete[] block;
}


void* Allocator::MyMalloc(const int memSize)
{
	//if invalid argument 
	//return NULL 
	if (memSize <= 0)
		return NULL;

	//Calculate adjustment needed to keep object correctly aligned 
	size_t adjustment = memSize % BlockSize ? 1 : 0;
	adjustment += 2;

	//Calculate the total memory that could be allocated
	size_t totalMemory = memSize / BlockSize + adjustment;

	//Check if there is enough memory 
	if ((memSize / BlockSize) * sizeof(uint) > size - usedSpace)
		return NULL;

	size_t indexMem = size / sizeof(uint);

	//Try to find a block to fit the allocation
	size_t i = 0;
	while (i < indexMem - 2)
	{
		//If the current block is allocated go to the next one
		if (isAllocated(&block[i]))
		{
			i += (size_t)block[i] / sizeof(uint);
		}
		else
		{
			//If there is not enough space in the current block 
			//go to the next one
			if (block[i] < totalMemory)
				i += (size_t)block[i] / sizeof(uint);
			else
			{
				if (empty)
					empty = false;

				//If after the current allocation there won't be any 
				//place for more allocations in that block, allocate it 
				if (block[i] - totalMemory * sizeof(uint) <= 2 * HEADER)
				{
					//Update the capacity 
					usedSpace += block[i] - 2 * HEADER;

					//Set the block as allocated
					++block[i];
					++block[i + totalMemory - 1];
				}
				else
				{
					//Update the capacity 
					usedSpace += totalMemory * HEADER;

					//Get the old size of the block
					uint oldMem = block[i];

					//Set the block as allocated
					block[i] = totalMemory * HEADER + 1;
					block[i + totalMemory - 1] = totalMemory * HEADER + 1;

					//Update the new free block in the block after the allocation
					block[i + totalMemory] = oldMem - totalMemory * HEADER;
					block[i + oldMem / HEADER - 1] = oldMem - totalMemory * HEADER;
				}
				//Check if the allocator is full
				if (usedSpace == size)
					full = true;

				//return pointer to the first 8 bits after the header
				return (void*)(&block[i + 1]);
			}
		}
	}

	return NULL;
}


void Allocator::MyFree(void* p)
{
	//if NULL 
	//return
	if (!p)
		return;

	uint* pTarget = (uint*)p;

	size_t thisSize = *(pTarget - 1) / sizeof(uint);

	//throw sth if the block doesn't pass for an allocated one
	// still there could be an invalid argument which passes 
	// in that case the behavior is undefined
	if (!isAllocated(pTarget - 1) || !isAllocated(pTarget - 1 + thisSize - 1)
		|| *(pTarget - 1) > size)
		throw "360 no skope";

	//Set the block as free
	*(pTarget - 1) -= 1;
	*(pTarget - 1 + thisSize-1) -= 1;

	//Update the capacity 
	usedSpace -= *(pTarget - 1) - 2 * HEADER;

	uint* pStart = pTarget - 1;

	if (!isAllocated(pTarget - 2) && pTarget - 1 != block)
	{
		//Put the size of the new block at the start of it
		*(pTarget - *(pTarget - 2) / sizeof(uint)-1) += *(pTarget - 1);

		//Put the size of the new block at the end of it
		*(pTarget - 1 + thisSize - 1) = *(pTarget - *(pTarget - 2) / sizeof(uint)-1);

		//Get a pointer to the new start
		pStart = pTarget - *(pTarget - 2) / sizeof(uint)-1;

		//Update the capacity 
		usedSpace -= 2 * HEADER;
	}

	if (!isAllocated(pTarget - 1 + thisSize)
		&& pTarget - 1 + thisSize-1 != block + size - 1)
	{
		//Get the size of the next empty block
		uint newTail = *(pTarget - 1 + thisSize) / sizeof(uint);

		//Put the size of the new block at the end of it
		*(pTarget - 1 + thisSize + newTail - 1) += *pStart;

		//Put the size of the new block at the start of it
		*pStart = *(pTarget - 1 + thisSize + newTail - 1);

		//Update the capacity 
		usedSpace -= 2 * HEADER;
	}

	//Check if the allocator is empty
	if (usedSpace == 2 * HEADER)
		empty = true;

	if (full)
		full = false;

	bool emptyChecker = true;
	if (empty && block[BlockSize / sizeof(uint)-1] != size - (BlockSize / sizeof(uint)-1))
		emptyChecker = false;

	assert(emptyChecker);
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


inline bool Allocator::isFromMe(void* p)
{
	if (p >= block && p <= block + size / sizeof(uint)-1)
		return true;
	return false;
}


inline bool Allocator::isFull()
{
	if (full)
		return true;
	return false;
}
