/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2015 Dźmitry Laŭčuk

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
#include "UTF16LEToStringTest.hpp"

#include <string>

#include <afc/SimpleString.hpp>
#include <afc/utils.h>

using namespace std;
using namespace afc;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::UTF16LEToStringTest);

void afc::UTF16LEToStringTest::testEmptyString_toASCII()
{
	afc::String s(utf16leToString(u"", 0, "ascii"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8""), string(s.begin(), s.end()));
}

void afc::UTF16LEToStringTest::testNonEmptyString_toASCII()
{
	afc::String s(utf16leToString(u"Hello, World!", 13, "ascii"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8"Hello, World!"), string(s.begin(), s.end()));
}

void afc::UTF16LEToStringTest::testMultiByteNonEmptyString_toASCII_withTransliteration()
{
	afc::String s(utf16leToString(u"\u0160\u010da\u015bcie", 7, "ascii//TRANSLIT"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8"??a?cie"), string(s.begin(), s.end()));
}

void afc::UTF16LEToStringTest::testEmptyString_toUTF8()
{
	afc::String s(utf16leToString(u"", 0, "UTF-8"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(), string(s.begin(), s.end()));
}

void afc::UTF16LEToStringTest::testSimpleNonEmptyString_toUTF8()
{
	afc::String s(utf16leToString(u"Hello, World!", 13, "UTF-8"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8"Hello, World!"), string(s.begin(), s.end()));
}

void afc::UTF16LEToStringTest::testMultiByteNonEmptyString_toUTF8()
{
	afc::String s(utf16leToString(u"\u0160\u010da\u015bcie", 7, "UTF-8"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8"\u0160\u010da\u015bcie"), string(s.begin(), s.end()));
}
