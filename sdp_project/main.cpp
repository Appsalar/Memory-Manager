#include <iostream>
#include "TempAllocator.h"
#include <vector>
#include <list>
#include <map>


#define PRINT std::cout <<\
				"-----------------------"<< std::endl


int main()
{
	/*TempAllocator<int> a;

	int * p = a.allocate(4);

	*p = 4;

	std::cout << *p << std::endl;

	int* f = a.allocate(10);

	a.deallocate(p, 4);
	PRINT;*/

	std::vector<int, TempAllocator<int>> vec;

	for (size_t i = 0; i < 1000; i++)
		vec.push_back(i);

	for (size_t i = 0; i < 1000; i++)
		std::cout << vec[i] << std::endl;

	PRINT;

	std::map<int, int, std::less<int>,
		TempAllocator<
		std::pair<int, int>
		>
	> foo;

	for (size_t i = 0; i < 10000; i++)
		foo.insert(std::pair<int, int>(i, i + 1));

	PRINT;

	std::list<int, TempAllocator<int>> bar;

	for (size_t i = 0; i < 100; i++)
		vec.push_back(i);

	for (size_t i = 0; i < 100; i++)
		std::cout << vec[i] << std::endl;

	return 0;
}