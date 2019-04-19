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

#include "crc.hpp"
#include "builtin.hpp"
#include <memory>

namespace
{
	constexpr std::uint_fast64_t tableValLoop(const std::uint_fast64_t crc, const unsigned char iteration)
	{
		// Emulating 'for (int i = 0; i < 8; ++i)' with crc recalculated on each iteration.
		return iteration == 8 ? crc :
				tableValLoop((crc & 1) ? (crc >> 1) ^ afc::crc64Reversed_impl::polynome : crc >> 1, iteration + 1);
	}

	// for sequence xx
	constexpr std::uint_fast64_t tableVal(const unsigned char index)
	{
		return tableValLoop(index, 0);
	}

	// for sequence xx00
	constexpr std::uint_fast64_t tableVal2(const unsigned char index)
	{
		return (tableVal(index) >> 8) ^ tableVal(tableVal(index) & 0xff);
	}

	// for sequence xx0000
	constexpr std::uint_fast64_t tableVal3(const unsigned char index)
	{
		return (tableVal2(index) >> 8) ^ tableVal(tableVal2(index) & 0xff);
	}

	// for sequence xx000000
	constexpr std::uint_fast64_t tableVal4(const unsigned char index)
	{
		return (tableVal3(index) >> 8) ^ tableVal(tableVal3(index) & 0xff);
	}

	// for sequence xx00000000
	constexpr std::uint_fast64_t tableVal5(const unsigned char index)
	{
		return (tableVal4(index) >> 8) ^ tableVal(tableVal4(index) & 0xff);
	}

	// for sequence xx0000000000
	constexpr std::uint_fast64_t tableVal6(const unsigned char index)
	{
		return (tableVal5(index) >> 8) ^ tableVal(tableVal5(index) & 0xff);
	}

	// for sequence xx000000000000
	constexpr std::uint_fast64_t tableVal7(const unsigned char index)
	{
		return (tableVal6(index) >> 8) ^ tableVal(tableVal6(index) & 0xff);
	}

	// for sequence xx00000000000000
	constexpr std::uint_fast64_t tableVal8(const unsigned char index)
	{
		return (tableVal7(index) >> 8) ^ tableVal(tableVal7(index) & 0xff);
	}

	/* Processes 64 bits per iteration.
	 *
	 * In the classic CRC table method CPU must wait for a single input octet processed
	 * before it can start processing the next one. This can be improved.
	 *
	 * CRC is a linear function. This implies that CRC(XYZ) = CRC(X00 xor Y0 xor Z) =
	 * CRC(X00) xor CRC(YO) xor CRC(Z). Therefore, we can use tables with pre-calculated
	 * CRC for each X(0..0)n, 256 items each. If we have tables for up to X0000000 (8 bytes)
	 * then we can expand the CRC table method to process 64 bits of the input data
	 * independently. CPUs can take advantage of this because in this case there are much
	 * less data dependencies per iteration.
	 *
	 * Note, however, that CPUs with few registers can start using stack intensively.
	 * In addition, 16K+ of datas cache size is needed to keep tables which can be
	 * an overhead for some CPUs. So check if this method boosts performance and consider
	 * processing data in less chunks if it is more efficient in your conditions.
	 */
	inline std::uint_fast64_t crc64Fast64Impl(std::uint_fast64_t currentCrc,
			const unsigned char * const data, const std::size_t n)
	{
		using namespace afc::crc64Reversed_impl;

		assert(currentCrc == (currentCrc & 0xffffffffffffffff));
		assert(n > 0);
		assert(n % 8 == 0);

		std::size_t crc = currentCrc;
		std::size_t i = 0;
		do {
			crc = lookupTable8[(data[i] ^ crc) & 0xff] ^
					lookupTable7[(data[i + 1] ^ (crc >> 8)) & 0xff] ^
					lookupTable6[(data[i + 2] ^ (crc >> 16)) & 0xff] ^
					lookupTable5[(data[i + 3] ^ (crc >> 24)) & 0xff] ^
					lookupTable4[(data[i + 4] ^ (crc >> 32)) & 0xff] ^
					lookupTable3[(data[i + 5] ^ (crc >> 40)) & 0xff] ^
					lookupTable2[(data[i + 6] ^ (crc >> 48)) & 0xff] ^
					lookupTable[(data[i + 7] ^ (crc >> 56)) & 0xff];
			i += 8;
		} while (i < n);

		return crc;
	}

	/* Works as crc64Fast64Impl() but does more efficient xor of the next data chunk and current crc
	 * relying on that the platform is little-endian.
	 *
	 * This implementation is not portable and it is also a non-standard C++ code.
	 */
	inline std::uint_fast64_t crc64Fast64Aligned8Impl(std::uint_fast64_t currentCrc,
			const unsigned char * const data, const std::size_t n)
	{
		using namespace afc::crc64Reversed_impl;

		assert(afc::PLATFORM_BYTE_ORDER == afc::endianness::LE);
		assert(sizeof(std::uint_fast64_t) == 8 && CHAR_BIT == 8);
		assert((static_cast<std::uintptr_t>(data) & ~std::uintptr_t(0x07)) == static_cast<std::uintptr_t>(data));
		assert(currentCrc == (currentCrc & 0xffffffffffffffff));
		assert(n > 0);
		assert(n % 8 == 0);

		std::size_t crc = currentCrc;
		std::size_t i = 0;
		do {
			// Reading the data chunk as an uint64 value to xor it with crc at once.
			std::uint_fast64_t chunk = *reinterpret_cast<const std::uint_fast64_t *>(data + i);
			chunk ^= crc;
			crc = lookupTable8[chunk & 0xff] ^
					lookupTable7[(chunk >> 8) & 0xff] ^
					lookupTable6[(chunk >> 16) & 0xff] ^
					lookupTable5[(chunk >> 24) & 0xff] ^
					lookupTable4[(chunk >> 32) & 0xff] ^
					lookupTable3[(chunk >> 40) & 0xff] ^
					lookupTable2[(chunk >> 48) & 0xff] ^
					lookupTable[(chunk >> 56) & 0xff];
			i += 8;
		} while (i < n);

		return crc;
	}

	inline std::uint_fast64_t crc64ReversedUpdate_Aligned8Impl_Inline(const std::uint_fast64_t currentCrc,
			const unsigned char * const data, const std::size_t n)
	{
		assert(currentCrc == (currentCrc & 0xffffffffffffffff));

		const std::size_t fastN = n & ~std::size_t(0x07); // n - n % 8

		std::uint_fast64_t crc = currentCrc;

		if (fastN > 0) {
			// Calculating fast for as much data as possible.
			crc = crc64Fast64Aligned8Impl(crc, data, fastN);
		}

		// The rest of the data is calculated using the slow version of CRC64.
		for (std::size_t i = fastN; i < n; ++i) {
			crc = (crc >> 8) ^ afc::crc64Reversed_impl::lookupTable[(data[i] ^ crc) & 0xff];
		}

		return crc;
	}
}

