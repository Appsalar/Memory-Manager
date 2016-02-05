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
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(5 * sizeof(double));
			Assert::IsNotNull(foo);
		}

		TEST_METHOD(Allocate_and_modify)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(5 * sizeof(double));
			foo[0] = 1;

			Assert::AreEqual(foo[0], 1.0);
		}

		TEST_METHOD(Allocate_and_use)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(5 * sizeof(double));
			for (size_t i = 0; i < 5; ++i)
				foo[i] = i;

			int* bar = (int*)a.MyMalloc(6 * sizeof(int));
			for (size_t i = 0; i < 6; ++i)
				bar[i] = i * 10;

			for (size_t i = 0; i < 5; ++i)
				Assert::AreEqual(foo[i], (double)i);
		}

		TEST_METHOD(Allocate_and_delete)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(5 * sizeof(double));
			foo[0] = 1;

			a.MyFree(foo);

			Assert::IsTrue(a.isEmpty());
		}

		TEST_METHOD(Allocate_too_much)
		{
			Allocator a(100);

			int* foo = (int*)a.MyMalloc(120 * sizeof(double));
			Assert::IsNull(foo);
		}

		TEST_METHOD(Allocate_multiple_times)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(30 * sizeof(double));
			int* bar = (int*)a.MyMalloc(6 * sizeof(int));
			short* aliBaba = (short*)a.MyMalloc(3 * sizeof(short));

			Assert::IsNotNull(foo);
			Assert::IsNotNull(bar);
			Assert::IsNotNull(aliBaba);
		}

		TEST_METHOD(Allocate_max_memory)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(98 * sizeof(double));

			Assert::IsNotNull(foo);
		}

		TEST_METHOD(Allocate_and_del_multiple_times)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(80 * sizeof(double));
			int* bar = (int*)a.MyMalloc(6 * sizeof(int));
			short* aliBaba = (short*)a.MyMalloc(3 * sizeof(short));
			char* Alladin = a.MyMalloc(8 * sizeof(char));

			a.MyFree(foo);
			a.MyFree(aliBaba);
			a.MyFree(Alladin);
			a.MyFree(bar);

			Assert::IsTrue(a.isEmpty());
		}

		TEST_METHOD(Allocate__del_allocate)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(85 * sizeof(double));
			a.MyFree(foo);

			int* bar = (int*)a.MyMalloc(85 * sizeof(int));

			Assert::IsNotNull(bar);
		}
	};
}