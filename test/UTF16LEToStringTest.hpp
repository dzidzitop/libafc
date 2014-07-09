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
#ifndef AFC_UTF16LETOSTRINGTEST_HPP_
#define AFC_UTF16LETOSTRINGTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	// TODO add tests with surrogate pairs
	// TODO add tests for error cases
	class UTF16LEToStringTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(UTF16LEToStringTest);
		CPPUNIT_TEST(testEmptyString_toASCII);
		CPPUNIT_TEST(testNonEmptyString_toASCII);
		CPPUNIT_TEST(testMultiByteNonEmptyString_toASCII_withTransliteration);
		CPPUNIT_TEST(testEmptyString_toUTF8);
		CPPUNIT_TEST(testSimpleNonEmptyString_toUTF8);
		CPPUNIT_TEST(testMultiByteNonEmptyString_toUTF8);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testEmptyString_toASCII();
		void testNonEmptyString_toASCII();
		void testMultiByteNonEmptyString_toASCII_withTransliteration();
		void testEmptyString_toUTF8();
		void testSimpleNonEmptyString_toUTF8();
		void testMultiByteNonEmptyString_toUTF8();
	};
}

#endif /* AFC_UTF16LETOSTRINGTEST_HPP_ */
