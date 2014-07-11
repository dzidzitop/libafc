/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2014 Dźmitry Laŭčuk

libafc is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#include "MathUtilsTest.hpp"
#include <afc/math_utils.h>
#include <limits>

using std::numeric_limits;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::MathUtilsTest);

void afc::MathUtilsTest::testMin()
{
	CPPUNIT_ASSERT_EQUAL(1u, afc::math::min<const unsigned>(1u, 1u));
	CPPUNIT_ASSERT_EQUAL(1u, afc::math::min<const unsigned>(1u, 2u));
	CPPUNIT_ASSERT_EQUAL(1u, afc::math::min<const unsigned>(1u, 3u));
	CPPUNIT_ASSERT_EQUAL(1u, afc::math::min<const unsigned>(2u, 1u));
	CPPUNIT_ASSERT_EQUAL(1u, afc::math::min<const unsigned>(3u, 1u));

	CPPUNIT_ASSERT_EQUAL(0u, afc::math::min<const unsigned>(std::numeric_limits<unsigned>::max(), 0u));
	CPPUNIT_ASSERT_EQUAL(1u, afc::math::min<const unsigned>(std::numeric_limits<unsigned>::max(), 1u));
	CPPUNIT_ASSERT_EQUAL(0u, afc::math::min<const unsigned>(0u, std::numeric_limits<unsigned>::max()));
	CPPUNIT_ASSERT_EQUAL(1u, afc::math::min<const unsigned>(1u, std::numeric_limits<unsigned>::max()));

	// Checking that the reference of the correct type is returned.
	int x = 5, y = 4;
	int &z = afc::math::min(x, y);
	CPPUNIT_ASSERT_EQUAL(4, z);

	const int a = 2, b = 3;
	const int &c = afc::math::min(a, b);
	CPPUNIT_ASSERT_EQUAL(2, c);
}

void afc::MathUtilsTest::testMax()
{
	CPPUNIT_ASSERT_EQUAL(1u, afc::math::max<const unsigned>(1u, 1u));
	CPPUNIT_ASSERT_EQUAL(2u, afc::math::max<const unsigned>(1u, 2u));
	CPPUNIT_ASSERT_EQUAL(3u, afc::math::max<const unsigned>(1u, 3u));
	CPPUNIT_ASSERT_EQUAL(2u, afc::math::max<const unsigned>(2u, 1u));
	CPPUNIT_ASSERT_EQUAL(3u, afc::math::max<const unsigned>(3u, 1u));

	CPPUNIT_ASSERT_EQUAL(std::numeric_limits<unsigned>::max(),
			afc::math::max<const unsigned>(std::numeric_limits<unsigned>::max(), 0u));
	CPPUNIT_ASSERT_EQUAL(std::numeric_limits<unsigned>::max(),
			afc::math::max<const unsigned>(std::numeric_limits<unsigned>::max(), 1u));
	CPPUNIT_ASSERT_EQUAL(std::numeric_limits<unsigned>::max(),
			afc::math::max<const unsigned>(0u, std::numeric_limits<unsigned>::max()));
	CPPUNIT_ASSERT_EQUAL(std::numeric_limits<unsigned>::max(),
			afc::math::max<const unsigned>(1u, std::numeric_limits<unsigned>::max()));

	// Checking that the reference of the correct type is returned.
	int x = 5, y = 4;
	int &z = afc::math::max(x, y);
	CPPUNIT_ASSERT_EQUAL(5, z);

	const int a = 2, b = 3;
	const int &c = afc::math::max(a, b);
	CPPUNIT_ASSERT_EQUAL(3, c);
}

void afc::MathUtilsTest::testSameSign()
{
	CPPUNIT_ASSERT(sameSign(0, 0));
	CPPUNIT_ASSERT(!sameSign(0, 1));
	CPPUNIT_ASSERT(!sameSign(1, 0));
	CPPUNIT_ASSERT(!sameSign(0, -1));
	CPPUNIT_ASSERT(!sameSign(1, -1));
	CPPUNIT_ASSERT(!sameSign(-1, 1));
	CPPUNIT_ASSERT(!sameSign(1, -1));
	CPPUNIT_ASSERT(sameSign(1, 2));
	CPPUNIT_ASSERT(sameSign(-23432, -1));
}

void afc::MathUtilsTest::testIsEven()
{
	CPPUNIT_ASSERT(!isEven(-1));
	CPPUNIT_ASSERT(isEven(0));
	CPPUNIT_ASSERT(!isEven(1));
	CPPUNIT_ASSERT(isEven(2));
	CPPUNIT_ASSERT(isEven(0xE));
	CPPUNIT_ASSERT(!isEven(0xF));
	CPPUNIT_ASSERT(isEven(100));
}

void afc::MathUtilsTest::testIsOdd()
{
	CPPUNIT_ASSERT(isOdd(-1));
	CPPUNIT_ASSERT(!isOdd(0));
	CPPUNIT_ASSERT(isOdd(1));
	CPPUNIT_ASSERT(!isOdd(2));
	CPPUNIT_ASSERT(!isOdd(0xE));
	CPPUNIT_ASSERT(isOdd(0xF));
	CPPUNIT_ASSERT(!isOdd(100));
}