// CRC64 value for each 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64Reversed_impl::lookupTable[0x100] = {
		tableVal(0x00), tableVal(0x01), tableVal(0x02), tableVal(0x03),
		tableVal(0x04), tableVal(0x05), tableVal(0x06), tableVal(0x07),
		tableVal(0x08), tableVal(0x09), tableVal(0x0a), tableVal(0x0b),
		tableVal(0x0c), tableVal(0x0d), tableVal(0x0e), tableVal(0x0f),
		tableVal(0x10), tableVal(0x11), tableVal(0x12), tableVal(0x13),
		tableVal(0x14), tableVal(0x15), tableVal(0x16), tableVal(0x17),
		tableVal(0x18), tableVal(0x19), tableVal(0x1a), tableVal(0x1b),
		tableVal(0x1c), tableVal(0x1d), tableVal(0x1e), tableVal(0x1f),
		tableVal(0x20), tableVal(0x21), tableVal(0x22), tableVal(0x23),
		tableVal(0x24), tableVal(0x25), tableVal(0x26), tableVal(0x27),
		tableVal(0x28), tableVal(0x29), tableVal(0x2a), tableVal(0x2b),
		tableVal(0x2c), tableVal(0x2d), tableVal(0x2e), tableVal(0x2f),
		tableVal(0x30), tableVal(0x31), tableVal(0x32), tableVal(0x33),
		tableVal(0x34), tableVal(0x35), tableVal(0x36), tableVal(0x37),
		tableVal(0x38), tableVal(0x39), tableVal(0x3a), tableVal(0x3b),
		tableVal(0x3c), tableVal(0x3d), tableVal(0x3e), tableVal(0x3f),
		tableVal(0x40), tableVal(0x41), tableVal(0x42), tableVal(0x43),
		tableVal(0x44), tableVal(0x45), tableVal(0x46), tableVal(0x47),
		tableVal(0x48), tableVal(0x49), tableVal(0x4a), tableVal(0x4b),
		tableVal(0x4c), tableVal(0x4d), tableVal(0x4e), tableVal(0x4f),
		tableVal(0x50), tableVal(0x51), tableVal(0x52), tableVal(0x53),
		tableVal(0x54), tableVal(0x55), tableVal(0x56), tableVal(0x57),
		tableVal(0x58), tableVal(0x59), tableVal(0x5a), tableVal(0x5b),
		tableVal(0x5c), tableVal(0x5d), tableVal(0x5e), tableVal(0x5f),
		tableVal(0x60), tableVal(0x61), tableVal(0x62), tableVal(0x63),
		tableVal(0x64), tableVal(0x65), tableVal(0x66), tableVal(0x67),
		tableVal(0x68), tableVal(0x69), tableVal(0x6a), tableVal(0x6b),
		tableVal(0x6c), tableVal(0x6d), tableVal(0x6e), tableVal(0x6f),
		tableVal(0x70), tableVal(0x71), tableVal(0x72), tableVal(0x73),
		tableVal(0x74), tableVal(0x75), tableVal(0x76), tableVal(0x77),
		tableVal(0x78), tableVal(0x79), tableVal(0x7a), tableVal(0x7b),
		tableVal(0x7c), tableVal(0x7d), tableVal(0x7e), tableVal(0x7f),
		tableVal(0x80), tableVal(0x81), tableVal(0x82), tableVal(0x83),
		tableVal(0x84), tableVal(0x85), tableVal(0x86), tableVal(0x87),
		tableVal(0x88), tableVal(0x89), tableVal(0x8a), tableVal(0x8b),
		tableVal(0x8c), tableVal(0x8d), tableVal(0x8e), tableVal(0x8f),
		tableVal(0x90), tableVal(0x91), tableVal(0x92), tableVal(0x93),
		tableVal(0x94), tableVal(0x95), tableVal(0x96), tableVal(0x97),
		tableVal(0x98), tableVal(0x99), tableVal(0x9a), tableVal(0x9b),
		tableVal(0x9c), tableVal(0x9d), tableVal(0x9e), tableVal(0x9f),
		tableVal(0xa0), tableVal(0xa1), tableVal(0xa2), tableVal(0xa3),
		tableVal(0xa4), tableVal(0xa5), tableVal(0xa6), tableVal(0xa7),
		tableVal(0xa8), tableVal(0xa9), tableVal(0xaa), tableVal(0xab),
		tableVal(0xac), tableVal(0xad), tableVal(0xae), tableVal(0xaf),
		tableVal(0xb0), tableVal(0xb1), tableVal(0xb2), tableVal(0xb3),
		tableVal(0xb4), tableVal(0xb5), tableVal(0xb6), tableVal(0xb7),
		tableVal(0xb8), tableVal(0xb9), tableVal(0xba), tableVal(0xbb),
		tableVal(0xbc), tableVal(0xbd), tableVal(0xbe), tableVal(0xbf),
		tableVal(0xc0), tableVal(0xc1), tableVal(0xc2), tableVal(0xc3),
		tableVal(0xc4), tableVal(0xc5), tableVal(0xc6), tableVal(0xc7),
		tableVal(0xc8), tableVal(0xc9), tableVal(0xca), tableVal(0xcb),
		tableVal(0xcc), tableVal(0xcd), tableVal(0xce), tableVal(0xcf),
		tableVal(0xd0), tableVal(0xd1), tableVal(0xd2), tableVal(0xd3),
		tableVal(0xd4), tableVal(0xd5), tableVal(0xd6), tableVal(0xd7),
		tableVal(0xd8), tableVal(0xd9), tableVal(0xda), tableVal(0xdb),
		tableVal(0xdc), tableVal(0xdd), tableVal(0xde), tableVal(0xdf),
		tableVal(0xe0), tableVal(0xe1), tableVal(0xe2), tableVal(0xe3),
		tableVal(0xe4), tableVal(0xe5), tableVal(0xe6), tableVal(0xe7),
		tableVal(0xe8), tableVal(0xe9), tableVal(0xea), tableVal(0xeb),
		tableVal(0xec), tableVal(0xed), tableVal(0xee), tableVal(0xef),
		tableVal(0xf0), tableVal(0xf1), tableVal(0xf2), tableVal(0xf3),
		tableVal(0xf4), tableVal(0xf5), tableVal(0xf6), tableVal(0xf7),
		tableVal(0xf8), tableVal(0xf9), tableVal(0xfa), tableVal(0xfb),
		tableVal(0xfc), tableVal(0xfd), tableVal(0xfe), tableVal(0xff)
};

