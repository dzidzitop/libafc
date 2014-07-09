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
#include "Int32Test.h"
#include <afc/cpu/primitive.h>

const afc::endianness LE = afc::endianness::LE;
const afc::endianness BE = afc::endianness::BE;
typedef unsigned char uc;

void afc::Int32Test::testInt32Conversions_PlatformEndianness()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<>(0x12345678).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<>(0x12345678, LE).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), Int32<>(0x12345678, BE).value());
	} else {
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<>(0x12345678).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), Int32<>(0x12345678, LE).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<>(0x12345678, BE).value());
	}
}

void afc::Int32Test::testInt32Conversions_LittleEndian()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<LE>(0x12345678).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<LE>(0x12345678, LE).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), Int32<LE>(0x12345678, BE).value());
	} else {
		CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), Int32<LE>(0x12345678).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<LE>(0x12345678, LE).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), Int32<LE>(0x12345678, BE).value());
	}
}

void afc::Int32Test::testInt32Conversions_BigEndian()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), Int32<BE>(0x12345678).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), Int32<BE>(0x12345678, LE).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<BE>(0x12345678, BE).value());
	} else {
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<BE>(0x12345678).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), Int32<BE>(0x12345678, LE).value());
		CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), Int32<BE>(0x12345678, BE).value());
	}
}

void afc::Int32Test::testConversionToBytes_PlatformEndianness()
{
	CPPUNIT_FAIL("implement me");
}

void afc::Int32Test::testConversionToBytes_LittleEndian()
{
	CPPUNIT_FAIL("implement me");
}

void afc::Int32Test::testConversionToBytes_BigEndian()
{
	CPPUNIT_FAIL("implement me");
}

void afc::Int32Test::testInt32LE()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		{
			const Int32<LE> i(0x12345678);

			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), static_cast<int32_t>(i));
		}
		{
			const Int32<LE> i(0x12345678, LE);

			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), static_cast<int32_t>(i));
		}
		{
			const Int32<LE> i(0x12345678, BE);
			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), static_cast<int32_t>(i));
		}
		{
			const unsigned char in[4] = {0x12, 0x34, 0x56, 0x78};
			const Int32<LE> i = Int32<LE>::fromBytes<LE>(in);
			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), static_cast<int32_t>(i));
		}
		{
			const unsigned char in[4] = {0x12, 0x34, 0x56, 0x78};
			const Int32<LE> i = Int32<LE>::fromBytes<BE>(in);
			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), static_cast<int32_t>(i));
		}
	} else {
		// TODO
	}
}

void afc::Int32Test::testInt32BE()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		{
			const Int32<BE> i(0x12345678);

			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), static_cast<int32_t>(i));
		}
		{
			const Int32<BE> i(0x12345678, LE);

			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), static_cast<int32_t>(i));
		}
		{
			const Int32<BE> i(0x12345678, BE);
			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), static_cast<int32_t>(i));
		}
		{
			const unsigned char in[4] = {0x12, 0x34, 0x56, 0x78};
			const Int32<BE> i = Int32<BE>::fromBytes<LE>(in);
			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x12345678), static_cast<int32_t>(i));
		}
		{
			const unsigned char in[4] = {0x12, 0x34, 0x56, 0x78};
			const Int32<BE> i = Int32<BE>::fromBytes<BE>(in);
			unsigned char out[4];

			i.toBytes<LE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[3]);

			i.toBytes<BE>(out);
			CPPUNIT_ASSERT_EQUAL(uc(0x12), out[0]);
			CPPUNIT_ASSERT_EQUAL(uc(0x34), out[1]);
			CPPUNIT_ASSERT_EQUAL(uc(0x56), out[2]);
			CPPUNIT_ASSERT_EQUAL(uc(0x78), out[3]);

			CPPUNIT_ASSERT_EQUAL(int32_t(0x78563412), static_cast<int32_t>(i));
		}
	} else {
		// TODO
	}
}
