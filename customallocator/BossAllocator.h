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
	~BossAllocator();
	void* MyMalloc(const int);
	void MyFree(char*);
};


//if invalid argument or no space
//return NULL 
BOSS_API void* MyMalloc(const int memSize);

//if invalid argument 
//undefined behavior
BOSS_API void MyFree(char* p);