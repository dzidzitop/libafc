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
#include "CompileTimeMathTest.h"
#include <afc/compile_time_math.h>
#include <climits>
#include <limits>

CPPUNIT_TEST_SUITE_REGISTRATION(afc::CompileTimeMathTest);

void afc::CompileTimeMathTest::testBitCount()
{
	CPPUNIT_ASSERT_EQUAL(1u, bitCount(0));
	CPPUNIT_ASSERT_EQUAL(1u, bitCount(1));
	CPPUNIT_ASSERT_EQUAL(8u, bitCount(0xff));
	CPPUNIT_ASSERT_EQUAL(7u, bitCount(0x7f));
	CPPUNIT_ASSERT_EQUAL(6u, bitCount(0x20));
	CPPUNIT_ASSERT_EQUAL(13u, bitCount(0x1234));
}

void afc::CompileTimeMathTest::testOnesCount()
{
	CPPUNIT_ASSERT_EQUAL(0u, onesCount(0));
	CPPUNIT_ASSERT_EQUAL(1u, onesCount(1));
	CPPUNIT_ASSERT_EQUAL(1u, onesCount(2));
	CPPUNIT_ASSERT_EQUAL(2u, onesCount(3));
	CPPUNIT_ASSERT_EQUAL(1u, onesCount(4));
	CPPUNIT_ASSERT_EQUAL(2u, onesCount(5));
	CPPUNIT_ASSERT_EQUAL(2u, onesCount(6));
	CPPUNIT_ASSERT_EQUAL(3u, onesCount(7));
	CPPUNIT_ASSERT_EQUAL(8u, onesCount(0xff));
	CPPUNIT_ASSERT_EQUAL(1u, onesCount(0x100));
}

void afc::CompileTimeMathTest::testLeadZeroCount()
{
	CPPUNIT_ASSERT_EQUAL(unsigned(std::numeric_limits<unsigned>::digits), leadZeroCount(0u));
	CPPUNIT_ASSERT_EQUAL(unsigned(std::numeric_limits<unsigned>::digits - 1), leadZeroCount(1u));
	CPPUNIT_ASSERT_EQUAL(0u, leadZeroCount(UINT_MAX));
	CPPUNIT_ASSERT_EQUAL(unsigned(std::numeric_limits<unsigned>::digits - 8), leadZeroCount(0xf0u));
}

void afc::CompileTimeMathTest::testLog2()
{
	CPPUNIT_ASSERT_EQUAL(0u, log2Floor(1u));
	CPPUNIT_ASSERT_EQUAL(0u, log2Ceil(1u));
	CPPUNIT_ASSERT_EQUAL(1u, log2Floor(2u));
	CPPUNIT_ASSERT_EQUAL(1u, log2Ceil(2u));
	CPPUNIT_ASSERT_EQUAL(2u, log2Floor(4u));
	CPPUNIT_ASSERT_EQUAL(2u, log2Ceil(4u));
	CPPUNIT_ASSERT_EQUAL(4u, log2Floor(16u));
	CPPUNIT_ASSERT_EQUAL(4u, log2Ceil(16u));
	CPPUNIT_ASSERT_EQUAL(3u, log2Floor(15u));
	CPPUNIT_ASSERT_EQUAL(4u, log2Ceil(15u));
	CPPUNIT_ASSERT_EQUAL(4u, log2Floor(17u));
	CPPUNIT_ASSERT_EQUAL(5u, log2Ceil(17u));
}
