/* libafc - utils to facilitate C++ development.
Copyright (C) 2017 Dźmitry Laŭčuk

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
#include "ConvertCharsetTest.hpp"
#include <afc/utils.h>

#include <afc/StringRef.hpp>
#include <afc/SimpleString.hpp>
#include <string>

using afc::operator"" _s;
using afc::ConstStringRef;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::ConvertCharsetTest);

void afc::ConvertCharsetTest::testConvertToUtf8(void)
{
	{
		const auto input = ""_s;
		afc::U8String result = afc::convertToUtf8(input.value(), "ASCII");
		CPPUNIT_ASSERT_EQUAL(std::string(input.begin(), input.end()), std::string(result.begin(), result.end()));
	}

	{
		const auto input = "Hello, World!"_s;
		afc::U8String result = afc::convertToUtf8(input.value(), "ASCII");
		CPPUNIT_ASSERT_EQUAL(std::string(input.begin(), input.end()), std::string(result.begin(), result.end()));
	}

	{
		const auto input = "Najvialik" "\x9a" "aje baha" "\xe6" "cie"_s;
		afc::U8String result = afc::convertToUtf8(input.value(), "CP1250");
		CPPUNIT_ASSERT_EQUAL(std::string(u8"Najvialik\u0161aje baha\u0107cie"), std::string(result.begin(), result.end()));
	}
}
