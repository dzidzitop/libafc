#include "CompileTimeMathTest.h"
#include <afc/compile_time_math.h>
#include <climits>
#include <cmath>

afc::CompileTimeMathTest::CompileTimeMathTest()
{
	TEST_ADD(CompileTimeMathTest::testBitCount);
	TEST_ADD(CompileTimeMathTest::testOnesCount);
	TEST_ADD(CompileTimeMathTest::testLeadZeroCount);
	TEST_ADD(CompileTimeMathTest::testLog2);
	TEST_ADD(CompileTimeMathTest::testIntMin);
	TEST_ADD(CompileTimeMathTest::testIntMax);
}

void afc::CompileTimeMathTest::testBitCount()
{
	TEST_ASSERT(BitCount<0>::result == 1);
	TEST_ASSERT(BitCount<1>::result == 1);
	TEST_ASSERT(BitCount<0xff>::result == 8);
	TEST_ASSERT(BitCount<0x7f>::result == 7);
	TEST_ASSERT(BitCount<0x20>::result == 6);
	TEST_ASSERT(BitCount<0x1234>::result == 13);
}

void afc::CompileTimeMathTest::testOnesCount()
{
	TEST_ASSERT(OnesCount<0>::result == 0);
	TEST_ASSERT(OnesCount<1>::result == 1);
	TEST_ASSERT(OnesCount<2>::result == 1);
	TEST_ASSERT(OnesCount<3>::result == 2);
	TEST_ASSERT(OnesCount<4>::result == 1);
	TEST_ASSERT(OnesCount<5>::result == 2);
	TEST_ASSERT(OnesCount<6>::result == 2);
	TEST_ASSERT(OnesCount<7>::result == 3);
	TEST_ASSERT(OnesCount<0xFF>::result == 8);
	TEST_ASSERT(OnesCount<0x100>::result == 1);
}

void afc::CompileTimeMathTest::testLeadZeroCount()
{
	if (UINT_MAX == 0xffffffff) {
		TEST_ASSERT(LeadZeroCount<0>::result == 32);
		TEST_ASSERT(LeadZeroCount<1>::result == 31);
	}
	TEST_ASSERT(LeadZeroCount<0>::result == static_cast<unsigned>(BitCount<UINT_MAX>::result));
	TEST_ASSERT(LeadZeroCount<UINT_MAX>::result == 0);
	TEST_ASSERT(LeadZeroCount<0xf0>::result == static_cast<unsigned>(BitCount<UINT_MAX>::result - 8));
}

void afc::CompileTimeMathTest::testLog2()
{
	TEST_ASSERT(Log2<1>::floor == 0);
	TEST_ASSERT(Log2<1>::ceil == 0);
	TEST_ASSERT(Log2<2>::floor == 1);
	TEST_ASSERT(Log2<2>::ceil == 1);
	TEST_ASSERT(Log2<4>::floor == 2);
	TEST_ASSERT(Log2<4>::ceil == 2);
	TEST_ASSERT(Log2<16>::floor == 4);
	TEST_ASSERT(Log2<16>::ceil == 4);
	TEST_ASSERT(Log2<15>::floor == 3);
	TEST_ASSERT(Log2<15>::ceil == 4);
	TEST_ASSERT(Log2<17>::floor == 4);
	TEST_ASSERT(Log2<17>::ceil == 5);
}

void afc::CompileTimeMathTest::testIntMin()
{
	TEST_ASSERT((IntMin<0, 0>::result == 0));
	TEST_ASSERT((IntMin<0, 1>::result == 0));
	TEST_ASSERT((IntMin<1, 0>::result == 0));
	TEST_ASSERT((IntMin<10, -10>::result == -10));
	TEST_ASSERT((IntMin<-4, 12>::result == -4));
	TEST_ASSERT((IntMin<100, 201>::result == 100));
}

void afc::CompileTimeMathTest::testIntMax()
{
	TEST_ASSERT((IntMax<0, 0>::result == 0));
	TEST_ASSERT((IntMax<0, 1>::result == 1));
	TEST_ASSERT((IntMax<1, 0>::result == 1));
	TEST_ASSERT((IntMax<10, -10>::result == 10));
	TEST_ASSERT((IntMax<-4, 12>::result == 12));
	TEST_ASSERT((IntMax<100, 201>::result == 201));
}
