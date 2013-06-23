#include "FastDivisionTest.h"
#include <afc/fast_division.h>
#include <climits>

afc::FastDivisionTest::FastDivisionTest()
{
	TEST_ADD(FastDivisionTest::testDivideBy10);
}

void afc::FastDivisionTest::testDivideBy10()
{
	for (unsigned i = 0; i < 1026u; ++i) {
		TEST_ASSERT(divide<10>(i) == i/10);
	}
	TEST_ASSERT(divide<10>(1231423432) == 123142343);
	TEST_ASSERT(divide<10>(34233232) == 3423323);
	TEST_ASSERT(divide<10>(UINT_MAX) == UINT_MAX/10);
	TEST_ASSERT(divide<10>(UINT_MAX-1) == (UINT_MAX-1)/10);
	// some kind of random numbers
	for (unsigned i = 12312; i < UINT_MAX/10; i += UINT_MAX/12343) {
		TEST_ASSERT(divide<10>(i) == i/10);
	}
}
