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
#ifndef AFC_ENCODEBASE64TEST_H_
#define AFC_ENCODEBASE64TEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	// TODO Add tests that test all possible values of an octet.
	class EncodeBase64Test : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(EncodeBase64Test);
		CPPUNIT_TEST(testString_EncodeEmptyString);
		CPPUNIT_TEST(testString_EncodeOctet);
		CPPUNIT_TEST(testString_EncodeTwoOctets);
		CPPUNIT_TEST(testString_EncodeTriplet);
		CPPUNIT_TEST(testString_EncodeTripletAndOctet);
		CPPUNIT_TEST(testString_EncodeTripletAndTwoOctets);
		CPPUNIT_TEST(testString_EncodeTwoTriplets);
		CPPUNIT_TEST(testString_EncodeTwoTripletsAndOctet);
		CPPUNIT_TEST(testString_EncodeTwoTripletsAndTwoOctets);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testString_EncodeEmptyString();
		void testString_EncodeOctet();
		void testString_EncodeTwoOctets();
		void testString_EncodeTriplet();
		void testString_EncodeTripletAndOctet();
		void testString_EncodeTripletAndTwoOctets();
		void testString_EncodeTwoTriplets();
		void testString_EncodeTwoTripletsAndOctet();
		void testString_EncodeTwoTripletsAndTwoOctets();
	};
}

#endif /* AFC_ENCODEBASE64TEST_H_ */
