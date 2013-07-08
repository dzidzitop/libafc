#include "Int32Test.h"
#include <afc/cpu/primitive.h>

const afc::endianness LE = afc::endianness::LE;
const afc::endianness BE = afc::endianness::BE;

afc::Int32Test::Int32Test()
{
	TEST_ADD(Int32Test::testInt32Conversions_PlatformEndianness);
	TEST_ADD(Int32Test::testInt32Conversions_LittleEndian);
	TEST_ADD(Int32Test::testInt32Conversions_BigEndian);
	TEST_ADD(Int32Test::testConversionToBytes_PlatformEndianness);
	TEST_ADD(Int32Test::testConversionToBytes_LittleEndian);
	TEST_ADD(Int32Test::testConversionToBytes_BigEndian);
	TEST_ADD(Int32Test::testInt32LE);
	TEST_ADD(Int32Test::testInt32BE);
}

void afc::Int32Test::testInt32Conversions_PlatformEndianness()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		TEST_ASSERT(Int32<>(0x12345678) == 0x12345678);
		TEST_ASSERT(Int32<>(0x12345678, LE) == 0x12345678);
		TEST_ASSERT(Int32<>(0x12345678, BE) == 0x78563412);
	} else {
		TEST_ASSERT(Int32<>(0x12345678) == 0x12345678);
		TEST_ASSERT(Int32<>(0x12345678, LE) == 0x78563412);
		TEST_ASSERT(Int32<>(0x12345678, BE) == 0x12345678);
	}
}

void afc::Int32Test::testInt32Conversions_LittleEndian()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		TEST_ASSERT(Int32<LE>(0x12345678) == 0x12345678);
		TEST_ASSERT(Int32<LE>(0x12345678, LE) == 0x12345678);
		TEST_ASSERT(Int32<LE>(0x12345678, BE) == 0x78563412);
	} else {
		TEST_ASSERT(Int32<LE>(0x12345678) == 0x78563412);
		TEST_ASSERT(Int32<LE>(0x12345678, LE) == 0x12345678);
		TEST_ASSERT(Int32<LE>(0x12345678, BE) == 0x78563412);
	}
}

void afc::Int32Test::testInt32Conversions_BigEndian()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		TEST_ASSERT(Int32<BE>(0x12345678) == 0x78563412);
		TEST_ASSERT(Int32<BE>(0x12345678, LE) == 0x78563412);
		TEST_ASSERT(Int32<BE>(0x12345678, BE) == 0x12345678);
	} else {
		TEST_ASSERT(Int32<BE>(0x12345678) == 0x12345678);
		TEST_ASSERT(Int32<BE>(0x12345678, LE) == 0x78563412);
		TEST_ASSERT(Int32<BE>(0x12345678, BE) == 0x12345678);
	}
}

void afc::Int32Test::testConversionToBytes_PlatformEndianness()
{
	TEST_FAIL("implement me");
}

void afc::Int32Test::testConversionToBytes_LittleEndian()
{
	TEST_FAIL("implement me");
}

void afc::Int32Test::testConversionToBytes_BigEndian()
{
	TEST_FAIL("implement me");
}

void afc::Int32Test::testInt32LE()
{
	if (PLATFORM_BYTE_ORDER == LE) {
		{
			const Int32<LE> i(0x12345678);

			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x12345678);
		}
		{
			const Int32<LE> i(0x12345678, LE);

			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x12345678);
		}
		{
			const Int32<LE> i(0x12345678, BE);
			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x78563412);
		}
		{
			const unsigned char in[4] = {0x12, 0x34, 0x56, 0x78};
			const Int32<LE> i = Int32<LE>::fromBytes<LE>(in);
			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x78563412);
		}
		{
			const unsigned char in[4] = {0x12, 0x34, 0x56, 0x78};
			const Int32<LE> i = Int32<LE>::fromBytes<BE>(in);
			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x12345678);
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
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x78563412);
		}
		{
			const Int32<BE> i(0x12345678, LE);

			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x78563412);
		}
		{
			const Int32<BE> i(0x12345678, BE);
			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x12345678);
		}
		{
			const unsigned char in[4] = {0x12, 0x34, 0x56, 0x78};
			const Int32<BE> i = Int32<BE>::fromBytes<LE>(in);
			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x12345678);
		}
		{
			const unsigned char in[4] = {0x12, 0x34, 0x56, 0x78};
			const Int32<BE> i = Int32<BE>::fromBytes<BE>(in);
			unsigned char out[4];

			i.toBytes<LE>(out);
			TEST_ASSERT(out[0] == 0x78);
			TEST_ASSERT(out[1] == 0x56);
			TEST_ASSERT(out[2] == 0x34);
			TEST_ASSERT(out[3] == 0x12);

			i.toBytes<BE>(out);
			TEST_ASSERT(out[0] == 0x12);
			TEST_ASSERT(out[1] == 0x34);
			TEST_ASSERT(out[2] == 0x56);
			TEST_ASSERT(out[3] == 0x78);

			TEST_ASSERT(static_cast<int32_t>(i) == 0x78563412);
		}
	} else {
		// TODO
	}
}