void afc::MathUtilsTest::testMean_Signed()
{
	CPPUNIT_ASSERT_EQUAL(4, mean(3, 5));
	CPPUNIT_ASSERT_EQUAL(1, mean(0, 3));
	CPPUNIT_ASSERT_EQUAL(0, mean(0, 0));
	CPPUNIT_ASSERT_EQUAL(-4, mean(-3, -5));
	CPPUNIT_ASSERT_EQUAL(1, mean(-3, 5));
	CPPUNIT_ASSERT_EQUAL(~0-2, mean(~0, ~0-4));
}

void afc::MathUtilsTest::testMean_Unsigned()
{
	CPPUNIT_ASSERT_EQUAL(4u, mean(3u, 5u));
	CPPUNIT_ASSERT_EQUAL(1u, mean(0u, 3u));
	CPPUNIT_ASSERT_EQUAL(0u, mean(0u, 0u));
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned>(~0)-2, mean(~0u, ~0u-4u));
}

void afc::MathUtilsTest::testMean_SignedLong()
{
	CPPUNIT_ASSERT_EQUAL(4l, mean(3l, 5l));
	CPPUNIT_ASSERT_EQUAL(1l, mean(0l, 3l));
	CPPUNIT_ASSERT_EQUAL(0l, mean(0l, 0l));
	CPPUNIT_ASSERT_EQUAL(-4l, mean(-3l, -5l));
	CPPUNIT_ASSERT_EQUAL(1l, mean(-3l, 5l));
	CPPUNIT_ASSERT_EQUAL(~0-2l, mean(~0l, ~0l-4l));
}

void afc::MathUtilsTest::testMean_UnsignedLong()
{
	CPPUNIT_ASSERT_EQUAL(4ul, mean(3ul, 5ul));
	CPPUNIT_ASSERT_EQUAL(1ul, mean(0ul, 3ul));
	CPPUNIT_ASSERT_EQUAL(0ul, mean(0ul, 0ul));
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(~0)-2, mean(~0ul, ~0ul-4ul));
}

void afc::MathUtilsTest::testIsPow2_SignedInt()
{
	CPPUNIT_ASSERT(!isPow2(int(0)));
	CPPUNIT_ASSERT(isPow2(int(1)));
	CPPUNIT_ASSERT(isPow2(int(2)));
	CPPUNIT_ASSERT(!isPow2(int(3)));
	CPPUNIT_ASSERT(isPow2(int(4)));
	CPPUNIT_ASSERT(!isPow2(int(5)));
	CPPUNIT_ASSERT(!isPow2(int(6)));
	CPPUNIT_ASSERT(!isPow2(int(7)));
	CPPUNIT_ASSERT(isPow2(int(8)));
	CPPUNIT_ASSERT(!isPow2(int(9)));
	CPPUNIT_ASSERT(!isPow2(int(10)));

	CPPUNIT_ASSERT(!isPow2(int(-1)));
	CPPUNIT_ASSERT(!isPow2(int(-2)));
	CPPUNIT_ASSERT(!isPow2(int(-3)));
	CPPUNIT_ASSERT(!isPow2(int(-4)));
	CPPUNIT_ASSERT(!isPow2(int(-5)));
	CPPUNIT_ASSERT(!isPow2(int(-6)));
	CPPUNIT_ASSERT(!isPow2(int(-7)));
	CPPUNIT_ASSERT(!isPow2(int(-8)));
	CPPUNIT_ASSERT(!isPow2(int(-9)));
	CPPUNIT_ASSERT(!isPow2(int(-10)));

	CPPUNIT_ASSERT(!isPow2(numeric_limits<int>::max()));
	CPPUNIT_ASSERT(!isPow2(numeric_limits<int>::max() / 2));
	CPPUNIT_ASSERT(isPow2(numeric_limits<int>::max() / 2 + 1));
	CPPUNIT_ASSERT(!isPow2(numeric_limits<int>::min()));
	CPPUNIT_ASSERT(!isPow2(numeric_limits<int>::min() / 2));
	CPPUNIT_ASSERT(isPow2(-(numeric_limits<int>::min() / 4)));
}

void afc::MathUtilsTest::testIsPow2_UnsignedInt()
{
	CPPUNIT_ASSERT(!isPow2(unsigned(0)));
	CPPUNIT_ASSERT(isPow2(unsigned(1)));
	CPPUNIT_ASSERT(isPow2(unsigned(2)));
	CPPUNIT_ASSERT(!isPow2(unsigned(3)));
	CPPUNIT_ASSERT(isPow2(unsigned(4)));
	CPPUNIT_ASSERT(!isPow2(unsigned(5)));
	CPPUNIT_ASSERT(!isPow2(unsigned(6)));
	CPPUNIT_ASSERT(!isPow2(unsigned(7)));
	CPPUNIT_ASSERT(isPow2(unsigned(8)));
	CPPUNIT_ASSERT(!isPow2(unsigned(9)));
	CPPUNIT_ASSERT(!isPow2(unsigned(10)));

	CPPUNIT_ASSERT(!isPow2(numeric_limits<unsigned>::max()));
	CPPUNIT_ASSERT(!isPow2(numeric_limits<unsigned>::max() / 2));
	CPPUNIT_ASSERT(isPow2(numeric_limits<unsigned>::max() / 2 + 1));
}