// CRC64 value for each 00 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64Reversed_impl::lookupTable2[0x100] = {
		tableVal2(0x00), tableVal2(0x01), tableVal2(0x02), tableVal2(0x03),
		tableVal2(0x04), tableVal2(0x05), tableVal2(0x06), tableVal2(0x07),
		tableVal2(0x08), tableVal2(0x09), tableVal2(0x0a), tableVal2(0x0b),
		tableVal2(0x0c), tableVal2(0x0d), tableVal2(0x0e), tableVal2(0x0f),
		tableVal2(0x10), tableVal2(0x11), tableVal2(0x12), tableVal2(0x13),
		tableVal2(0x14), tableVal2(0x15), tableVal2(0x16), tableVal2(0x17),
		tableVal2(0x18), tableVal2(0x19), tableVal2(0x1a), tableVal2(0x1b),
		tableVal2(0x1c), tableVal2(0x1d), tableVal2(0x1e), tableVal2(0x1f),
		tableVal2(0x20), tableVal2(0x21), tableVal2(0x22), tableVal2(0x23),
		tableVal2(0x24), tableVal2(0x25), tableVal2(0x26), tableVal2(0x27),
		tableVal2(0x28), tableVal2(0x29), tableVal2(0x2a), tableVal2(0x2b),
		tableVal2(0x2c), tableVal2(0x2d), tableVal2(0x2e), tableVal2(0x2f),
		tableVal2(0x30), tableVal2(0x31), tableVal2(0x32), tableVal2(0x33),
		tableVal2(0x34), tableVal2(0x35), tableVal2(0x36), tableVal2(0x37),
		tableVal2(0x38), tableVal2(0x39), tableVal2(0x3a), tableVal2(0x3b),
		tableVal2(0x3c), tableVal2(0x3d), tableVal2(0x3e), tableVal2(0x3f),
		tableVal2(0x40), tableVal2(0x41), tableVal2(0x42), tableVal2(0x43),
		tableVal2(0x44), tableVal2(0x45), tableVal2(0x46), tableVal2(0x47),
		tableVal2(0x48), tableVal2(0x49), tableVal2(0x4a), tableVal2(0x4b),
		tableVal2(0x4c), tableVal2(0x4d), tableVal2(0x4e), tableVal2(0x4f),
		tableVal2(0x50), tableVal2(0x51), tableVal2(0x52), tableVal2(0x53),
		tableVal2(0x54), tableVal2(0x55), tableVal2(0x56), tableVal2(0x57),
		tableVal2(0x58), tableVal2(0x59), tableVal2(0x5a), tableVal2(0x5b),
		tableVal2(0x5c), tableVal2(0x5d), tableVal2(0x5e), tableVal2(0x5f),
		tableVal2(0x60), tableVal2(0x61), tableVal2(0x62), tableVal2(0x63),
		tableVal2(0x64), tableVal2(0x65), tableVal2(0x66), tableVal2(0x67),
		tableVal2(0x68), tableVal2(0x69), tableVal2(0x6a), tableVal2(0x6b),
		tableVal2(0x6c), tableVal2(0x6d), tableVal2(0x6e), tableVal2(0x6f),
		tableVal2(0x70), tableVal2(0x71), tableVal2(0x72), tableVal2(0x73),
		tableVal2(0x74), tableVal2(0x75), tableVal2(0x76), tableVal2(0x77),
		tableVal2(0x78), tableVal2(0x79), tableVal2(0x7a), tableVal2(0x7b),
		tableVal2(0x7c), tableVal2(0x7d), tableVal2(0x7e), tableVal2(0x7f),
		tableVal2(0x80), tableVal2(0x81), tableVal2(0x82), tableVal2(0x83),
		tableVal2(0x84), tableVal2(0x85), tableVal2(0x86), tableVal2(0x87),
		tableVal2(0x88), tableVal2(0x89), tableVal2(0x8a), tableVal2(0x8b),
		tableVal2(0x8c), tableVal2(0x8d), tableVal2(0x8e), tableVal2(0x8f),
		tableVal2(0x90), tableVal2(0x91), tableVal2(0x92), tableVal2(0x93),
		tableVal2(0x94), tableVal2(0x95), tableVal2(0x96), tableVal2(0x97),
		tableVal2(0x98), tableVal2(0x99), tableVal2(0x9a), tableVal2(0x9b),
		tableVal2(0x9c), tableVal2(0x9d), tableVal2(0x9e), tableVal2(0x9f),
		tableVal2(0xa0), tableVal2(0xa1), tableVal2(0xa2), tableVal2(0xa3),
		tableVal2(0xa4), tableVal2(0xa5), tableVal2(0xa6), tableVal2(0xa7),
		tableVal2(0xa8), tableVal2(0xa9), tableVal2(0xaa), tableVal2(0xab),
		tableVal2(0xac), tableVal2(0xad), tableVal2(0xae), tableVal2(0xaf),
		tableVal2(0xb0), tableVal2(0xb1), tableVal2(0xb2), tableVal2(0xb3),
		tableVal2(0xb4), tableVal2(0xb5), tableVal2(0xb6), tableVal2(0xb7),
		tableVal2(0xb8), tableVal2(0xb9), tableVal2(0xba), tableVal2(0xbb),
		tableVal2(0xbc), tableVal2(0xbd), tableVal2(0xbe), tableVal2(0xbf),
		tableVal2(0xc0), tableVal2(0xc1), tableVal2(0xc2), tableVal2(0xc3),
		tableVal2(0xc4), tableVal2(0xc5), tableVal2(0xc6), tableVal2(0xc7),
		tableVal2(0xc8), tableVal2(0xc9), tableVal2(0xca), tableVal2(0xcb),
		tableVal2(0xcc), tableVal2(0xcd), tableVal2(0xce), tableVal2(0xcf),
		tableVal2(0xd0), tableVal2(0xd1), tableVal2(0xd2), tableVal2(0xd3),
		tableVal2(0xd4), tableVal2(0xd5), tableVal2(0xd6), tableVal2(0xd7),
		tableVal2(0xd8), tableVal2(0xd9), tableVal2(0xda), tableVal2(0xdb),
		tableVal2(0xdc), tableVal2(0xdd), tableVal2(0xde), tableVal2(0xdf),
		tableVal2(0xe0), tableVal2(0xe1), tableVal2(0xe2), tableVal2(0xe3),
		tableVal2(0xe4), tableVal2(0xe5), tableVal2(0xe6), tableVal2(0xe7),
		tableVal2(0xe8), tableVal2(0xe9), tableVal2(0xea), tableVal2(0xeb),
		tableVal2(0xec), tableVal2(0xed), tableVal2(0xee), tableVal2(0xef),
		tableVal2(0xf0), tableVal2(0xf1), tableVal2(0xf2), tableVal2(0xf3),
		tableVal2(0xf4), tableVal2(0xf5), tableVal2(0xf6), tableVal2(0xf7),
		tableVal2(0xf8), tableVal2(0xf9), tableVal2(0xfa), tableVal2(0xfb),
		tableVal2(0xfc), tableVal2(0xfd), tableVal2(0xfe), tableVal2(0xff)
};

// CRC64 value for each 00 00 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64Reversed_impl::lookupTable3[0x100] = {
		tableVal3(0x00), tableVal3(0x01), tableVal3(0x02), tableVal3(0x03),
		tableVal3(0x04), tableVal3(0x05), tableVal3(0x06), tableVal3(0x07),
		tableVal3(0x08), tableVal3(0x09), tableVal3(0x0a), tableVal3(0x0b),
		tableVal3(0x0c), tableVal3(0x0d), tableVal3(0x0e), tableVal3(0x0f),
		tableVal3(0x10), tableVal3(0x11), tableVal3(0x12), tableVal3(0x13),
		tableVal3(0x14), tableVal3(0x15), tableVal3(0x16), tableVal3(0x17),
		tableVal3(0x18), tableVal3(0x19), tableVal3(0x1a), tableVal3(0x1b),
		tableVal3(0x1c), tableVal3(0x1d), tableVal3(0x1e), tableVal3(0x1f),
		tableVal3(0x20), tableVal3(0x21), tableVal3(0x22), tableVal3(0x23),
		tableVal3(0x24), tableVal3(0x25), tableVal3(0x26), tableVal3(0x27),
		tableVal3(0x28), tableVal3(0x29), tableVal3(0x2a), tableVal3(0x2b),
		tableVal3(0x2c), tableVal3(0x2d), tableVal3(0x2e), tableVal3(0x2f),
		tableVal3(0x30), tableVal3(0x31), tableVal3(0x32), tableVal3(0x33),
		tableVal3(0x34), tableVal3(0x35), tableVal3(0x36), tableVal3(0x37),
		tableVal3(0x38), tableVal3(0x39), tableVal3(0x3a), tableVal3(0x3b),
		tableVal3(0x3c), tableVal3(0x3d), tableVal3(0x3e), tableVal3(0x3f),
		tableVal3(0x40), tableVal3(0x41), tableVal3(0x42), tableVal3(0x43),
		tableVal3(0x44), tableVal3(0x45), tableVal3(0x46), tableVal3(0x47),
		tableVal3(0x48), tableVal3(0x49), tableVal3(0x4a), tableVal3(0x4b),
		tableVal3(0x4c), tableVal3(0x4d), tableVal3(0x4e), tableVal3(0x4f),
		tableVal3(0x50), tableVal3(0x51), tableVal3(0x52), tableVal3(0x53),
		tableVal3(0x54), tableVal3(0x55), tableVal3(0x56), tableVal3(0x57),
		tableVal3(0x58), tableVal3(0x59), tableVal3(0x5a), tableVal3(0x5b),
		tableVal3(0x5c), tableVal3(0x5d), tableVal3(0x5e), tableVal3(0x5f),
		tableVal3(0x60), tableVal3(0x61), tableVal3(0x62), tableVal3(0x63),
		tableVal3(0x64), tableVal3(0x65), tableVal3(0x66), tableVal3(0x67),
		tableVal3(0x68), tableVal3(0x69), tableVal3(0x6a), tableVal3(0x6b),
		tableVal3(0x6c), tableVal3(0x6d), tableVal3(0x6e), tableVal3(0x6f),
		tableVal3(0x70), tableVal3(0x71), tableVal3(0x72), tableVal3(0x73),
		tableVal3(0x74), tableVal3(0x75), tableVal3(0x76), tableVal3(0x77),
		tableVal3(0x78), tableVal3(0x79), tableVal3(0x7a), tableVal3(0x7b),
		tableVal3(0x7c), tableVal3(0x7d), tableVal3(0x7e), tableVal3(0x7f),
		tableVal3(0x80), tableVal3(0x81), tableVal3(0x82), tableVal3(0x83),
		tableVal3(0x84), tableVal3(0x85), tableVal3(0x86), tableVal3(0x87),
		tableVal3(0x88), tableVal3(0x89), tableVal3(0x8a), tableVal3(0x8b),
		tableVal3(0x8c), tableVal3(0x8d), tableVal3(0x8e), tableVal3(0x8f),
		tableVal3(0x90), tableVal3(0x91), tableVal3(0x92), tableVal3(0x93),
		tableVal3(0x94), tableVal3(0x95), tableVal3(0x96), tableVal3(0x97),
		tableVal3(0x98), tableVal3(0x99), tableVal3(0x9a), tableVal3(0x9b),
		tableVal3(0x9c), tableVal3(0x9d), tableVal3(0x9e), tableVal3(0x9f),
		tableVal3(0xa0), tableVal3(0xa1), tableVal3(0xa2), tableVal3(0xa3),
		tableVal3(0xa4), tableVal3(0xa5), tableVal3(0xa6), tableVal3(0xa7),
		tableVal3(0xa8), tableVal3(0xa9), tableVal3(0xaa), tableVal3(0xab),
		tableVal3(0xac), tableVal3(0xad), tableVal3(0xae), tableVal3(0xaf),
		tableVal3(0xb0), tableVal3(0xb1), tableVal3(0xb2), tableVal3(0xb3),
		tableVal3(0xb4), tableVal3(0xb5), tableVal3(0xb6), tableVal3(0xb7),
		tableVal3(0xb8), tableVal3(0xb9), tableVal3(0xba), tableVal3(0xbb),
		tableVal3(0xbc), tableVal3(0xbd), tableVal3(0xbe), tableVal3(0xbf),
		tableVal3(0xc0), tableVal3(0xc1), tableVal3(0xc2), tableVal3(0xc3),
		tableVal3(0xc4), tableVal3(0xc5), tableVal3(0xc6), tableVal3(0xc7),
		tableVal3(0xc8), tableVal3(0xc9), tableVal3(0xca), tableVal3(0xcb),
		tableVal3(0xcc), tableVal3(0xcd), tableVal3(0xce), tableVal3(0xcf),
		tableVal3(0xd0), tableVal3(0xd1), tableVal3(0xd2), tableVal3(0xd3),
		tableVal3(0xd4), tableVal3(0xd5), tableVal3(0xd6), tableVal3(0xd7),
		tableVal3(0xd8), tableVal3(0xd9), tableVal3(0xda), tableVal3(0xdb),
		tableVal3(0xdc), tableVal3(0xdd), tableVal3(0xde), tableVal3(0xdf),
		tableVal3(0xe0), tableVal3(0xe1), tableVal3(0xe2), tableVal3(0xe3),
		tableVal3(0xe4), tableVal3(0xe5), tableVal3(0xe6), tableVal3(0xe7),
		tableVal3(0xe8), tableVal3(0xe9), tableVal3(0xea), tableVal3(0xeb),
		tableVal3(0xec), tableVal3(0xed), tableVal3(0xee), tableVal3(0xef),
		tableVal3(0xf0), tableVal3(0xf1), tableVal3(0xf2), tableVal3(0xf3),
		tableVal3(0xf4), tableVal3(0xf5), tableVal3(0xf6), tableVal3(0xf7),
		tableVal3(0xf8), tableVal3(0xf9), tableVal3(0xfa), tableVal3(0xfb),
		tableVal3(0xfc), tableVal3(0xfd), tableVal3(0xfe), tableVal3(0xff)
};

