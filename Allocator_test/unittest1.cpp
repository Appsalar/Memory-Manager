#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Allocator/Allocator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Allocator_TEST
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(Allocate_memory)
		{
			Allocator a;

			double* foo = (double*)a.MyMalloc(5 * sizeof(double));
			foo[0] = 1;

			Assert::AreEqual(foo[0], 1.0);
		}

		TEST_METHOD(Allocate_and_delete)
		{
			Allocator a;

			double* foo = (double*)a.MyMalloc(5 * sizeof(double));
			foo[0] = 1;

			a.MyFree(foo);
			Assert::IsTrue(a.isEmpty());
		}
	};
}