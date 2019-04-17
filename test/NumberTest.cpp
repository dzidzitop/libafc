/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2019 Dźmitry Laŭčuk

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
#include <afc/StringRef.hpp>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::NumberTest);

void afc::NumberTest::testAppendNumber_Int()
{
	{
		string result;
		appendNumber<10, int>(123, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("123"), result);
		appendNumber<10, int>(-456, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("123-456"), result);
	}
	{
		string result;
		appendNumber<10, int>(123, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("123"), result);
		appendNumber<10, int>(-456, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("123-456"), result);
		appendNumber<10, int>(0, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("123-4560"), result);
	}
	{
		string result("aaa");
		appendNumber<10, int>(411787, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("aaa411787"), result);
		appendNumber<27, int>(411787, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("aaa411787kona"), result);
		appendNumber<36, int>(45896, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("aaa411787konazew"), result);
		appendNumber<35, int>(45896, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("aaa411787konazew12gb"), result);
	}
	{
		string result;
		appendNumber<2, int>(static_cast<int>(-0x80000000),
				[&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("-10000000000000000000000000000000"), result);
	}
	{
		string result;
		appendNumber<10>(-20, [&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(string("-20"), result);
	}
}

void afc::NumberTest::testAppendNumber_MinSignedChar()
{
	{
		string result;
		appendNumber<10, signed char>(std::numeric_limits<signed char>::min(),
				[&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(std::to_string(std::numeric_limits<signed char>::min()), result);
	}
}

void afc::NumberTest::testAppendNumber_MinSignedLongLong()
{
	{
		string result;
		appendNumber<10, signed long long>(std::numeric_limits<signed long long>::min(),
				[&](const char *begin, const char *end) { result.append(begin, end); });
		CPPUNIT_ASSERT_EQUAL(std::to_string(std::numeric_limits<signed long long>::min()), result);
	}
}

void afc::NumberTest::testParseNumber_DecimalInts()
{
	{
		ConstStringRef positiveNumber = "1053"_s;
		int parsedPositiveNumber = 1;

		const char * const iteratorPositiveNumber = parseNumber<10>(positiveNumber.begin(), positiveNumber.end(),
				parsedPositiveNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(1053, parsedPositiveNumber);
		CPPUNIT_ASSERT_EQUAL(positiveNumber.end(), iteratorPositiveNumber);
	}

	{
		ConstStringRef negativeNumber = "-12366"_s;
		int parsedNegativeNumber = 1;

		const char * const iteratorNegativeNumber = parseNumber<10>(negativeNumber.begin(), negativeNumber.end(),
				parsedNegativeNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(-12366, parsedNegativeNumber);
		CPPUNIT_ASSERT_EQUAL(negativeNumber.end(), iteratorNegativeNumber);
	}

	{
		ConstStringRef zero = "0"_s;
		int parsedZero = 1;

		const char * const iteratorZero = parseNumber<10>(zero.begin(), zero.end(),
				parsedZero, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(0, parsedZero);
		CPPUNIT_ASSERT_EQUAL(zero.end(), iteratorZero);
	}

	{
		string maxInt(std::to_string(std::numeric_limits<int>::max()));
		int parsedNumber = 0;

		string::iterator result = parseNumber<10>(maxInt.begin(), maxInt.end(),
				parsedNumber, [](string::iterator) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int>::max(), parsedNumber);
		CPPUNIT_ASSERT(maxInt.end() == result);
	}

	{
		string minInt(std::to_string(std::numeric_limits<int>::min()));
		int parsedNumber = 0;

		string::iterator result = parseNumber<10>(minInt.begin(), minInt.end(),
				parsedNumber, [](string::iterator) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int>::min(), parsedNumber);
		CPPUNIT_ASSERT(minInt.end() == result);
	}

	if (std::numeric_limits<long long>::max() > std::numeric_limits<int>::max()) {
		bool errorReported = false;
		string overflow(std::to_string(static_cast<long long>(std::numeric_limits<int>::max()) + 1));
		int parsedNumber = 0;

		parseNumber<10>(overflow.begin(), overflow.end(), parsedNumber,
				[&](string::iterator) { errorReported = true; });
		CPPUNIT_ASSERT(errorReported);
	}

	if (std::numeric_limits<long long>::min() < std::numeric_limits<int>::min()) {
		bool errorReported = false;
		string underflow(std::to_string(static_cast<long long>(std::numeric_limits<int>::min()) - 1));
		int parsedNumber = 0;

		parseNumber<10>(underflow.begin(), underflow.end(), parsedNumber,
				[&](string::iterator) { errorReported = true; });
		CPPUNIT_ASSERT(errorReported);
	}
}

void afc::NumberTest::testParseNumber_DecimalUnsignedInts()
{
	{
		ConstStringRef positiveNumber = "1053"_s;
		unsigned parsedPositiveNumber = 1;

		const char * const iteratorPositiveNumber = parseNumber<10>(positiveNumber.begin(), positiveNumber.end(),
				parsedPositiveNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(1053u, parsedPositiveNumber);
		CPPUNIT_ASSERT_EQUAL(positiveNumber.end(), iteratorPositiveNumber);
	}

	{
		ConstStringRef zero = "0"_s;
		unsigned parsedZero = 1;

		const char * const iteratorZero = parseNumber<10>(zero.begin(), zero.end(),
				parsedZero, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(0u, parsedZero);
		CPPUNIT_ASSERT_EQUAL(zero.end(), iteratorZero);
	}

	{
		string maxUInt(std::to_string(std::numeric_limits<unsigned>::max()));
		unsigned parsedNumber = 0;

		string::iterator result = parseNumber<10>(maxUInt.begin(), maxUInt.end(),
				parsedNumber, [](string::iterator) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<unsigned>::max(), parsedNumber);
		CPPUNIT_ASSERT(maxUInt.end() == result);
	}

	if (std::numeric_limits<unsigned long long>::max() > std::numeric_limits<unsigned>::max()) {
		bool errorReported = false;
		string overflow(std::to_string(static_cast<unsigned long long>(std::numeric_limits<unsigned>::max()) + 1));
		unsigned parsedNumber = 0;

		parseNumber<10>(overflow.begin(), overflow.end(), parsedNumber,
				[&](string::iterator) { errorReported = true; });
		CPPUNIT_ASSERT(errorReported);
	}
}

void afc::NumberTest::testParseNumber_HexInts()
{
	{
		ConstStringRef positiveNumber = "F3aB"_s;
		int parsedPositiveNumber = 1;

		const char * const iteratorPositiveNumber = parseNumber<16>(positiveNumber.begin(), positiveNumber.end(),
				parsedPositiveNumber, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(0xf3ab, parsedPositiveNumber);
		CPPUNIT_ASSERT_EQUAL(positiveNumber.end(), iteratorPositiveNumber);
	}

	{
		ConstStringRef negativeNumber = "-1f9e"_s;
		int parsedNegativeNumber = 1;

		const char * const iteratorNegativeNumber = parseNumber<16>(negativeNumber.begin(), negativeNumber.end(),
				parsedNegativeNumber, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(-0x1f9e, parsedNegativeNumber);
		CPPUNIT_ASSERT_EQUAL(negativeNumber.end(), iteratorNegativeNumber);
	}

	{
		ConstStringRef zero = "0"_s;
		int parsedZero = 1;

		const char * const iteratorZero = parseNumber<16>(zero.begin(), zero.end(),
				parsedZero, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(0, parsedZero);
		CPPUNIT_ASSERT_EQUAL(zero.end(), iteratorZero);
	}
}

void afc::NumberTest::testParseNumber_HexUnsignedInts()
{
	{
		ConstStringRef positiveNumber = "e05D"_s;
		unsigned parsedPositiveNumber = 1;

		const char * const iteratorPositiveNumber = parseNumber<16>(positiveNumber.begin(), positiveNumber.end(),
				parsedPositiveNumber, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(0xe05du, parsedPositiveNumber);
		CPPUNIT_ASSERT_EQUAL(positiveNumber.end(), iteratorPositiveNumber);
	}

	{
		ConstStringRef zero = "0"_s;
		unsigned parsedZero = 1;

		const char * const iteratorZero = parseNumber<16>(zero.begin(), zero.end(),
				parsedZero, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(0u, parsedZero);
		CPPUNIT_ASSERT_EQUAL(zero.end(), iteratorZero);
	}
}

void afc::NumberTest::testParseNumberCString_DecimalInts()
{
	{
		ConstStringRef positiveNumber = "1053"_s;
		int parsedPositiveNumber = 1;

		const char * const iteratorPositiveNumber = parseNumber<10>(positiveNumber.begin(),
				parsedPositiveNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(1053, parsedPositiveNumber);
		CPPUNIT_ASSERT_EQUAL((unsigned long long) positiveNumber.end(), (unsigned long long) iteratorPositiveNumber);
	}

	{
		ConstStringRef negativeNumber = "-12366"_s;
		int parsedNegativeNumber = 1;

		const char * const iteratorNegativeNumber = parseNumber<10>(negativeNumber.begin(),
				parsedNegativeNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(-12366, parsedNegativeNumber);
		CPPUNIT_ASSERT_EQUAL(negativeNumber.end(), iteratorNegativeNumber);
	}

	{
		ConstStringRef zero = "0"_s;
		int parsedZero = 1;

		const char * const iteratorZero = parseNumber<10>(zero.begin(),
				parsedZero, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(0, parsedZero);
		CPPUNIT_ASSERT_EQUAL(zero.end(), iteratorZero);
	}

	{
		string maxInt(std::to_string(std::numeric_limits<int>::max()));
		int parsedNumber = 0;

		const char * const result = parseNumber<10>(maxInt.c_str(),
				parsedNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int>::max(), parsedNumber);
		CPPUNIT_ASSERT(maxInt.c_str() + maxInt.size() == result);
	}

	{
		string minInt(std::to_string(std::numeric_limits<int>::min()));
		int parsedNumber = 0;

		const char * const result = parseNumber<10>(minInt.c_str(),
				parsedNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<int>::min(), parsedNumber);
		CPPUNIT_ASSERT(minInt.c_str() + minInt.size() == result);
	}

	if (std::numeric_limits<long long>::max() > std::numeric_limits<int>::max()) {
		bool errorReported = false;
		string overflow(std::to_string(static_cast<long long>(std::numeric_limits<int>::max()) + 1));
		int parsedNumber = 0;

		parseNumber<10>(overflow.c_str(), parsedNumber, [&](const char *) { errorReported = true; });
		CPPUNIT_ASSERT(errorReported);
	}

	if (std::numeric_limits<long long>::min() < std::numeric_limits<int>::min()) {
		bool errorReported = false;
		string underflow(std::to_string(static_cast<long long>(std::numeric_limits<int>::min()) - 1));
		int parsedNumber = 0;

		parseNumber<10>(underflow.c_str(), parsedNumber, [&](const char *) { errorReported = true; });
		CPPUNIT_ASSERT(errorReported);
	}
}

void afc::NumberTest::testParseNumberCString_DecimalUnsignedInts()
{
	{
		ConstStringRef positiveNumber = "1053"_s;
		unsigned parsedPositiveNumber = 1;

		const char * const iteratorPositiveNumber = parseNumber<10>(positiveNumber.begin(),
				parsedPositiveNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(1053u, parsedPositiveNumber);
		CPPUNIT_ASSERT_EQUAL(positiveNumber.end(), iteratorPositiveNumber);
	}

	{
		ConstStringRef zero = "0"_s;
		unsigned parsedZero = 1;

		const char * const iteratorZero = parseNumber<10>(zero.begin(),
				parsedZero, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(0u, parsedZero);
		CPPUNIT_ASSERT_EQUAL(zero.end(), iteratorZero);
	}

	{
		string maxUInt(std::to_string(std::numeric_limits<unsigned>::max()));
		unsigned parsedNumber = 0;

		const char * const result = parseNumber<10>(maxUInt.c_str(),
				parsedNumber, [](const char *) { CPPUNIT_FAIL(""); });
		CPPUNIT_ASSERT_EQUAL(std::numeric_limits<unsigned>::max(), parsedNumber);
		CPPUNIT_ASSERT(maxUInt.c_str() + maxUInt.size() == result);
	}

	if (std::numeric_limits<unsigned long long>::max() > std::numeric_limits<unsigned>::max()) {
		bool errorReported = false;
		string overflow(std::to_string(static_cast<unsigned long long>(std::numeric_limits<unsigned>::max()) + 1));
		unsigned parsedNumber = 0;

		parseNumber<10>(overflow.c_str(), parsedNumber, [&](const char *) { errorReported = true; });
		CPPUNIT_ASSERT(errorReported);
	}
}

void afc::NumberTest::testParseNumberCString_HexInts()
{
	{
		ConstStringRef positiveNumber = "F3aB"_s;
		int parsedPositiveNumber = 1;

		const char * const iteratorPositiveNumber = parseNumber<16>(positiveNumber.begin(),
				parsedPositiveNumber, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(0xf3ab, parsedPositiveNumber);
		CPPUNIT_ASSERT_EQUAL(positiveNumber.end(), iteratorPositiveNumber);
	}

	{
		ConstStringRef negativeNumber = "-1f9e"_s;
		int parsedNegativeNumber = 1;

		const char * const iteratorNegativeNumber = parseNumber<16>(negativeNumber.begin(),
				parsedNegativeNumber, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(-0x1f9e, parsedNegativeNumber);
		CPPUNIT_ASSERT_EQUAL(negativeNumber.end(), iteratorNegativeNumber);
	}

	{
		ConstStringRef zero = "0"_s;
		int parsedZero = 1;

		const char * const iteratorZero = parseNumber<16>(zero.begin(),
				parsedZero, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(0, parsedZero);
		CPPUNIT_ASSERT_EQUAL(zero.end(), iteratorZero);
	}
}

void afc::NumberTest::testParseNumberCString_HexUnsignedInts()
{
	{
		ConstStringRef positiveNumber = "e05D"_s;
		unsigned parsedPositiveNumber = 1;

		const char * const iteratorPositiveNumber = parseNumber<16>(positiveNumber.begin(),
				parsedPositiveNumber, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(0xe05du, parsedPositiveNumber);
		CPPUNIT_ASSERT_EQUAL(positiveNumber.end(), iteratorPositiveNumber);
	}

	{
		ConstStringRef zero = "0"_s;
		unsigned parsedZero = 1;

		const char * const iteratorZero = parseNumber<16>(zero.begin(),
				parsedZero, [](const char *) { CPPUNIT_FAIL("parse error"); });
		CPPUNIT_ASSERT_EQUAL(0u, parsedZero);
		CPPUNIT_ASSERT_EQUAL(zero.end(), iteratorZero);
	}
}
