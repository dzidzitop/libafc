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

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace afc
{
	namespace crc64_impl
	{
		extern const std::uint_fast64_t lookupTable[0x100];
	}

	std::uint_fast64_t crc64(const unsigned char *data, std::size_t n);

	template<typename Iterator>
	std::uint_fast64_t crc64(Iterator begin, Iterator end)
	{
		static_assert(std::is_same<typename std::decay<decltype(*std::declval<Iterator>())>::type, unsigned char>::value, "Not an unsigned char iterator.");

		std::uint_fast64_t crc = 0;

		for (Iterator p = begin; p != end; ++p) {
			crc = (crc >> 8) ^ afc::crc64_impl::lookupTable[(*p ^ crc) & 0xff];
		}

		return crc;
	}
}

#endif /* AFC_CRC_HPP_ */
