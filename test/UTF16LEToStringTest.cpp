/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2014 Dźmitry Laŭčuk

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
#include <afc/utils.h>
#include <string>

using namespace std;
using namespace afc;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::UTF16LEToStringTest);

void afc::UTF16LEToStringTest::testEmptyString_toASCII()
{
	string s(utf16leToString(u16string(), "ascii"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(""), s);
}

void afc::UTF16LEToStringTest::testNonEmptyString_toASCII()
{
	string s(utf16leToString(u16string(u"Hello, World!"), "ascii"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string("Hello, World!"), s);
}

void afc::UTF16LEToStringTest::testMultiByteNonEmptyString_toASCII_withTransliteration()
{
	string s(utf16leToString(u16string(u"\u0160\u010da\u015bcie"), "ascii//TRANSLIT"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8"??a?cie"), s);
}

void afc::UTF16LEToStringTest::testEmptyString_toUTF8()
{
	string s(utf16leToString(u16string(), "UTF-8"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8""), s);
}

void afc::UTF16LEToStringTest::testSimpleNonEmptyString_toUTF8()
{
	string s(utf16leToString(u16string(u"Hello, World!"), "UTF-8"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8"Hello, World!"), s);
}

void afc::UTF16LEToStringTest::testMultiByteNonEmptyString_toUTF8()
{
	string s(utf16leToString(u16string(u"\u0160\u010da\u015bcie"), "UTF-8"));

	CPPUNIT_ASSERT_EQUAL_MESSAGE(s.c_str(), string(u8"\u0160\u010da\u015bcie"), s);
}
