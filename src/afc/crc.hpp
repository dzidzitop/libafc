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

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace afc
{
	namespace crc64_impl
	{
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

	// Each data chunk must be aligned by 4 octets. The result will be incorrect otherwise!
	std::uint_fast64_t crc64Update_FastAligned32(std::uint_fast64_t currentCrc,
			const unsigned char *data, std::size_t n);

	// Each data chunk must be aligned by 8 octets. The result will be incorrect otherwise!
	std::uint_fast64_t crc64Update_FastAligned64(std::uint_fast64_t currentCrc,
			const unsigned char *data, std::size_t n);

	inline std::uint_fast64_t crc64(const unsigned char * const data, const std::size_t n)
	{
		return crc64Update(0, data, n);
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
