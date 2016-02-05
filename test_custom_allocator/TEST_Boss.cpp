#include "stdafx.h"
#include "CppUnitTest.h"

#include "../CustomAllocator/BossAllocator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TEST_Custom_Allocator
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Test_allocation)
		{
			int* foo = (int*)MyMalloc(5 * sizeof(int));

			Assert::IsNotNull(foo);
		}

		TEST_METHOD(Test_allocate_and_del)
		{
			int* foo = (int*)MyMalloc(5 * sizeof(int));
			double* bar = (double*)MyMalloc(500 * sizeof(double));

			Assert::IsNotNull(foo);
			Assert::IsNotNull(bar);

			MyFree(foo);
			MyFree(bar);
		}

		TEST_METHOD(Test_allocate_many_times)
		{
			int** foo = new int*[1500];

			for (size_t i = 0; i < 1500; ++i)
				foo[i] = (int*)MyMalloc(sizeof(int));

			for (size_t i = 0; i < 1500; ++i)
				Assert::IsNotNull(foo[i]);
		}
	};
}