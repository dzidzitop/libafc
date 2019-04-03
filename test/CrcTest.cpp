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
#include "CrcTest.hpp"
#include <afc/crc.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(afc::CrcTest);

void afc::CrcTest::testCrc64()
{
	{
		unsigned char data[] = {};
		CPPUNIT_ASSERT_EQUAL(std::uint_fast64_t(0), afc::crc64(data, sizeof(data)));
	}

	{
		unsigned char data[] = {0x80};
		CPPUNIT_ASSERT_EQUAL(0xc96c5795d7870f42u, afc::crc64(data, sizeof(data)));
	}

	{
		unsigned char data[] = {0xde, 0xad};
		CPPUNIT_ASSERT_EQUAL(0x44277f18417c45a5u, afc::crc64(data, sizeof(data)));
	}

	{
		unsigned char data[] = {0xde, 0xad, 0xbe, 0xef};
		CPPUNIT_ASSERT_EQUAL(0xfc232c18806871afu, afc::crc64(data, sizeof(data)));
	}

	{
		unsigned char data[] =
				{0x99, 0xeb, 0x96, 0xdd, 0x94, 0xc8, 0x8e, 0x97, 0x5b, 0x58, 0x5d, 0x2f, 0x28, 0x78, 0x5e, 0x36};
		CPPUNIT_ASSERT_EQUAL(0xdb7ac38f63413c4eu, afc::crc64(data, sizeof(data)));
	}
}

void afc::CrcTest::testCrc64_Iterator()
{
	{
		CPPUNIT_ASSERT_EQUAL(std::uint_fast64_t(0),
				afc::crc64(static_cast<unsigned char *>(nullptr), static_cast<unsigned char *>(nullptr)));
	}

	{
		unsigned char nodata;
		CPPUNIT_ASSERT_EQUAL(std::uint_fast64_t(0), afc::crc64(&nodata, &nodata));
	}

	{
		unsigned char data[] = {0x80};
		CPPUNIT_ASSERT_EQUAL(0xc96c5795d7870f42u, afc::crc64(&data[0], &data[0] + sizeof(data)));
	}

	{
		unsigned char data[] = {0xde, 0xad};
		CPPUNIT_ASSERT_EQUAL(0x44277f18417c45a5u, afc::crc64(&data[0], &data[0] + sizeof(data)));
	}

	{
		unsigned char data[] = {0xde, 0xad, 0xbe, 0xef};
		CPPUNIT_ASSERT_EQUAL(0xfc232c18806871afu, afc::crc64(&data[0], &data[0] + sizeof(data)));
	}

	{
		unsigned char data[] =
				{0x99, 0xeb, 0x96, 0xdd, 0x94, 0xc8, 0x8e, 0x97, 0x5b, 0x58, 0x5d, 0x2f, 0x28, 0x78, 0x5e, 0x36};
		CPPUNIT_ASSERT_EQUAL(0xdb7ac38f63413c4eu, afc::crc64(&data[0], &data[0] + sizeof(data)));
	}
}