// CRC64 value for each 00 00 00 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64Reversed_impl::lookupTable4[0x100] = {
		tableVal4(0x00), tableVal4(0x01), tableVal4(0x02), tableVal4(0x03),
		tableVal4(0x04), tableVal4(0x05), tableVal4(0x06), tableVal4(0x07),
		tableVal4(0x08), tableVal4(0x09), tableVal4(0x0a), tableVal4(0x0b),
		tableVal4(0x0c), tableVal4(0x0d), tableVal4(0x0e), tableVal4(0x0f),
		tableVal4(0x10), tableVal4(0x11), tableVal4(0x12), tableVal4(0x13),
		tableVal4(0x14), tableVal4(0x15), tableVal4(0x16), tableVal4(0x17),
		tableVal4(0x18), tableVal4(0x19), tableVal4(0x1a), tableVal4(0x1b),
		tableVal4(0x1c), tableVal4(0x1d), tableVal4(0x1e), tableVal4(0x1f),
		tableVal4(0x20), tableVal4(0x21), tableVal4(0x22), tableVal4(0x23),
		tableVal4(0x24), tableVal4(0x25), tableVal4(0x26), tableVal4(0x27),
		tableVal4(0x28), tableVal4(0x29), tableVal4(0x2a), tableVal4(0x2b),
		tableVal4(0x2c), tableVal4(0x2d), tableVal4(0x2e), tableVal4(0x2f),
		tableVal4(0x30), tableVal4(0x31), tableVal4(0x32), tableVal4(0x33),
		tableVal4(0x34), tableVal4(0x35), tableVal4(0x36), tableVal4(0x37),
		tableVal4(0x38), tableVal4(0x39), tableVal4(0x3a), tableVal4(0x3b),
		tableVal4(0x3c), tableVal4(0x3d), tableVal4(0x3e), tableVal4(0x3f),
		tableVal4(0x40), tableVal4(0x41), tableVal4(0x42), tableVal4(0x43),
		tableVal4(0x44), tableVal4(0x45), tableVal4(0x46), tableVal4(0x47),
		tableVal4(0x48), tableVal4(0x49), tableVal4(0x4a), tableVal4(0x4b),
		tableVal4(0x4c), tableVal4(0x4d), tableVal4(0x4e), tableVal4(0x4f),
		tableVal4(0x50), tableVal4(0x51), tableVal4(0x52), tableVal4(0x53),
		tableVal4(0x54), tableVal4(0x55), tableVal4(0x56), tableVal4(0x57),
		tableVal4(0x58), tableVal4(0x59), tableVal4(0x5a), tableVal4(0x5b),
		tableVal4(0x5c), tableVal4(0x5d), tableVal4(0x5e), tableVal4(0x5f),
		tableVal4(0x60), tableVal4(0x61), tableVal4(0x62), tableVal4(0x63),
		tableVal4(0x64), tableVal4(0x65), tableVal4(0x66), tableVal4(0x67),
		tableVal4(0x68), tableVal4(0x69), tableVal4(0x6a), tableVal4(0x6b),
		tableVal4(0x6c), tableVal4(0x6d), tableVal4(0x6e), tableVal4(0x6f),
		tableVal4(0x70), tableVal4(0x71), tableVal4(0x72), tableVal4(0x73),
		tableVal4(0x74), tableVal4(0x75), tableVal4(0x76), tableVal4(0x77),
		tableVal4(0x78), tableVal4(0x79), tableVal4(0x7a), tableVal4(0x7b),
		tableVal4(0x7c), tableVal4(0x7d), tableVal4(0x7e), tableVal4(0x7f),
		tableVal4(0x80), tableVal4(0x81), tableVal4(0x82), tableVal4(0x83),
		tableVal4(0x84), tableVal4(0x85), tableVal4(0x86), tableVal4(0x87),
		tableVal4(0x88), tableVal4(0x89), tableVal4(0x8a), tableVal4(0x8b),
		tableVal4(0x8c), tableVal4(0x8d), tableVal4(0x8e), tableVal4(0x8f),
		tableVal4(0x90), tableVal4(0x91), tableVal4(0x92), tableVal4(0x93),
		tableVal4(0x94), tableVal4(0x95), tableVal4(0x96), tableVal4(0x97),
		tableVal4(0x98), tableVal4(0x99), tableVal4(0x9a), tableVal4(0x9b),
		tableVal4(0x9c), tableVal4(0x9d), tableVal4(0x9e), tableVal4(0x9f),
		tableVal4(0xa0), tableVal4(0xa1), tableVal4(0xa2), tableVal4(0xa3),
		tableVal4(0xa4), tableVal4(0xa5), tableVal4(0xa6), tableVal4(0xa7),
		tableVal4(0xa8), tableVal4(0xa9), tableVal4(0xaa), tableVal4(0xab),
		tableVal4(0xac), tableVal4(0xad), tableVal4(0xae), tableVal4(0xaf),
		tableVal4(0xb0), tableVal4(0xb1), tableVal4(0xb2), tableVal4(0xb3),
		tableVal4(0xb4), tableVal4(0xb5), tableVal4(0xb6), tableVal4(0xb7),
		tableVal4(0xb8), tableVal4(0xb9), tableVal4(0xba), tableVal4(0xbb),
		tableVal4(0xbc), tableVal4(0xbd), tableVal4(0xbe), tableVal4(0xbf),
		tableVal4(0xc0), tableVal4(0xc1), tableVal4(0xc2), tableVal4(0xc3),
		tableVal4(0xc4), tableVal4(0xc5), tableVal4(0xc6), tableVal4(0xc7),
		tableVal4(0xc8), tableVal4(0xc9), tableVal4(0xca), tableVal4(0xcb),
		tableVal4(0xcc), tableVal4(0xcd), tableVal4(0xce), tableVal4(0xcf),
		tableVal4(0xd0), tableVal4(0xd1), tableVal4(0xd2), tableVal4(0xd3),
		tableVal4(0xd4), tableVal4(0xd5), tableVal4(0xd6), tableVal4(0xd7),
		tableVal4(0xd8), tableVal4(0xd9), tableVal4(0xda), tableVal4(0xdb),
		tableVal4(0xdc), tableVal4(0xdd), tableVal4(0xde), tableVal4(0xdf),
		tableVal4(0xe0), tableVal4(0xe1), tableVal4(0xe2), tableVal4(0xe3),
		tableVal4(0xe4), tableVal4(0xe5), tableVal4(0xe6), tableVal4(0xe7),
		tableVal4(0xe8), tableVal4(0xe9), tableVal4(0xea), tableVal4(0xeb),
		tableVal4(0xec), tableVal4(0xed), tableVal4(0xee), tableVal4(0xef),
		tableVal4(0xf0), tableVal4(0xf1), tableVal4(0xf2), tableVal4(0xf3),
		tableVal4(0xf4), tableVal4(0xf5), tableVal4(0xf6), tableVal4(0xf7),
		tableVal4(0xf8), tableVal4(0xf9), tableVal4(0xfa), tableVal4(0xfb),
		tableVal4(0xfc), tableVal4(0xfd), tableVal4(0xfe), tableVal4(0xff)
};

