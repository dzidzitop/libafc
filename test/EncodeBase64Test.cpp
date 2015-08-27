/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2015 Dźmitry Laŭčuk

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
#include <string>
#include <iterator>

using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::EncodeBase64Test);

void afc::EncodeBase64Test::testString_EncodeEmptyString()
{
	string result;
	encodeBase64("", 0, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string(), result);
}

void afc::EncodeBase64Test::testString_EncodeOctet()
{
	string result;
	encodeBase64("M", 1, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TQ=="), result);
}

void afc::EncodeBase64Test::testString_EncodeTwoOctets()
{
	string result;
	encodeBase64("Ma", 2, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TWE="), result);
}

void afc::EncodeBase64Test::testString_EncodeTriplet()
{
	string result;
	encodeBase64("Man", 3, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TWFu"), result);
}

void afc::EncodeBase64Test::testString_EncodeTripletAndOctet()
{
	string result;
	encodeBase64("ManM", 4, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TWFuTQ=="), result);
}

void afc::EncodeBase64Test::testString_EncodeTripletAndTwoOctets()
{
	string result;
	encodeBase64("ManMa", 5, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("TWFuTWE="), result);
}

void afc::EncodeBase64Test::testString_EncodeTwoTriplets()
{
	string result;
	encodeBase64("Triple", 6, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxl"), result);
}

void afc::EncodeBase64Test::testString_EncodeTwoTripletsAndOctet()
{
	string result;
	encodeBase64("TripleM", 7, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxlTQ=="), result);
}

void afc::EncodeBase64Test::testString_EncodeTwoTripletsAndTwoOctets()
{
	string result;
	encodeBase64("TripleMa", 8, back_inserter(result));

	CPPUNIT_ASSERT_EQUAL(string("VHJpcGxlTWE="), result);
}
