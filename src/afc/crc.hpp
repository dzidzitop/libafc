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
#ifndef AFC_CRC_HPP_
#define AFC_CRC_HPP_

#include "cpu/primitive.h"
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace afc
{
	namespace crc64_impl
	{
		/* x^64 + x^62 + x^57 + x^55 + x^54 + x^53 + x^52 + x^47 + x^46 + x^45 + x^40 +
		 * x^39 + x^38 + x^37 + x^35 + x^33 + x^32 + x^31 + x^29 + x^27 + x^24 + x^23 +
		 * x^22 + x^21 + x^19 + x^17 + x^13 + x^12 + x^10 + x^9  + x^7  + x^4  + x^1  + 1;
		 */
		constexpr std::uint_fast64_t polynome_reversed = 0xc96c5795d7870f42;

		// CRC64 of xx
		extern const std::uint_fast64_t lookupTable[0x100];
		// CRC64 of xx00
		extern const std::uint_fast64_t lookupTable2[0x100];
		// CRC64 of xx0000
		extern const std::uint_fast64_t lookupTable3[0x100];
		// CRC64 of xx000000
		extern const std::uint_fast64_t lookupTable4[0x100];
		// CRC64 of xx00000000
		extern const std::uint_fast64_t lookupTable5[0x100];
		// CRC64 of xx0000000000
		extern const std::uint_fast64_t lookupTable6[0x100];
		// CRC64 of xx000000000000
		extern const std::uint_fast64_t lookupTable7[0x100];
		// CRC64 of xx00000000000000
		extern const std::uint_fast64_t lookupTable8[0x100];
	}

	std::uint_fast64_t crc64Update(std::uint_fast64_t currentCrc, const unsigned char *data, std::size_t n);

	std::uint_fast64_t crc64Update_Aligned8Impl(std::uint_fast64_t currentCrc, const unsigned char *data,
			std::size_t n);

	inline std::uint_fast64_t crc64Update_Aligned8(const std::uint_fast64_t currentCrc, const unsigned char *data,
			const std::size_t n) {
		if (afc::PLATFORM_BYTE_ORDER == afc::endianness::LE && sizeof(std::uint_fast64_t) == 8 && CHAR_BIT == 8) {
			// An optimised version that works only for little-endian platforms with 64-bit std::uint_fast64_t.
			return crc64Update_Aligned8Impl(currentCrc, data, n);
		} else {
			return crc64Update(currentCrc, data, n);
		}
	}

	// Each data chunk must be aligned by 4 octets. The result will be incorrect otherwise!
	std::uint_fast64_t crc64Update_Fast32(std::uint_fast64_t currentCrc,
			const unsigned char *data, std::size_t n);

	// Each data chunk must be aligned by 8 octets. The result will be incorrect otherwise!
	std::uint_fast64_t crc64Update_Fast64(std::uint_fast64_t currentCrc,
			const unsigned char *data, std::size_t n);

	// CRC-64 ECMA with LSB-first bit order (reversed).
	inline std::uint_fast64_t crc64(const unsigned char * const data, const std::size_t n)
	{
		return crc64Update(0, data, n);
	}

	/* CRC-64 ECMA with LSB-first bit order (reversed).
	 *
	 * It works on data aligned to 8-byte boundary only. On little-endian platforms this can
	 * gain some performance boost.
	 */
	inline std::uint_fast64_t crc64_Aligned8(const unsigned char * const data, const std::size_t n)
	{
		return crc64Update_Aligned8(0, data, n);
	}

	template<typename Iterator>
	std::uint_fast64_t crc64Update(const std::uint_fast64_t currentCrc, Iterator begin, Iterator end)
	{
		using DataType = typename std::decay<decltype(*std::declval<Iterator>())>::type;
		static_assert(std::is_same<DataType, unsigned char>::value, "Not an unsigned char iterator.");

		assert(currentCrc == (currentCrc & 0xffffffffffffffff));

		std::uint_fast64_t crc = currentCrc;

		for (Iterator p = begin; p != end; ++p) {
			crc = (crc >> 8) ^ afc::crc64_impl::lookupTable[(*p ^ crc) & 0xff];
		}

		return crc;
	}

	template<typename Iterator>
	std::uint_fast64_t crc64(Iterator begin, Iterator end) { return crc64Update(0, begin, end); }
}

#endif /* AFC_CRC_HPP_ */
