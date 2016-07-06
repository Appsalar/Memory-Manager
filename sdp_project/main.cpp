#include <iostream>
#include "TempAllocator.h"
#include <vector>
#include <list>
#include <map>

std::pair<int, int> edge;

int main()
{
	TempAllocator<int> a;

	int * p = a.allocate(4);

	*p = 4;

	std::cout << *p << std::endl;


	
	std::list<int, TempAllocator<int>> vec;

	for (size_t i = 0; i < 100; i++)
		vec.push_back(i);

	//for (size_t i = 0; i < 100; i++)
		//std::cout << vec[i] << std::endl;


	std::map<int, int, std::less<int>,
		TempAllocator<
		std::pair<int, int>
		>
	> foo;

	for (size_t i = 0; i < 10000; i++)
		foo.insert(std::pair<int, int>(i, i + 1));

	return 0;
}