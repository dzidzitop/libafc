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
#ifndef AFC_TOKENISERTEST_H_
#define AFC_TOKENISERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class TokeniserTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(TokeniserTest);
		CPPUNIT_TEST(testEmptyInputString);
		CPPUNIT_TEST(testSingleToken);
		CPPUNIT_TEST(testMultipleTokens);
		CPPUNIT_TEST(testMultipleTokensWithEmptyToken);
		CPPUNIT_TEST(testOnlyEmptyTokens);
		CPPUNIT_TEST(testWideStringMultipleTokens);
		CPPUNIT_TEST(testInputIsNotRvalue);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testEmptyInputString();
		void testSingleToken();
		void testMultipleTokens();
		void testMultipleTokensWithEmptyToken();
		void testOnlyEmptyTokens();
		void testWideStringMultipleTokens();
		void testInputIsNotRvalue();
	};
}

#endif /*AFC_TOKENISERTEST_H_*/