// CRC64 value for each 00 00 00 00 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64Reversed_impl::lookupTable5[0x100] = {
		tableVal5(0x00), tableVal5(0x01), tableVal5(0x02), tableVal5(0x03),
		tableVal5(0x04), tableVal5(0x05), tableVal5(0x06), tableVal5(0x07),
		tableVal5(0x08), tableVal5(0x09), tableVal5(0x0a), tableVal5(0x0b),
		tableVal5(0x0c), tableVal5(0x0d), tableVal5(0x0e), tableVal5(0x0f),
		tableVal5(0x10), tableVal5(0x11), tableVal5(0x12), tableVal5(0x13),
		tableVal5(0x14), tableVal5(0x15), tableVal5(0x16), tableVal5(0x17),
		tableVal5(0x18), tableVal5(0x19), tableVal5(0x1a), tableVal5(0x1b),
		tableVal5(0x1c), tableVal5(0x1d), tableVal5(0x1e), tableVal5(0x1f),
		tableVal5(0x20), tableVal5(0x21), tableVal5(0x22), tableVal5(0x23),
		tableVal5(0x24), tableVal5(0x25), tableVal5(0x26), tableVal5(0x27),
		tableVal5(0x28), tableVal5(0x29), tableVal5(0x2a), tableVal5(0x2b),
		tableVal5(0x2c), tableVal5(0x2d), tableVal5(0x2e), tableVal5(0x2f),
		tableVal5(0x30), tableVal5(0x31), tableVal5(0x32), tableVal5(0x33),
		tableVal5(0x34), tableVal5(0x35), tableVal5(0x36), tableVal5(0x37),
		tableVal5(0x38), tableVal5(0x39), tableVal5(0x3a), tableVal5(0x3b),
		tableVal5(0x3c), tableVal5(0x3d), tableVal5(0x3e), tableVal5(0x3f),
		tableVal5(0x40), tableVal5(0x41), tableVal5(0x42), tableVal5(0x43),
		tableVal5(0x44), tableVal5(0x45), tableVal5(0x46), tableVal5(0x47),
		tableVal5(0x48), tableVal5(0x49), tableVal5(0x4a), tableVal5(0x4b),
		tableVal5(0x4c), tableVal5(0x4d), tableVal5(0x4e), tableVal5(0x4f),
		tableVal5(0x50), tableVal5(0x51), tableVal5(0x52), tableVal5(0x53),
		tableVal5(0x54), tableVal5(0x55), tableVal5(0x56), tableVal5(0x57),
		tableVal5(0x58), tableVal5(0x59), tableVal5(0x5a), tableVal5(0x5b),
		tableVal5(0x5c), tableVal5(0x5d), tableVal5(0x5e), tableVal5(0x5f),
		tableVal5(0x60), tableVal5(0x61), tableVal5(0x62), tableVal5(0x63),
		tableVal5(0x64), tableVal5(0x65), tableVal5(0x66), tableVal5(0x67),
		tableVal5(0x68), tableVal5(0x69), tableVal5(0x6a), tableVal5(0x6b),
		tableVal5(0x6c), tableVal5(0x6d), tableVal5(0x6e), tableVal5(0x6f),
		tableVal5(0x70), tableVal5(0x71), tableVal5(0x72), tableVal5(0x73),
		tableVal5(0x74), tableVal5(0x75), tableVal5(0x76), tableVal5(0x77),
		tableVal5(0x78), tableVal5(0x79), tableVal5(0x7a), tableVal5(0x7b),
		tableVal5(0x7c), tableVal5(0x7d), tableVal5(0x7e), tableVal5(0x7f),
		tableVal5(0x80), tableVal5(0x81), tableVal5(0x82), tableVal5(0x83),
		tableVal5(0x84), tableVal5(0x85), tableVal5(0x86), tableVal5(0x87),
		tableVal5(0x88), tableVal5(0x89), tableVal5(0x8a), tableVal5(0x8b),
		tableVal5(0x8c), tableVal5(0x8d), tableVal5(0x8e), tableVal5(0x8f),
		tableVal5(0x90), tableVal5(0x91), tableVal5(0x92), tableVal5(0x93),
		tableVal5(0x94), tableVal5(0x95), tableVal5(0x96), tableVal5(0x97),
		tableVal5(0x98), tableVal5(0x99), tableVal5(0x9a), tableVal5(0x9b),
		tableVal5(0x9c), tableVal5(0x9d), tableVal5(0x9e), tableVal5(0x9f),
		tableVal5(0xa0), tableVal5(0xa1), tableVal5(0xa2), tableVal5(0xa3),
		tableVal5(0xa4), tableVal5(0xa5), tableVal5(0xa6), tableVal5(0xa7),
		tableVal5(0xa8), tableVal5(0xa9), tableVal5(0xaa), tableVal5(0xab),
		tableVal5(0xac), tableVal5(0xad), tableVal5(0xae), tableVal5(0xaf),
		tableVal5(0xb0), tableVal5(0xb1), tableVal5(0xb2), tableVal5(0xb3),
		tableVal5(0xb4), tableVal5(0xb5), tableVal5(0xb6), tableVal5(0xb7),
		tableVal5(0xb8), tableVal5(0xb9), tableVal5(0xba), tableVal5(0xbb),
		tableVal5(0xbc), tableVal5(0xbd), tableVal5(0xbe), tableVal5(0xbf),
		tableVal5(0xc0), tableVal5(0xc1), tableVal5(0xc2), tableVal5(0xc3),
		tableVal5(0xc4), tableVal5(0xc5), tableVal5(0xc6), tableVal5(0xc7),
		tableVal5(0xc8), tableVal5(0xc9), tableVal5(0xca), tableVal5(0xcb),
		tableVal5(0xcc), tableVal5(0xcd), tableVal5(0xce), tableVal5(0xcf),
		tableVal5(0xd0), tableVal5(0xd1), tableVal5(0xd2), tableVal5(0xd3),
		tableVal5(0xd4), tableVal5(0xd5), tableVal5(0xd6), tableVal5(0xd7),
		tableVal5(0xd8), tableVal5(0xd9), tableVal5(0xda), tableVal5(0xdb),
		tableVal5(0xdc), tableVal5(0xdd), tableVal5(0xde), tableVal5(0xdf),
		tableVal5(0xe0), tableVal5(0xe1), tableVal5(0xe2), tableVal5(0xe3),
		tableVal5(0xe4), tableVal5(0xe5), tableVal5(0xe6), tableVal5(0xe7),
		tableVal5(0xe8), tableVal5(0xe9), tableVal5(0xea), tableVal5(0xeb),
		tableVal5(0xec), tableVal5(0xed), tableVal5(0xee), tableVal5(0xef),
		tableVal5(0xf0), tableVal5(0xf1), tableVal5(0xf2), tableVal5(0xf3),
		tableVal5(0xf4), tableVal5(0xf5), tableVal5(0xf6), tableVal5(0xf7),
		tableVal5(0xf8), tableVal5(0xf9), tableVal5(0xfa), tableVal5(0xfb),
		tableVal5(0xfc), tableVal5(0xfd), tableVal5(0xfe), tableVal5(0xff)
};

