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

namespace
{
	/* x^64 + x^62 + x^57 + x^55 + x^54 + x^53 + x^52 + x^47 + x^46 + x^45 + x^40 +
	 * x^39 + x^38 + x^37 + x^35 + x^33 + x^32 + x^31 + x^29 + x^27 + x^24 + x^23 +
	 * x^22 + x^21 + x^19 + x^17 + x^13 + x^12 + x^10 + x^9  + x^7  + x^4  + x^1  + 1;
	 */
	const std::uint_fast64_t polynome = 0xc96c5795d7870f42;


	constexpr std::uint_fast64_t tableValLoop(const std::uint_fast64_t crc, const unsigned char iteration)
	{
		// Emulating 'for (int i = 0; i < 8; ++i)' with crc recalculated on each iteration.
		return iteration == 8 ? crc :
				tableValLoop((crc & 1) ? (crc >> 1) ^ polynome : crc >> 1, iteration + 1);
	}

	constexpr std::uint_fast64_t tableVal(const unsigned char index)
	{
		return tableValLoop(index, 0);
	}
}

// CRC64 value for each 00 00 00 00 00 00 00 xx.
const std::uint_fast64_t afc::crc64_impl::lookupTable[0x100] = {
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

std::uint_fast64_t afc::crc64Update(const std::uint_fast64_t currentCrc,
		const unsigned char * const data, const std::size_t n)
{
	assert(currentCrc == currentCrc & 0xffffffffffffffff);

	std::uint_fast64_t crc = currentCrc;

	for (std::size_t i= 0; i < n; ++i) {
		crc = (crc >> 8) ^ afc::crc64_impl::lookupTable[(data[i] ^ crc) & 0xff];
	}

	return crc;
}
