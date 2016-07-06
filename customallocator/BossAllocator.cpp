#include "BossAllocator.h"

#define MAXSIZE 2048 * 32

#define DSIZE sizeof(double)

BossAllocator::BossAllocator() : Small(510), Medium(2045), Huge(2045 * 4), Gigantic(2048 * 32)
{
}


BossAllocator::~BossAllocator()
{
}


void* BossAllocator::MyMalloc(const int memSize)
{
	if (memSize<=0 || memSize >= MAXSIZE)
		return NULL;

	if (memSize <= DSIZE && !Small.isFull())
		return Small.MyMalloc(DSIZE);

	if (memSize <= 2 * DSIZE && !Medium.isFull())
		return Medium.MyMalloc(2 * DSIZE);

	if (memSize <= 8 * DSIZE && !Huge.isFull())
		return Huge.MyMalloc(8 * DSIZE);

	return Gigantic.MyMalloc(memSize > 512 ? memSize : 512);
}


void BossAllocator::MyFree(char* p)
{
	//*Target <= 3 * DSIZE && Small.isFromMe(p) ????
	if (Small.isFromMe(p))
			Small.MyFree(p);
	else if (Medium.isFromMe(p))
			Medium.MyFree(p);
	else if (Huge.isFromMe(p))
		Huge.MyFree(p);
	else if (Gigantic.isFromMe(p))
		Gigantic.MyFree(p);
}

BossAllocator GOUEM;

void* MyMalloc(const int memSize)
{
	return GOUEM.MyMalloc(memSize);
}

void MyFree(char* p)
{
	GOUEM.MyFree(p);
}