// CRC64 value for each 00 00 00 00 00 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64Reversed_impl::lookupTable6[0x100] = {
		tableVal6(0x00), tableVal6(0x01), tableVal6(0x02), tableVal6(0x03),
		tableVal6(0x04), tableVal6(0x05), tableVal6(0x06), tableVal6(0x07),
		tableVal6(0x08), tableVal6(0x09), tableVal6(0x0a), tableVal6(0x0b),
		tableVal6(0x0c), tableVal6(0x0d), tableVal6(0x0e), tableVal6(0x0f),
		tableVal6(0x10), tableVal6(0x11), tableVal6(0x12), tableVal6(0x13),
		tableVal6(0x14), tableVal6(0x15), tableVal6(0x16), tableVal6(0x17),
		tableVal6(0x18), tableVal6(0x19), tableVal6(0x1a), tableVal6(0x1b),
		tableVal6(0x1c), tableVal6(0x1d), tableVal6(0x1e), tableVal6(0x1f),
		tableVal6(0x20), tableVal6(0x21), tableVal6(0x22), tableVal6(0x23),
		tableVal6(0x24), tableVal6(0x25), tableVal6(0x26), tableVal6(0x27),
		tableVal6(0x28), tableVal6(0x29), tableVal6(0x2a), tableVal6(0x2b),
		tableVal6(0x2c), tableVal6(0x2d), tableVal6(0x2e), tableVal6(0x2f),
		tableVal6(0x30), tableVal6(0x31), tableVal6(0x32), tableVal6(0x33),
		tableVal6(0x34), tableVal6(0x35), tableVal6(0x36), tableVal6(0x37),
		tableVal6(0x38), tableVal6(0x39), tableVal6(0x3a), tableVal6(0x3b),
		tableVal6(0x3c), tableVal6(0x3d), tableVal6(0x3e), tableVal6(0x3f),
		tableVal6(0x40), tableVal6(0x41), tableVal6(0x42), tableVal6(0x43),
		tableVal6(0x44), tableVal6(0x45), tableVal6(0x46), tableVal6(0x47),
		tableVal6(0x48), tableVal6(0x49), tableVal6(0x4a), tableVal6(0x4b),
		tableVal6(0x4c), tableVal6(0x4d), tableVal6(0x4e), tableVal6(0x4f),
		tableVal6(0x50), tableVal6(0x51), tableVal6(0x52), tableVal6(0x53),
		tableVal6(0x54), tableVal6(0x55), tableVal6(0x56), tableVal6(0x57),
		tableVal6(0x58), tableVal6(0x59), tableVal6(0x5a), tableVal6(0x5b),
		tableVal6(0x5c), tableVal6(0x5d), tableVal6(0x5e), tableVal6(0x5f),
		tableVal6(0x60), tableVal6(0x61), tableVal6(0x62), tableVal6(0x63),
		tableVal6(0x64), tableVal6(0x65), tableVal6(0x66), tableVal6(0x67),
		tableVal6(0x68), tableVal6(0x69), tableVal6(0x6a), tableVal6(0x6b),
		tableVal6(0x6c), tableVal6(0x6d), tableVal6(0x6e), tableVal6(0x6f),
		tableVal6(0x70), tableVal6(0x71), tableVal6(0x72), tableVal6(0x73),
		tableVal6(0x74), tableVal6(0x75), tableVal6(0x76), tableVal6(0x77),
		tableVal6(0x78), tableVal6(0x79), tableVal6(0x7a), tableVal6(0x7b),
		tableVal6(0x7c), tableVal6(0x7d), tableVal6(0x7e), tableVal6(0x7f),
		tableVal6(0x80), tableVal6(0x81), tableVal6(0x82), tableVal6(0x83),
		tableVal6(0x84), tableVal6(0x85), tableVal6(0x86), tableVal6(0x87),
		tableVal6(0x88), tableVal6(0x89), tableVal6(0x8a), tableVal6(0x8b),
		tableVal6(0x8c), tableVal6(0x8d), tableVal6(0x8e), tableVal6(0x8f),
		tableVal6(0x90), tableVal6(0x91), tableVal6(0x92), tableVal6(0x93),
		tableVal6(0x94), tableVal6(0x95), tableVal6(0x96), tableVal6(0x97),
		tableVal6(0x98), tableVal6(0x99), tableVal6(0x9a), tableVal6(0x9b),
		tableVal6(0x9c), tableVal6(0x9d), tableVal6(0x9e), tableVal6(0x9f),
		tableVal6(0xa0), tableVal6(0xa1), tableVal6(0xa2), tableVal6(0xa3),
		tableVal6(0xa4), tableVal6(0xa5), tableVal6(0xa6), tableVal6(0xa7),
		tableVal6(0xa8), tableVal6(0xa9), tableVal6(0xaa), tableVal6(0xab),
		tableVal6(0xac), tableVal6(0xad), tableVal6(0xae), tableVal6(0xaf),
		tableVal6(0xb0), tableVal6(0xb1), tableVal6(0xb2), tableVal6(0xb3),
		tableVal6(0xb4), tableVal6(0xb5), tableVal6(0xb6), tableVal6(0xb7),
		tableVal6(0xb8), tableVal6(0xb9), tableVal6(0xba), tableVal6(0xbb),
		tableVal6(0xbc), tableVal6(0xbd), tableVal6(0xbe), tableVal6(0xbf),
		tableVal6(0xc0), tableVal6(0xc1), tableVal6(0xc2), tableVal6(0xc3),
		tableVal6(0xc4), tableVal6(0xc5), tableVal6(0xc6), tableVal6(0xc7),
		tableVal6(0xc8), tableVal6(0xc9), tableVal6(0xca), tableVal6(0xcb),
		tableVal6(0xcc), tableVal6(0xcd), tableVal6(0xce), tableVal6(0xcf),
		tableVal6(0xd0), tableVal6(0xd1), tableVal6(0xd2), tableVal6(0xd3),
		tableVal6(0xd4), tableVal6(0xd5), tableVal6(0xd6), tableVal6(0xd7),
		tableVal6(0xd8), tableVal6(0xd9), tableVal6(0xda), tableVal6(0xdb),
		tableVal6(0xdc), tableVal6(0xdd), tableVal6(0xde), tableVal6(0xdf),
		tableVal6(0xe0), tableVal6(0xe1), tableVal6(0xe2), tableVal6(0xe3),
		tableVal6(0xe4), tableVal6(0xe5), tableVal6(0xe6), tableVal6(0xe7),
		tableVal6(0xe8), tableVal6(0xe9), tableVal6(0xea), tableVal6(0xeb),
		tableVal6(0xec), tableVal6(0xed), tableVal6(0xee), tableVal6(0xef),
		tableVal6(0xf0), tableVal6(0xf1), tableVal6(0xf2), tableVal6(0xf3),
		tableVal6(0xf4), tableVal6(0xf5), tableVal6(0xf6), tableVal6(0xf7),
		tableVal6(0xf8), tableVal6(0xf9), tableVal6(0xfa), tableVal6(0xfb),
		tableVal6(0xfc), tableVal6(0xfd), tableVal6(0xfe), tableVal6(0xff)
};

