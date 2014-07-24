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
#ifndef AFC_NUMBERTEST_HPP_
#define AFC_NUMBERTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class NumberTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(NumberTest);
		CPPUNIT_TEST(testPrintNumber_Int);
		CPPUNIT_TEST(testPrintNumber_MinSignedChar);
		CPPUNIT_TEST(testPrintNumber_MinSignedLongLong);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testPrintNumber_Int();
		void testPrintNumber_MinSignedChar();
		void testPrintNumber_MinSignedLongLong();
	};
}

#endif /* AFC_NUMBERTEST_HPP_ */
