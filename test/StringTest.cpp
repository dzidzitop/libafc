/* libafc - utils to facilitate C++ development.
Copyright (C) 2016 Dźmitry Laŭčuk

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
#include "StringTest.hpp"
#include <afc/SimpleString.hpp>

#include <string>
#include <afc/StringRef.hpp>

using afc::operator"" _s;
using afc::ConstStringRef;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::StringTest);

void afc::StringTest::testCopyConstructor_EmptyString()
{
	afc::String src;
	afc::String dest(src);

	CPPUNIT_ASSERT_EQUAL(std::size_t(0), src.size());
	CPPUNIT_ASSERT_EQUAL(std::size_t(0), dest.size());

	// Testing strings are unrelated.
	src.assign("Hello", "Hello"_s.size());

	CPPUNIT_ASSERT_EQUAL("Hello"_s.size(), src.size());
	CPPUNIT_ASSERT_EQUAL(std::size_t(0), dest.size());
}

void afc::StringTest::testCopyConstructor_NonEmptyString()
{
	afc::String src("SuperWorld"_s);
	afc::String dest(src);

	CPPUNIT_ASSERT_EQUAL("SuperWorld"_s.size(), src.size());
	CPPUNIT_ASSERT_EQUAL("SuperWorld"_s.size(), dest.size());

	// Testing strings are unrelated.
	src.assign("Hello", "Hello"_s.size());

	CPPUNIT_ASSERT_EQUAL("Hello"_s.size(), src.size());
	CPPUNIT_ASSERT_EQUAL("SuperWorld"_s.size(), dest.size());
	CPPUNIT_ASSERT_EQUAL(std::string("SuperWorld"), std::string(dest.c_str()));
}
