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
#include "MathUtilsTest.h"
#include <afc/math_utils.h>

CPPUNIT_TEST_SUITE_REGISTRATION(afc::MathUtilsTest);

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
	CPPUNIT_ASSERT_EQUAL(4l, mean(0l, 3l));
	CPPUNIT_ASSERT_EQUAL(4l, mean(0l, 0l));
	CPPUNIT_ASSERT_EQUAL(4l, mean(-3l, -5l));
	CPPUNIT_ASSERT_EQUAL(4l, mean(-3l, 5l));
	CPPUNIT_ASSERT_EQUAL(4l, mean(~0l, ~0l-4l));
}

void afc::MathUtilsTest::testMean_UnsignedLong()
{
	CPPUNIT_ASSERT_EQUAL(4ul, mean(3ul, 5ul));
	CPPUNIT_ASSERT_EQUAL(1ul, mean(0ul, 3ul));
	CPPUNIT_ASSERT_EQUAL(0ul, mean(0ul, 0ul));
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned long>(~0)-2, mean(~0ul, ~0ul-4ul));
}
