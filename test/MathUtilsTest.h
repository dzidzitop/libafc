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
#ifndef AFC_MATHUTILSTEST_H_
#define AFC_MATHUTILSTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class MathUtilsTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(MathUtilsTest);
		CPPUNIT_TEST(testSameSign);
		CPPUNIT_TEST(testIsEven);
		CPPUNIT_TEST(testIsOdd);
		CPPUNIT_TEST(testMean_Signed);
		CPPUNIT_TEST(testMean_Unsigned);
		CPPUNIT_TEST(testMean_SignedLong);
		CPPUNIT_TEST(testMean_UnsignedLong);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testSameSign();
		void testIsEven();
		void testIsOdd();
		void testMean_Signed();
		void testMean_Unsigned();
		void testMean_SignedLong();
		void testMean_UnsignedLong();
	};
}

#endif /*AFC_MATHUTILSTEST_H_*/
