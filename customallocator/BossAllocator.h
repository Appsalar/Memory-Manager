#pragma once

#include "../Allocator/Allocator.h"

#ifdef CUSTOMALLOCATOR_EXPORTS
#define BOSS_API __declspec(dllexport)
#else
#define BOSS_API __declspec(dllimport)
#endif

class BossAllocator
{
private:
	Allocator Small;
	Allocator Medium;
	Allocator Huge;
	Allocator Gigantic;
public:
	BossAllocator();
	virtual ~BossAllocator();
	void* MyMalloc(const int);
	void MyFree(void*);
};



BOSS_API void* MyMalloc(const int memSize);

BOSS_API void MyFree(void* p);