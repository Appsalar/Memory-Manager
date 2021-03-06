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
			Allocator a(1000);

			double* foo = (double*)a.MyMalloc(5 * sizeof(double));
			for (size_t i = 0; i < 5; ++i)
				foo[i] = i;

			int* bar = (int*)a.MyMalloc(10 * sizeof(int));
			for (size_t i = 0; i < 6; ++i)
				bar[i] = i * 10;

			for (size_t i = 0; i < 5; ++i)
				Assert::AreEqual(foo[i], (double)i);

			for (size_t i = 0; i < 6; ++i)
				Assert::AreEqual(bar[i], (int)i * 10);
		}

		TEST_METHOD(Allocate_and_delete)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(5 * sizeof(double));
			foo[0] = 1;

			a.MyFree((char*)foo);

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
			Allocator a(1000);

			double* foo = (double*)a.MyMalloc(30 * sizeof(double));
			int* bar = (int*)a.MyMalloc(120 * sizeof(int));
			short* aliBaba = (short*)a.MyMalloc(3 * sizeof(short));

			Assert::IsNotNull(foo);
			Assert::IsNotNull(bar);
			Assert::IsNotNull(aliBaba);
		}

		TEST_METHOD(Allocate_max_memory)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(95 * sizeof(double));
			double* bar = (double*)a.MyMalloc(sizeof(double));

			Assert::IsNotNull(foo);
			Assert::IsNotNull(bar);

			Assert::IsTrue(a.isFull());
		}

		TEST_METHOD(Allocate_and_del_multiple_times)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(87 * sizeof(double));
			int* bar = (int*)a.MyMalloc(6 * sizeof(int));
			short* aliBaba = (short*)a.MyMalloc(3 * sizeof(short));
			char* Alladin = (char*)a.MyMalloc(8 * sizeof(char));

			a.MyFree((char*)foo);
			a.MyFree((char*)aliBaba);
			a.MyFree((char*)Alladin);
			a.MyFree((char*)bar);

			Assert::IsTrue(a.isEmpty());
		}

		TEST_METHOD(Allocate_del_allocate)
		{
			Allocator a(100);
			
			double* foo = (double*)a.MyMalloc(87 * sizeof(double));
			a.MyFree((char*)foo);
			
			int* bar = (int*)a.MyMalloc(87 * sizeof(int));
			
			Assert::IsNotNull(bar);
		}

		TEST_METHOD(Allocate_small_pieces)
		{
			Allocator a(100);

			double** foo = new double*[32];

			for (size_t i = 0; i < 32;++i)
				foo[i] = (double*)a.MyMalloc(sizeof(double));

			for (size_t i = 0; i < 32; ++i)
				a.MyFree((char*)foo[i]);
		}

		TEST_METHOD(Is_from_allocator)
		{
			Allocator a(100);

			double* foo = (double*)a.MyMalloc(sizeof(double));
			Assert::IsNotNull(foo);
			Assert::IsTrue(a.isFromMe(foo));
			a.MyFree((char*)foo);

			int* bar = new int;
			Assert::IsFalse(a.isFromMe(bar));
		}
	};
}