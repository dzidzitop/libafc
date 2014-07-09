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
#ifndef AFC_INT32TEST_HPP_
#define AFC_INT32TEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class Int32Test : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(Int32Test);
		CPPUNIT_TEST(testInt32Conversions_PlatformEndianness);
		CPPUNIT_TEST(testInt32Conversions_LittleEndian);
		CPPUNIT_TEST(testInt32Conversions_BigEndian);
		CPPUNIT_TEST(testConversionToBytes_PlatformEndianness);
		CPPUNIT_TEST(testConversionToBytes_LittleEndian);
		CPPUNIT_TEST(testConversionToBytes_BigEndian);
		CPPUNIT_TEST(testInt32LE);
		CPPUNIT_TEST(testInt32BE);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testInt32Conversions_PlatformEndianness();
		void testInt32Conversions_LittleEndian();
		void testInt32Conversions_BigEndian();
		void testConversionToBytes_PlatformEndianness();
		void testConversionToBytes_LittleEndian();
		void testConversionToBytes_BigEndian();
		void testInt32LE();
		void testInt32BE();
	};
}

#endif /* AFC_INT32TEST_HPP_ */