// CRC64 value for each 00 00 00 00 00 00 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64Reversed_impl::lookupTable7[0x100] = {
		tableVal7(0x00), tableVal7(0x01), tableVal7(0x02), tableVal7(0x03),
		tableVal7(0x04), tableVal7(0x05), tableVal7(0x06), tableVal7(0x07),
		tableVal7(0x08), tableVal7(0x09), tableVal7(0x0a), tableVal7(0x0b),
		tableVal7(0x0c), tableVal7(0x0d), tableVal7(0x0e), tableVal7(0x0f),
		tableVal7(0x10), tableVal7(0x11), tableVal7(0x12), tableVal7(0x13),
		tableVal7(0x14), tableVal7(0x15), tableVal7(0x16), tableVal7(0x17),
		tableVal7(0x18), tableVal7(0x19), tableVal7(0x1a), tableVal7(0x1b),
		tableVal7(0x1c), tableVal7(0x1d), tableVal7(0x1e), tableVal7(0x1f),
		tableVal7(0x20), tableVal7(0x21), tableVal7(0x22), tableVal7(0x23),
		tableVal7(0x24), tableVal7(0x25), tableVal7(0x26), tableVal7(0x27),
		tableVal7(0x28), tableVal7(0x29), tableVal7(0x2a), tableVal7(0x2b),
		tableVal7(0x2c), tableVal7(0x2d), tableVal7(0x2e), tableVal7(0x2f),
		tableVal7(0x30), tableVal7(0x31), tableVal7(0x32), tableVal7(0x33),
		tableVal7(0x34), tableVal7(0x35), tableVal7(0x36), tableVal7(0x37),
		tableVal7(0x38), tableVal7(0x39), tableVal7(0x3a), tableVal7(0x3b),
		tableVal7(0x3c), tableVal7(0x3d), tableVal7(0x3e), tableVal7(0x3f),
		tableVal7(0x40), tableVal7(0x41), tableVal7(0x42), tableVal7(0x43),
		tableVal7(0x44), tableVal7(0x45), tableVal7(0x46), tableVal7(0x47),
		tableVal7(0x48), tableVal7(0x49), tableVal7(0x4a), tableVal7(0x4b),
		tableVal7(0x4c), tableVal7(0x4d), tableVal7(0x4e), tableVal7(0x4f),
		tableVal7(0x50), tableVal7(0x51), tableVal7(0x52), tableVal7(0x53),
		tableVal7(0x54), tableVal7(0x55), tableVal7(0x56), tableVal7(0x57),
		tableVal7(0x58), tableVal7(0x59), tableVal7(0x5a), tableVal7(0x5b),
		tableVal7(0x5c), tableVal7(0x5d), tableVal7(0x5e), tableVal7(0x5f),
		tableVal7(0x60), tableVal7(0x61), tableVal7(0x62), tableVal7(0x63),
		tableVal7(0x64), tableVal7(0x65), tableVal7(0x66), tableVal7(0x67),
		tableVal7(0x68), tableVal7(0x69), tableVal7(0x6a), tableVal7(0x6b),
		tableVal7(0x6c), tableVal7(0x6d), tableVal7(0x6e), tableVal7(0x6f),
		tableVal7(0x70), tableVal7(0x71), tableVal7(0x72), tableVal7(0x73),
		tableVal7(0x74), tableVal7(0x75), tableVal7(0x76), tableVal7(0x77),
		tableVal7(0x78), tableVal7(0x79), tableVal7(0x7a), tableVal7(0x7b),
		tableVal7(0x7c), tableVal7(0x7d), tableVal7(0x7e), tableVal7(0x7f),
		tableVal7(0x80), tableVal7(0x81), tableVal7(0x82), tableVal7(0x83),
		tableVal7(0x84), tableVal7(0x85), tableVal7(0x86), tableVal7(0x87),
		tableVal7(0x88), tableVal7(0x89), tableVal7(0x8a), tableVal7(0x8b),
		tableVal7(0x8c), tableVal7(0x8d), tableVal7(0x8e), tableVal7(0x8f),
		tableVal7(0x90), tableVal7(0x91), tableVal7(0x92), tableVal7(0x93),
		tableVal7(0x94), tableVal7(0x95), tableVal7(0x96), tableVal7(0x97),
		tableVal7(0x98), tableVal7(0x99), tableVal7(0x9a), tableVal7(0x9b),
		tableVal7(0x9c), tableVal7(0x9d), tableVal7(0x9e), tableVal7(0x9f),
		tableVal7(0xa0), tableVal7(0xa1), tableVal7(0xa2), tableVal7(0xa3),
		tableVal7(0xa4), tableVal7(0xa5), tableVal7(0xa6), tableVal7(0xa7),
		tableVal7(0xa8), tableVal7(0xa9), tableVal7(0xaa), tableVal7(0xab),
		tableVal7(0xac), tableVal7(0xad), tableVal7(0xae), tableVal7(0xaf),
		tableVal7(0xb0), tableVal7(0xb1), tableVal7(0xb2), tableVal7(0xb3),
		tableVal7(0xb4), tableVal7(0xb5), tableVal7(0xb6), tableVal7(0xb7),
		tableVal7(0xb8), tableVal7(0xb9), tableVal7(0xba), tableVal7(0xbb),
		tableVal7(0xbc), tableVal7(0xbd), tableVal7(0xbe), tableVal7(0xbf),
		tableVal7(0xc0), tableVal7(0xc1), tableVal7(0xc2), tableVal7(0xc3),
		tableVal7(0xc4), tableVal7(0xc5), tableVal7(0xc6), tableVal7(0xc7),
		tableVal7(0xc8), tableVal7(0xc9), tableVal7(0xca), tableVal7(0xcb),
		tableVal7(0xcc), tableVal7(0xcd), tableVal7(0xce), tableVal7(0xcf),
		tableVal7(0xd0), tableVal7(0xd1), tableVal7(0xd2), tableVal7(0xd3),
		tableVal7(0xd4), tableVal7(0xd5), tableVal7(0xd6), tableVal7(0xd7),
		tableVal7(0xd8), tableVal7(0xd9), tableVal7(0xda), tableVal7(0xdb),
		tableVal7(0xdc), tableVal7(0xdd), tableVal7(0xde), tableVal7(0xdf),
		tableVal7(0xe0), tableVal7(0xe1), tableVal7(0xe2), tableVal7(0xe3),
		tableVal7(0xe4), tableVal7(0xe5), tableVal7(0xe6), tableVal7(0xe7),
		tableVal7(0xe8), tableVal7(0xe9), tableVal7(0xea), tableVal7(0xeb),
		tableVal7(0xec), tableVal7(0xed), tableVal7(0xee), tableVal7(0xef),
		tableVal7(0xf0), tableVal7(0xf1), tableVal7(0xf2), tableVal7(0xf3),
		tableVal7(0xf4), tableVal7(0xf5), tableVal7(0xf6), tableVal7(0xf7),
		tableVal7(0xf8), tableVal7(0xf9), tableVal7(0xfa), tableVal7(0xfb),
		tableVal7(0xfc), tableVal7(0xfd), tableVal7(0xfe), tableVal7(0xff)
};

