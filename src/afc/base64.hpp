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
#ifndef AFC_BASE64_HPP_
#define AFC_BASE64_HPP_

#include <algorithm>
#include <cassert>
#include <limits>

namespace afc
{
	template<typename InputIterator, typename OutputIterator>
	OutputIterator encodeBase64(InputIterator begin, std::size_t size, OutputIterator dest);

	namespace _impl
	{
		static const char base64EncodeTable[] = {
				'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
				'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
				'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
				'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
				'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
				'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
				'w', 'x', 'y', 'z', '0', '1', '2', '3',
				'4', '5', '6', '7', '8', '9', '+', '/' };

		template<typename Iterator>
		inline Iterator encodeTriplet(Iterator p, char * const dest) noexcept
		{
			// Casting all chars to unsigned chars, because the result of applying >> is defined for the latter.
			const unsigned char v0 = static_cast<unsigned char>(*p);
			++p;
			const unsigned char v1 = static_cast<unsigned char>(*p);
			++p;
			const unsigned char v2 = static_cast<unsigned char>(*p);
			++p;

			const size_t pos1 = v0 >> 2;
			assert(pos1 < 64);
			const size_t pos2 = ((v0 & 0x03) << 4) + ((v1 & 0xf0) >> 4);
			assert(pos2 < 64);
			const size_t pos3 = ((v1 & 0x0f) << 2) + ((v2 & 0xc0) >> 6);
			assert(pos3 < 64);
			const size_t pos4 = v2 & 0x3f;
			assert(pos4 < 64);

			dest[0] = base64EncodeTable[pos1];
			dest[1] = base64EncodeTable[pos2];
			dest[2] = base64EncodeTable[pos3];
			dest[3] = base64EncodeTable[pos4];

			return p;
		}
	}
}

static_assert(std::numeric_limits<unsigned char>::digits == 8, "8-bit bytes (chars) are supported only.");

template<typename InputIterator, typename OutputIterator>
OutputIterator afc::encodeBase64(InputIterator begin, std::size_t size, OutputIterator dest)
{
	const std::size_t tailSize = size % 3;

	struct alignas(4) {
		char chunk[4];
	} buf;

	InputIterator p = begin;
	for (std::size_t i = size - tailSize; i != 0; i -= 3) {
		p = afc::_impl::encodeTriplet(p, buf.chunk);
		dest = std::copy_n(buf.chunk, 4, dest);
	}

	// Last one or two octets are encoded in a different way.
	if (tailSize > 0) {
		char tail[3];
		tail[0] = *p;
		++p;
		if (tailSize == 1) {
			tail[1] = 0;
		} else {
			tail[1] = *p;
			++p;
		}
		tail[2] = 0;

		afc::_impl::encodeTriplet(tail, buf.chunk);

		if (tailSize == 1) {
			buf.chunk[2] = '=';
		}
		buf.chunk[3] = '=';

		dest = std::copy_n(buf.chunk, 4, dest);
	}

	return dest;
}


#endif /* AFC_BASE64_HPP_ */
