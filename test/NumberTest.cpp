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
#include "NumberTest.hpp"
#include <limits>
#include <string>
#include <afc/number.h>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::NumberTest);

void afc::NumberTest::testPrintNumber_Int()
{
	{
		string result;
		printNumber<10, int>(123, result);
		CPPUNIT_ASSERT_EQUAL(string("123"), result);
		printNumber<10, int>(-456, result);
		CPPUNIT_ASSERT_EQUAL(string("123-456"), result);
	}
	{
		string result;
		printNumber<10, int>(123, result);
		CPPUNIT_ASSERT_EQUAL(string("123"), result);
		printNumber<10, int>(-456, result);
		CPPUNIT_ASSERT_EQUAL(string("123-456"), result);
		printNumber<10, int>(0, result);
		CPPUNIT_ASSERT_EQUAL(string("123-4560"), result);
	}
	{
		string result("aaa");
		printNumber<10, int>(411787, result);
		CPPUNIT_ASSERT_EQUAL(string("aaa411787"), result);
		printNumber<27, int>(411787, result);
		CPPUNIT_ASSERT_EQUAL(string("aaa411787kona"), result);
		printNumber<36, int>(45896, result);
		CPPUNIT_ASSERT_EQUAL(string("aaa411787konazew"), result);
		printNumber<35, int>(45896, result);
		CPPUNIT_ASSERT_EQUAL(string("aaa411787konazew12gb"), result);
	}
	{
		string result;
		printNumber<2, int>(static_cast<int>(-0x80000000), result);
		CPPUNIT_ASSERT_EQUAL(string("-10000000000000000000000000000000"), result);
	}
	{
		string result;
		printNumber<10>(-20, result);
		CPPUNIT_ASSERT_EQUAL(string("-20"), result);
	}
}

void afc::NumberTest::testPrintNumber_MinSignedChar()
{
	{
		string result;
		printNumber<10, signed char>(std::numeric_limits<signed char>::min(), result);
		CPPUNIT_ASSERT_EQUAL(std::to_string(std::numeric_limits<signed char>::min()), result);
	}
}

void afc::NumberTest::testPrintNumber_MinSignedLongLong()
{
	{
		string result;
		printNumber<10, signed long long>(std::numeric_limits<signed long long>::min(), result);
		CPPUNIT_ASSERT_EQUAL(std::to_string(std::numeric_limits<signed long long>::min()), result);
	}
}
