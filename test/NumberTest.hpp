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
#ifndef AFC_NUMBERTEST_HPP_
#define AFC_NUMBERTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class NumberTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(NumberTest);
		CPPUNIT_TEST(testAppendNumber_Int);
		CPPUNIT_TEST(testAppendNumber_MinSignedChar);
		CPPUNIT_TEST(testAppendNumber_MinSignedLongLong);

		CPPUNIT_TEST(testParseNumber_DecimalInts);
		CPPUNIT_TEST(testParseNumber_DecimalUnsignedInts);

		CPPUNIT_TEST(testParseNumber_HexInts);
		CPPUNIT_TEST(testParseNumber_HexUnsignedInts);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testAppendNumber_Int();
		void testAppendNumber_MinSignedChar();
		void testAppendNumber_MinSignedLongLong();

		void testParseNumber_DecimalInts();
		void testParseNumber_DecimalUnsignedInts();

		void testParseNumber_HexInts();
		void testParseNumber_HexUnsignedInts();
	};
}

#endif /* AFC_NUMBERTEST_HPP_ */
