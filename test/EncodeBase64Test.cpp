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
#include "EncodeBase64Test.hpp"
#include <afc/base64.hpp>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::EncodeBase64Test);

void afc::EncodeBase64Test::testString_EncodeEmptyString()
{
	CPPUNIT_ASSERT_EQUAL(string(), encodeBase64(string()));
}

void afc::EncodeBase64Test::testString_EncodeOctet()
{
	CPPUNIT_ASSERT_EQUAL(string("TQ=="), encodeBase64(string("M")));
}

void afc::EncodeBase64Test::testString_EncodeTwoOctets()
{
	CPPUNIT_ASSERT_EQUAL(string("TWE="), encodeBase64(string("Ma")));
}

void afc::EncodeBase64Test::testString_EncodeTriplet()
{
	CPPUNIT_ASSERT_EQUAL(string("TWFu"), encodeBase64(string("Man")));
}

void afc::EncodeBase64Test::testString_EncodeTripletAndOctet()
{
	CPPUNIT_ASSERT_EQUAL(string("TWFuTQ=="), encodeBase64(string("ManM")));
}

void afc::EncodeBase64Test::testString_EncodeTripletAndTwoOctets()
{
	CPPUNIT_ASSERT_EQUAL(string("TWFuTWE="), encodeBase64(string("ManMa")));
}

void afc::EncodeBase64Test::testString_EncodeTwoTriplets()
{
	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxl"), encodeBase64(string("Triple")));
}

void afc::EncodeBase64Test::testString_EncodeTwoTripletsAndOctet()
{
	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxlTQ=="), encodeBase64(string("TripleM")));
}

void afc::EncodeBase64Test::testString_EncodeTwoTripletsAndTwoOctets()
{
	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxlTWE="), encodeBase64(string("TripleMa")));
}
