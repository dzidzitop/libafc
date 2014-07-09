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
#include <afc/number.h>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::NumberTest);

void afc::NumberTest::testAppendToStringInt()
{
	{
		string result;
		appendToString<int>(123, 10, result);
		CPPUNIT_ASSERT_EQUAL(string("123"), result);
		appendToString<int>(-456, 10, result);
		CPPUNIT_ASSERT_EQUAL(string("123-456"), result);
	}
	{
		string result;
		appendToString<int>(123, result);
		CPPUNIT_ASSERT_EQUAL(string("123"), result);
		appendToString<int>(-456, result);
		CPPUNIT_ASSERT_EQUAL(string("123-456"), result);
		appendToString<int>(0, result);
		CPPUNIT_ASSERT_EQUAL(string("123-4560"), result);
	}
	{
		string result("aaa");
		appendToString<int>(411787, 10, result);
		CPPUNIT_ASSERT_EQUAL(string("aaa411787"), result);
		appendToString<int>(411787, 27, result);
		CPPUNIT_ASSERT_EQUAL(string("aaa411787kona"), result);
		appendToString<int>(45896, 36, result);
		CPPUNIT_ASSERT_EQUAL(string("aaa411787konazew"), result);
		appendToString<int>(45896, 35, result);
		CPPUNIT_ASSERT_EQUAL(string("aaa411787konazew12gb"), result);
	}
	{
		string result("aaa");
		CPPUNIT_ASSERT_THROW(appendToString<int>(234, 1, result), InvalidArgumentException);
		CPPUNIT_ASSERT_EQUAL(string("aaa"), result);
		CPPUNIT_ASSERT_THROW(appendToString<int>(234, 0, result), InvalidArgumentException);
		CPPUNIT_ASSERT_EQUAL(string("aaa"), result);
		CPPUNIT_ASSERT_THROW(appendToString<int>(234, 37, result), InvalidArgumentException);
		CPPUNIT_ASSERT_EQUAL(string("aaa"), result);
		CPPUNIT_ASSERT_THROW(appendToString<int>(234, 255, result), InvalidArgumentException);
		CPPUNIT_ASSERT_EQUAL(string("aaa"), result);
	}
	{
		string result;
		appendToString<int>(static_cast<int>(-0x80000000), 2, result);
		CPPUNIT_ASSERT_EQUAL(string("-10000000000000000000000000000000"), result);
	}
	{
		string result;
		appendToString(-20, result);
		CPPUNIT_ASSERT_EQUAL(string("-20"), result);
	}
}
