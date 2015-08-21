/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2015 Dźmitry Laŭčuk

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

#include <afc/StringRef.hpp>

using afc::operator"" _s;
using afc::ConstStringRef;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::StringUtilTest);

void afc::StringUtilTest::testEndsWith()
{
	ConstStringRef str = "str"_s;
	ConstStringRef s = "s"_s;
	ConstStringRef empty = ""_s;
	ConstStringRef r = "r"_s;
	ConstStringRef tr = "tr"_s;
	ConstStringRef rt = "rt"_s;
	ConstStringRef astr = "astr"_s;
	ConstStringRef z = "z"_s;
	ConstStringRef ss = "ss"_s;

	CPPUNIT_ASSERT(endsWith(str.begin(), str.end(), r.begin(), r.end()));
	CPPUNIT_ASSERT(endsWith(str.begin(), str.end(), tr.begin(), tr.end()));
	CPPUNIT_ASSERT(endsWith(str.begin(), str.end(), str.begin(), str.end()));
	CPPUNIT_ASSERT(endsWith(str.begin(), str.end(), empty.begin(), empty.end()));
	CPPUNIT_ASSERT(!endsWith(str.begin(), str.end(), rt.begin(), rt.end()));
	CPPUNIT_ASSERT(!endsWith(str.begin(), str.end(), astr.begin(), astr.end()));
	CPPUNIT_ASSERT(endsWith(s.begin(), s.end(), empty.begin(), empty.end()));
	CPPUNIT_ASSERT(endsWith(s.begin(), s.end(), s.begin(), s.end()));
	CPPUNIT_ASSERT(!endsWith(s.begin(), s.end(), z.begin(), z.end()));
	CPPUNIT_ASSERT(!endsWith(s.begin(), s.end(), ss.begin(), ss.end()));
	CPPUNIT_ASSERT(endsWith(empty.begin(), empty.end(), empty.begin(), empty.end()));
	CPPUNIT_ASSERT(!endsWith(empty.begin(), empty.end(), z.begin(), z.end()));
	CPPUNIT_ASSERT(!endsWith(empty.begin(), empty.end(), ss.begin(), ss.end()));
}
