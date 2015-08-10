/* libafc - utils to facilitate C++ development.
Copyright (C) 2015 Dźmitry Laŭčuk

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
#include "JSONObjectParserTest.hpp"

#include <algorithm>
#include <string>

#include <afc/json.hpp>
#include <afc/number.h>
#include <afc/StringRef.hpp>

using afc::operator"" _s;
using std::string;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::JSONObjectParserTest);

struct ErrorHandler
{
	bool m_valid = true;

	void prematureEnd()
	{
		m_valid = false;
	}

	template<typename Iterator>
	void malformedJson(Iterator pos)
	{
		m_valid = false;
	}

	bool valid()
	{
		return m_valid;
	}
};

void afc::JSONObjectParserTest::testEmptyObject()
{
	bool objectBodyParserCalled = false;
	afc::ConstStringRef input = u8"{}"_s;
	auto objectBodyParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
	{
		CPPUNIT_ASSERT_EQUAL(input.begin() + 1, begin);
		CPPUNIT_ASSERT_EQUAL(input.end(), end);
		CPPUNIT_ASSERT(!objectBodyParserCalled);
		objectBodyParserCalled = true;
		return begin;
	};
	ErrorHandler errorHandler;

	const char * const result = afc::json::parseObject<const char *, decltype(objectBodyParser)&, ErrorHandler &>
			(input.begin(), input.end(), objectBodyParser, errorHandler);

	CPPUNIT_ASSERT_EQUAL(input.end(), result);
	CPPUNIT_ASSERT(objectBodyParserCalled);
	CPPUNIT_ASSERT(errorHandler.valid());
}

void afc::JSONObjectParserTest::testEmptyObjectWithOuterSpaces()
{
	bool objectBodyParserCalled = false;
	afc::ConstStringRef input = u8" \t  \n\v\f{}\n  \r\r\n"_s;
	auto objectBodyParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
	{
		CPPUNIT_ASSERT_EQUAL(input.begin() + 8, begin);
		CPPUNIT_ASSERT_EQUAL(u8"}"[0], *begin);
		CPPUNIT_ASSERT_EQUAL(input.end(), end);
		CPPUNIT_ASSERT(!objectBodyParserCalled);
		objectBodyParserCalled = true;
		return begin;
	};
	ErrorHandler errorHandler;

	const char * const result = afc::json::parseObject<const char *, decltype(objectBodyParser)&, ErrorHandler &>
			(input.begin(), input.end(), objectBodyParser, errorHandler);

	CPPUNIT_ASSERT_EQUAL(input.end(), result);
	CPPUNIT_ASSERT(objectBodyParserCalled);
	CPPUNIT_ASSERT(errorHandler.valid());
}

void afc::JSONObjectParserTest::testEmptyObjectWithInnerSpaces()
{
	bool objectBodyParserCalled = false;
	afc::ConstStringRef input = u8"{  \t\n \v}"_s;
	auto objectBodyParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
	{
		CPPUNIT_ASSERT_EQUAL(input.begin() + 1, begin);
		CPPUNIT_ASSERT_EQUAL(input.end(), end);
		CPPUNIT_ASSERT(!objectBodyParserCalled);
		objectBodyParserCalled = true;
		const char * const bodyEnd = std::find(begin, end, u8"}"[0]);
		CPPUNIT_ASSERT_EQUAL(begin + 6, bodyEnd);
		CPPUNIT_ASSERT_EQUAL(u8"}"[0], *bodyEnd);
		return bodyEnd;
	};
	ErrorHandler errorHandler;

	const char * const result = afc::json::parseObject<const char *, decltype(objectBodyParser)&, ErrorHandler &>
			(input.begin(), input.end(), objectBodyParser, errorHandler);

	CPPUNIT_ASSERT_EQUAL(input.end(), result);
	CPPUNIT_ASSERT(objectBodyParserCalled);
	CPPUNIT_ASSERT(errorHandler.valid());
}

void afc::JSONObjectParserTest::testObjectWithStringProperty()
{
	afc::ConstStringRef input = u8"{\"hello\":\"world\"}"_s;
	std::string propertyName;
	std::string propertyValue;

	ErrorHandler errorHandler;

	auto objectBodyParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
	{
		CPPUNIT_ASSERT_EQUAL(input.begin() + 1, begin);
		CPPUNIT_ASSERT_EQUAL(u8"\""[0], *begin);
		CPPUNIT_ASSERT_EQUAL(input.end(), end);

		auto propertyNameParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
		{
			CPPUNIT_ASSERT_EQUAL(input.begin() + 2, begin);
			CPPUNIT_ASSERT_EQUAL(u8"h"[0], *begin);
			CPPUNIT_ASSERT_EQUAL(input.end(), end);

			const char * const propNameEnd = std::find(begin, end, u8"\""[0]);

			CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5, propNameEnd);
			CPPUNIT_ASSERT_EQUAL(u8"\""[0], *propNameEnd);

			propertyName.assign(begin, propNameEnd);

			return propNameEnd;
		};

		auto propertyValueParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
		{
			CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5 + 3, begin);
			CPPUNIT_ASSERT_EQUAL(u8"w"[0], *begin);
			CPPUNIT_ASSERT_EQUAL(input.end(), end);

			const char * const propValueEnd = std::find(begin, end, u8"\""[0]);

			CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5 + 3 + 5, propValueEnd);
			CPPUNIT_ASSERT_EQUAL(u8"\""[0], *propValueEnd);

			propertyValue.assign(begin, propValueEnd);

			return propValueEnd;
		};

		const char *i = afc::json::parseString(begin, end, propertyNameParser, errorHandler);

		CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5 + 1, i);
		CPPUNIT_ASSERT_EQUAL(u8":"[0], *i);

		CPPUNIT_ASSERT_EQUAL(string("hello"), propertyName);
		CPPUNIT_ASSERT_EQUAL(string(), propertyValue);

		i = afc::json::parseString(++i, end, propertyValueParser, errorHandler);

		CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5 + 3 + 5 + 1, i);
		CPPUNIT_ASSERT_EQUAL(u8"}"[0], *i);

		CPPUNIT_ASSERT_EQUAL(string("hello"), propertyName);
		CPPUNIT_ASSERT_EQUAL(string("world"), propertyValue);

		return i;
	};

	const char * const result = afc::json::parseObject<const char *, decltype(objectBodyParser)&, ErrorHandler &>
			(input.begin(), input.end(), objectBodyParser, errorHandler);

	CPPUNIT_ASSERT_EQUAL(input.end(), result);
	CPPUNIT_ASSERT(errorHandler.valid());
}

void afc::JSONObjectParserTest::testObjectWithIntProperty()
{
	afc::ConstStringRef input = u8"{\"hello\":12345}"_s;
	std::string propertyName;
	int propertyValue = 0;

	ErrorHandler errorHandler;

	auto objectBodyParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
	{
		CPPUNIT_ASSERT_EQUAL(input.begin() + 1, begin);
		CPPUNIT_ASSERT_EQUAL(u8"\""[0], *begin);
		CPPUNIT_ASSERT_EQUAL(input.end(), end);

		auto propertyNameParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
		{
			CPPUNIT_ASSERT_EQUAL(input.begin() + 2, begin);
			CPPUNIT_ASSERT_EQUAL(u8"h"[0], *begin);
			CPPUNIT_ASSERT_EQUAL(input.end(), end);

			const char * const propNameEnd = std::find(begin, end, u8"\""[0]);

			CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5, propNameEnd);
			CPPUNIT_ASSERT_EQUAL(u8"\""[0], *propNameEnd);

			propertyName.assign(begin, propNameEnd);

			return propNameEnd;
		};

		auto propertyValueParser = [&](const char * const begin, const char * const end, ErrorHandler &errorHandler) -> const char *
		{
			CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5 + 2, begin);
			CPPUNIT_ASSERT_EQUAL(u8"1"[0], *begin);
			CPPUNIT_ASSERT_EQUAL(input.end(), end);

			const char * const propValueEnd = afc::parseNumber<10>(begin, end, propertyValue, [](const char *) { CPPUNIT_FAIL("Parse error."); });

			CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5 + 2 + 5, propValueEnd);

			return propValueEnd;
		};

		const char *i = afc::json::parseString(begin, end, propertyNameParser, errorHandler);

		CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5 + 1, i);
		CPPUNIT_ASSERT_EQUAL(u8":"[0], *i);

		CPPUNIT_ASSERT_EQUAL(string("hello"), propertyName);
		CPPUNIT_ASSERT_EQUAL(0, propertyValue);

		i = afc::json::parseNumber(++i, end, propertyValueParser, errorHandler);

		CPPUNIT_ASSERT_EQUAL(input.begin() + 2 + 5 + 2 + 5, i);
		CPPUNIT_ASSERT_EQUAL(u8"}"[0], *i);

		CPPUNIT_ASSERT_EQUAL(string("hello"), propertyName);
		CPPUNIT_ASSERT_EQUAL(12345, propertyValue);

		return i;
	};

	const char * const result = afc::json::parseObject<const char *, decltype(objectBodyParser)&, ErrorHandler &>
			(input.begin(), input.end(), objectBodyParser, errorHandler);

	CPPUNIT_ASSERT_EQUAL(input.end(), result);
	CPPUNIT_ASSERT(errorHandler.valid());
}
