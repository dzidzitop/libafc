/* libafc - utils to facilitate C++ development.
Copyright (C) 2019 Dźmitry Laŭčuk

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
#ifndef AFC_CRCTEST_HPP_
#define AFC_CRCTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class CrcTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(CrcTest);
		CPPUNIT_TEST(testCrc64Reversed);
		CPPUNIT_TEST(testCrc64ReversedUpdate);
		CPPUNIT_TEST(testCrc64Reversed_Iterator);
		CPPUNIT_TEST(testCrc64ReversedUpdate_Iterator);
		CPPUNIT_TEST(testCrc64Reversed_Aligned8);
		CPPUNIT_TEST(testCrc64ReversedUpdate_Aligned8);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testCrc64Reversed();
		void testCrc64ReversedUpdate();
		void testCrc64Reversed_Iterator();
		void testCrc64ReversedUpdate_Iterator();
		void testCrc64Reversed_Aligned8();
		void testCrc64ReversedUpdate_Aligned8();
	};
}

#endif /* AFC_CRCTEST_HPP_ */
