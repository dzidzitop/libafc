#include "MathUtilsTest.h"
#include <afc/math_utils.h>

afc::MathUtilsTest::MathUtilsTest()
{
	TEST_ADD(MathUtilsTest::testSameSign);
	TEST_ADD(MathUtilsTest::testIsEven);
	TEST_ADD(MathUtilsTest::testIsOdd);
	TEST_ADD(MathUtilsTest::testMean_Signed);
	TEST_ADD(MathUtilsTest::testMean_Unsigned);
	TEST_ADD(MathUtilsTest::testMean_SignedLong);
	TEST_ADD(MathUtilsTest::testMean_UnsignedLong);
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

void afc::MathUtilsTest::testMean_Signed()
{
	TEST_ASSERT(mean(3, 5) == 4);
	TEST_ASSERT(mean(0, 3) == 1);
	TEST_ASSERT(mean(0, 0) == 0);
	TEST_ASSERT(mean(-3, -5) == -4);
	TEST_ASSERT(mean(-3, 5) == 1);
	TEST_ASSERT(mean(~0, ~0-4) == ~0-2);
}

void afc::MathUtilsTest::testMean_Unsigned()
{
	TEST_ASSERT(mean(3u, 5u) == 4);
	TEST_ASSERT(mean(0u, 3u) == 1);
	TEST_ASSERT(mean(0u, 0u) == 0);
	TEST_ASSERT(mean(~0u, ~0u-4u) == static_cast<unsigned>(~0)-2);
}

void afc::MathUtilsTest::testMean_SignedLong()
{
	TEST_ASSERT(mean(3l, 5l) == 4l);
	TEST_ASSERT(mean(0l, 3l) == 1l);
	TEST_ASSERT(mean(0l, 0l) == 0l);
	TEST_ASSERT(mean(-3l, -5l) == -4l);
	TEST_ASSERT(mean(-3l, 5l) == 1l);
	TEST_ASSERT(mean(~0l, ~0l-4l) == ~0l-2l);
}

void afc::MathUtilsTest::testMean_UnsignedLong()
{
	TEST_ASSERT(mean(3ul, 5ul) == 4l);
	TEST_ASSERT(mean(0ul, 3ul) == 1l);
	TEST_ASSERT(mean(0ul, 0ul) == 0l);
	TEST_ASSERT(mean(~0ul, ~0ul-4ul) == static_cast<unsigned long>(~0)-2);
}
