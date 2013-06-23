#include "MathUtilsTest.h"
#include <afc/math_utils.h>

afc::MathUtilsTest::MathUtilsTest()
{
	TEST_ADD(MathUtilsTest::testSameSign);
	TEST_ADD(MathUtilsTest::testIsEven);
	TEST_ADD(MathUtilsTest::testIsOdd);
	TEST_ADD(MathUtilsTest::testMean);
}

void afc::MathUtilsTest::testSameSign()
{
	TEST_ASSERT(sameSign(0, 0));
	TEST_ASSERT(!sameSign(0, 1));
	TEST_ASSERT(!sameSign(1, 0));
	TEST_ASSERT(!sameSign(0, -1));
	TEST_ASSERT(!sameSign(1, -1));
	TEST_ASSERT(!sameSign(-1, 1));
	TEST_ASSERT(!sameSign(1, -1));
	TEST_ASSERT(sameSign(1, 2));
	TEST_ASSERT(sameSign(-23432, -1));
}

void afc::MathUtilsTest::testIsEven()
{
	TEST_ASSERT(!isEven(-1));
	TEST_ASSERT(isEven(0));
	TEST_ASSERT(!isEven(1));
	TEST_ASSERT(isEven(2));
	TEST_ASSERT(isEven(0xE));
	TEST_ASSERT(!isEven(0xF));
	TEST_ASSERT(isEven(100));
}

void afc::MathUtilsTest::testIsOdd()
{
	TEST_ASSERT(isOdd(-1));
	TEST_ASSERT(!isOdd(0));
	TEST_ASSERT(isOdd(1));
	TEST_ASSERT(!isOdd(2));
	TEST_ASSERT(!isOdd(0xE));
	TEST_ASSERT(isOdd(0xF));
	TEST_ASSERT(!isOdd(100));
}

void afc::MathUtilsTest::testMean()
{
	TEST_ASSERT(mean(3, 5) == 4);
	TEST_ASSERT(mean(0, 3) == 1);
	TEST_ASSERT(mean(0, 0) == 0);
	TEST_ASSERT(mean(~0, ~0-4) == static_cast<unsigned>(~0)-2);
}
