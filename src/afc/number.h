/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2013 Dźmitry Laŭčuk

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
#ifndef AFC_NUMBER_H_
#define AFC_NUMBER_H_

#include <string>
#include "Exception.h"
#include <type_traits>
#include <limits>
#include <cassert>

namespace afc
{
	namespace number_limits
	{
		const unsigned MIN_BASE = 2;
		const unsigned MAX_BASE = 36;
	}

	namespace _impl
	{
		static const char digitToChar[number_limits::MAX_BASE] =
			{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
			 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
			 'u', 'v', 'w', 'x', 'y', 'z'};
	}

	template<typename T, unsigned char base, typename OutputIterator>
	OutputIterator printNumber(const T value, OutputIterator dest);

	// TODO test this for all types and bases
	// TODO optimise performance
	template<typename T, unsigned char base = 10>
	void printNumber(const T value, std::string &out)
	{
		typedef typename std::make_unsigned<T>::type UnsignedT;
		// One character for sign.
		constexpr std::size_t maxSize = std::is_signed<T>::value ?
				std::numeric_limits<UnsignedT>::digits + 1 :
				std::numeric_limits<UnsignedT>::digits;

		char buf[maxSize];

		char * const begin = &buf[0];
		char * const end = printNumber<T, base, char *>(value, begin);
		out.append(begin, end);
	}
}

#include <iostream>
using namespace std;
template<typename T, unsigned char base, typename OutputIterator>
OutputIterator afc::printNumber(const T value, const OutputIterator dest)
{
	static_assert(std::is_integral<T>::value, "Integral types are supported only.");
	static_assert(base >= afc::number_limits::MIN_BASE && base <= afc::number_limits::MAX_BASE, "Unsupported base.");

	typedef typename std::make_unsigned<T>::type UnsignedT;
	using std::numeric_limits;
	using namespace afc::_impl;

	unsigned count = 0;
	UnsignedT val = value < 0 ? -value : value;

	// TODO use direct order and bulk append.
	// TODO calculate size using base.
	// The buffer that contains digits in the reverse order.
	static_assert(numeric_limits<UnsignedT>::digits - 1 == numeric_limits<typename std::make_signed<T>::type>::digits,
			"Unsupported number representation.");

	// The two's complement representation can take that even for signed values for the min value for base == 2.
	constexpr std::size_t maxDigitCount = numeric_limits<UnsignedT>::digits;
	char digits[maxDigitCount];

	while (val >= base) {
		const UnsignedT nextVal = val / base;
		digits[count++] = digitToChar[val - nextVal*base];
		val = nextVal;
	}
	digits[count++] = digitToChar[val];

	assert(count <= maxDigitCount);

	OutputIterator end = dest;
	if (value < 0) {
		*end++ = '-';
	}
	do {
		*end++ = digits[--count];
	} while (count != 0);

	return end;
}

#endif /*AFC_NUMBER_H_*/
