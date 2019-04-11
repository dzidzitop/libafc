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
		CPPUNIT_TEST(testCrc64);
		CPPUNIT_TEST(testCrc64Update);
		CPPUNIT_TEST(testCrc64_Iterator);
		CPPUNIT_TEST(testCrc64Update_Iterator);
		CPPUNIT_TEST(testCrc64_Aligned8);
		CPPUNIT_TEST(testCrc64Update_Aligned8);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testCrc64();
		void testCrc64Update();
		void testCrc64_Iterator();
		void testCrc64Update_Iterator();
		void testCrc64_Aligned8();
		void testCrc64Update_Aligned8();
	};
}

#endif /* AFC_CRCTEST_HPP_ */
