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
#include "StringUtilTest.hpp"
#include <afc/string_util.hpp>

using std::string;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::StringUtilTest);

void afc::StringUtilTest::testStartsWith()
{
	CPPUNIT_ASSERT(startsWith(string("str"), string("s")));
	CPPUNIT_ASSERT(startsWith(string("str"), string("st")));
	CPPUNIT_ASSERT(startsWith(string("str"), string("str")));
	CPPUNIT_ASSERT(!startsWith(string("str"), string("str2")));
	CPPUNIT_ASSERT(!startsWith(string("str"), string("sTr")));
	CPPUNIT_ASSERT(!startsWith(string("str"), string("S")));
	CPPUNIT_ASSERT(startsWith(string("str"), string("")));
	CPPUNIT_ASSERT(startsWith(string(""), string("")));
	CPPUNIT_ASSERT(!startsWith(string(""), string("sWW")));
	CPPUNIT_ASSERT(!startsWith(string("z"), string("s")));
	CPPUNIT_ASSERT(!startsWith(string("z"), string("zz")));
	CPPUNIT_ASSERT(startsWith(string("z"), string("z")));
	CPPUNIT_ASSERT(startsWith(string("z"), string("")));
}

void afc::StringUtilTest::testEndsWith()
{
	CPPUNIT_ASSERT(endsWith(string("str"), string("r")));
	CPPUNIT_ASSERT(endsWith(string("str"), string("tr")));
	CPPUNIT_ASSERT(endsWith(string("str"), string("str")));
	CPPUNIT_ASSERT(endsWith(string("str"), string("")));
	CPPUNIT_ASSERT(!endsWith(string("str"), string("rt")));
	CPPUNIT_ASSERT(!endsWith(string("str"), string("astr")));
	CPPUNIT_ASSERT(endsWith(string("s"), string("")));
	CPPUNIT_ASSERT(endsWith(string("s"), string("s")));
	CPPUNIT_ASSERT(!endsWith(string("s"), string("z")));
	CPPUNIT_ASSERT(!endsWith(string("s"), string("ss")));
	CPPUNIT_ASSERT(endsWith(string(""), string("")));
	CPPUNIT_ASSERT(!endsWith(string(""), string("z")));
	CPPUNIT_ASSERT(!endsWith(string(""), string("ss")));
}