// CRC64 value for each 00 00 00 00 00 00 00 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64Reversed_impl::lookupTable8[0x100] = {
		tableVal8(0x00), tableVal8(0x01), tableVal8(0x02), tableVal8(0x03),
		tableVal8(0x04), tableVal8(0x05), tableVal8(0x06), tableVal8(0x07),
		tableVal8(0x08), tableVal8(0x09), tableVal8(0x0a), tableVal8(0x0b),
		tableVal8(0x0c), tableVal8(0x0d), tableVal8(0x0e), tableVal8(0x0f),
		tableVal8(0x10), tableVal8(0x11), tableVal8(0x12), tableVal8(0x13),
		tableVal8(0x14), tableVal8(0x15), tableVal8(0x16), tableVal8(0x17),
		tableVal8(0x18), tableVal8(0x19), tableVal8(0x1a), tableVal8(0x1b),
		tableVal8(0x1c), tableVal8(0x1d), tableVal8(0x1e), tableVal8(0x1f),
		tableVal8(0x20), tableVal8(0x21), tableVal8(0x22), tableVal8(0x23),
		tableVal8(0x24), tableVal8(0x25), tableVal8(0x26), tableVal8(0x27),
		tableVal8(0x28), tableVal8(0x29), tableVal8(0x2a), tableVal8(0x2b),
		tableVal8(0x2c), tableVal8(0x2d), tableVal8(0x2e), tableVal8(0x2f),
		tableVal8(0x30), tableVal8(0x31), tableVal8(0x32), tableVal8(0x33),
		tableVal8(0x34), tableVal8(0x35), tableVal8(0x36), tableVal8(0x37),
		tableVal8(0x38), tableVal8(0x39), tableVal8(0x3a), tableVal8(0x3b),
		tableVal8(0x3c), tableVal8(0x3d), tableVal8(0x3e), tableVal8(0x3f),
		tableVal8(0x40), tableVal8(0x41), tableVal8(0x42), tableVal8(0x43),
		tableVal8(0x44), tableVal8(0x45), tableVal8(0x46), tableVal8(0x47),
		tableVal8(0x48), tableVal8(0x49), tableVal8(0x4a), tableVal8(0x4b),
		tableVal8(0x4c), tableVal8(0x4d), tableVal8(0x4e), tableVal8(0x4f),
		tableVal8(0x50), tableVal8(0x51), tableVal8(0x52), tableVal8(0x53),
		tableVal8(0x54), tableVal8(0x55), tableVal8(0x56), tableVal8(0x57),
		tableVal8(0x58), tableVal8(0x59), tableVal8(0x5a), tableVal8(0x5b),
		tableVal8(0x5c), tableVal8(0x5d), tableVal8(0x5e), tableVal8(0x5f),
		tableVal8(0x60), tableVal8(0x61), tableVal8(0x62), tableVal8(0x63),
		tableVal8(0x64), tableVal8(0x65), tableVal8(0x66), tableVal8(0x67),
		tableVal8(0x68), tableVal8(0x69), tableVal8(0x6a), tableVal8(0x6b),
		tableVal8(0x6c), tableVal8(0x6d), tableVal8(0x6e), tableVal8(0x6f),
		tableVal8(0x70), tableVal8(0x71), tableVal8(0x72), tableVal8(0x73),
		tableVal8(0x74), tableVal8(0x75), tableVal8(0x76), tableVal8(0x77),
		tableVal8(0x78), tableVal8(0x79), tableVal8(0x7a), tableVal8(0x7b),
		tableVal8(0x7c), tableVal8(0x7d), tableVal8(0x7e), tableVal8(0x7f),
		tableVal8(0x80), tableVal8(0x81), tableVal8(0x82), tableVal8(0x83),
		tableVal8(0x84), tableVal8(0x85), tableVal8(0x86), tableVal8(0x87),
		tableVal8(0x88), tableVal8(0x89), tableVal8(0x8a), tableVal8(0x8b),
		tableVal8(0x8c), tableVal8(0x8d), tableVal8(0x8e), tableVal8(0x8f),
		tableVal8(0x90), tableVal8(0x91), tableVal8(0x92), tableVal8(0x93),
		tableVal8(0x94), tableVal8(0x95), tableVal8(0x96), tableVal8(0x97),
		tableVal8(0x98), tableVal8(0x99), tableVal8(0x9a), tableVal8(0x9b),
		tableVal8(0x9c), tableVal8(0x9d), tableVal8(0x9e), tableVal8(0x9f),
		tableVal8(0xa0), tableVal8(0xa1), tableVal8(0xa2), tableVal8(0xa3),
		tableVal8(0xa4), tableVal8(0xa5), tableVal8(0xa6), tableVal8(0xa7),
		tableVal8(0xa8), tableVal8(0xa9), tableVal8(0xaa), tableVal8(0xab),
		tableVal8(0xac), tableVal8(0xad), tableVal8(0xae), tableVal8(0xaf),
		tableVal8(0xb0), tableVal8(0xb1), tableVal8(0xb2), tableVal8(0xb3),
		tableVal8(0xb4), tableVal8(0xb5), tableVal8(0xb6), tableVal8(0xb7),
		tableVal8(0xb8), tableVal8(0xb9), tableVal8(0xba), tableVal8(0xbb),
		tableVal8(0xbc), tableVal8(0xbd), tableVal8(0xbe), tableVal8(0xbf),
		tableVal8(0xc0), tableVal8(0xc1), tableVal8(0xc2), tableVal8(0xc3),
		tableVal8(0xc4), tableVal8(0xc5), tableVal8(0xc6), tableVal8(0xc7),
		tableVal8(0xc8), tableVal8(0xc9), tableVal8(0xca), tableVal8(0xcb),
		tableVal8(0xcc), tableVal8(0xcd), tableVal8(0xce), tableVal8(0xcf),
		tableVal8(0xd0), tableVal8(0xd1), tableVal8(0xd2), tableVal8(0xd3),
		tableVal8(0xd4), tableVal8(0xd5), tableVal8(0xd6), tableVal8(0xd7),
		tableVal8(0xd8), tableVal8(0xd9), tableVal8(0xda), tableVal8(0xdb),
		tableVal8(0xdc), tableVal8(0xdd), tableVal8(0xde), tableVal8(0xdf),
		tableVal8(0xe0), tableVal8(0xe1), tableVal8(0xe2), tableVal8(0xe3),
		tableVal8(0xe4), tableVal8(0xe5), tableVal8(0xe6), tableVal8(0xe7),
		tableVal8(0xe8), tableVal8(0xe9), tableVal8(0xea), tableVal8(0xeb),
		tableVal8(0xec), tableVal8(0xed), tableVal8(0xee), tableVal8(0xef),
		tableVal8(0xf0), tableVal8(0xf1), tableVal8(0xf2), tableVal8(0xf3),
		tableVal8(0xf4), tableVal8(0xf5), tableVal8(0xf6), tableVal8(0xf7),
		tableVal8(0xf8), tableVal8(0xf9), tableVal8(0xfa), tableVal8(0xfb),
		tableVal8(0xfc), tableVal8(0xfd), tableVal8(0xfe), tableVal8(0xff)
};

std::uint_fast64_t afc::crc64ReversedUpdate(const std::uint_fast64_t currentCrc,
		const unsigned char * const data, const std::size_t n)
{
	static_assert(alignof(std::max_align_t) >= 8, "alignment by 8 is unsupported");
	assert(currentCrc == (currentCrc & 0xffffffffffffffff));

	std::uint_fast64_t crc = currentCrc;

	if (afc::PLATFORM_BYTE_ORDER == afc::endianness::LE) {
		void *alignedData = const_cast<void *>(reinterpret_cast<const void *>(data));
		std::size_t remainingN = n;
		if (std::align(8, 1, alignedData, remainingN)) {
			std::size_t slowPreN = n - remainingN;
			// Processing non-aligned data.
			for (std::size_t i = 0; i < slowPreN; ++i) {
				crc = (crc >> 8) ^ afc::crc64Reversed_impl::lookupTable[(data[i] ^ crc) & 0xff];
			}

			return crc64ReversedUpdate_Aligned8Impl_Inline(
					crc, reinterpret_cast<const unsigned char *>(alignedData), remainingN);
		} else {
			// There is no aligned data. Going the slow way.
			for (std::size_t i = 0; i < n; ++i) {
				crc = (crc >> 8) ^ afc::crc64Reversed_impl::lookupTable[(data[i] ^ crc) & 0xff];
			}
		}
	} else {
		const std::size_t fastN = n & ~size_t(0x07); // n - n % 8

		std::uint_fast64_t crc = currentCrc;

		if (fastN > 0) {
			// Calculating fast for as much data as possible.
			crc = crc64Fast64Impl(crc, data, fastN);
		}

		// The rest of the data is calculated using the slow version of CRC64.
		for (std::size_t i = fastN; i < n; ++i) {
			crc = (crc >> 8) ^ afc::crc64Reversed_impl::lookupTable[(data[i] ^ crc) & 0xff];
		}
	}

	return crc;
}

std::uint_fast64_t afc::crc64ReversedUpdate_Aligned8Impl(const std::uint_fast64_t currentCrc,
		const unsigned char * const data, const std::size_t n)
{
	return crc64ReversedUpdate_Aligned8Impl_Inline(currentCrc, data, n);
}

std::uint_fast64_t afc::crc64ReversedUpdate_Fast32(const std::uint_fast64_t currentCrc,
		const unsigned char * const data, const std::size_t n)
{
	using namespace afc::crc64Reversed_impl;

	assert(currentCrc == (currentCrc & 0xffffffffffffffff));
	assert(n % 4 == 0);

	std::uint_fast64_t crc = currentCrc;

	for (std::size_t i = 0; i < n; i += 4) {
		crc = (crc >> 32) ^
				lookupTable4[(data[i] ^ crc) & 0xff] ^
				lookupTable3[(data[i + 1] ^ (crc >> 8)) & 0xff] ^
				lookupTable2[(data[i + 2] ^ (crc >> 16)) & 0xff] ^
				lookupTable[(data[i + 3] ^ (crc >> 24)) & 0xff];
	}

	return crc;
}

std::uint_fast64_t afc::crc64ReversedUpdate_Fast64(const std::uint_fast64_t currentCrc,
		const unsigned char * const data, const std::size_t n)
{
	if (likely(n > 0)) {
		return crc64Fast64Impl(currentCrc, data, n);
	} else {
		return currentCrc;
	}
}
