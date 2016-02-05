#include "Allocator.h"
#include <assert.h>

#define HEADER sizeof(uint)

#define LAST this->size / sizeof(uint)

//Creating allocator with capacity of
//size * sizeof(double) bites
Allocator::Allocator(size_t size, size_t BlockSize)
{
	this->size = size * sizeof(uint);
	block = new uint[size];
	empty = true;
	usedSpace = 2 * HEADER;
	this->BlockSize = BlockSize;
	firstFree = BlockSize / sizeof(uint) - 1;

	block[firstFree] = this->size - firstFree;
	block[size - 1] = block[firstFree];
	block[firstFree + 1] = 0;
	block[firstFree + 2] = LAST;
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

	size_t indexMem = size / sizeof(uint);

	//Try to find a block to fit the allocation
	size_t i = firstFree;
	while (i < indexMem - 2)
	{
		if (block[i] < totalMemory * sizeof(uint))
		{
			i = block[i + 2];
		}
		else
		{
			if (empty)
				empty = false;

			if (block[i] - totalMemory * sizeof(uint) <= 3 * HEADER)
			{
				usedSpace += block[i] - 2 * HEADER;
				++block[i];
				++block[i + totalMemory - 1];

				uint prev = block[i + 1];
				uint next = block[i + 2];

				//Attach the prev free block to the next, if there is a prev
				//else 
				//Set next as fristFree
				if (prev != 0)
				{
					block[prev + 2] = next;
				}
				else
					firstFree = next;

				//Attach the next free block to the prev
				if (next != LAST)
				{
					block[next + 1] = prev;
				}
			}
			else
			{
				usedSpace += totalMemory * HEADER;
				uint oldMem = block[i];
				block[i] = totalMemory * HEADER + 1;
				block[i + totalMemory - 1] = totalMemory * HEADER + 1;

				block[i + totalMemory] = oldMem - totalMemory * HEADER;
				block[i + oldMem / HEADER - 1] = oldMem - totalMemory * HEADER;

				//Get the previous and next free blocks
				uint prev = block[i + 1];
				uint next = block[i + 2];

				//Set the prev and next free blocks to the new block
				block[i + totalMemory + 1] = prev;
				block[i + totalMemory + 2] = next;

				//Attach the prev free block to this, if there is a prev 
				//else 
				//Set this as fristFree
				if (prev != 0)
				{
					block[prev + 2] = i + totalMemory;
				}
				else
					firstFree = i + totalMemory;

				//Attach the next free block to this
				if (next != LAST)
				{
					block[next + 1] = i + totalMemory;
				}
			}
			return (char*)(&block[i + 1]);
		}
	}

	return NULL;
}


void Allocator::MyFree(void* p)
{
	uint* pTarget = (uint*)p;
	if (!p)
		return;

	size_t thisSize = *(pTarget - 1) / sizeof(uint);

	if (!isAllocated(pTarget - 1) || !isAllocated(pTarget - 1 + thisSize - 1))
		throw "360 no skope";

	*(pTarget - 1) -= 1;
	*(pTarget - 1 + thisSize - 1) -= 1;
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
		usedSpace -= 2 * HEADER;
	}

	if (!isAllocated(pTarget - 1 + thisSize)
		&& pTarget - 1 + thisSize - 1 != block + size - 1)
	{
		//Set the prev and next free blocks to the new block 
		uint prev = *(pTarget - 1 + thisSize + 1);
		uint next = *(pTarget - 1 + thisSize + 2);

		//Get the size of the next empty block
		uint newTail = *(pTarget - 1 + thisSize) / sizeof(uint);

		//Put the size of the new block at the end of it
		*(pTarget - 1 + thisSize + newTail - 1) += *pStart;

		//Put the size of the new block at the start of it
		*pStart = *(pTarget - 1 + thisSize + newTail - 1);
		usedSpace -= 2 * HEADER;

		pStart[2] = next;
		if (pStart == pTarget - 1)
		{
			pStart[1] = prev;
			if (prev != 0)
			{
				block[prev + 2] = pStart[0];
			}
			else
				firstFree = 0;
		}

		//Attach the next free block to this
		if (next != LAST)
		{
			block[next + 1] = pStart[0];
		}
	}

	if (usedSpace == 2 * HEADER)
		empty = true;

	bool emptyChecker = true;
	if (empty && block[BlockSize / sizeof(uint) - 1] != size - (BlockSize / sizeof(uint)- 1))
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
