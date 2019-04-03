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

namespace afc
{
	namespace crc64_impl
	{
		extern const std::uint_fast64_t lookupTable[0x100];
	}

	std::uint_fast64_t crc64(const unsigned char *data, std::size_t n);
}

#endif /* AFC_CRC_HPP_ */
