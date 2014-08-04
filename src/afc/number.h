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
#ifndef AFC_NUMBER_H_
#define AFC_NUMBER_H_

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>
#include <type_traits>

namespace afc
{
	namespace number_limits
	{
		const unsigned MIN_BASE = 2;
		const unsigned MAX_BASE = 36;
	}

	namespace _impl
	{
		static constexpr char digitChars[number_limits::MAX_BASE] =
			{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
			 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
			 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
			 'u', 'v', 'w', 'x', 'y', 'z'};
	}

	/* TODO check thoroughly if this table can provide better performance
	 * against bit manipulations on real load (cache miss effects).
	 */
	static const char octetToHexTable[256][2] = {
			{'0', '0'}, {'0', '1'}, {'0', '2'}, {'0', '3'}, {'0', '4'}, {'0', '5'}, {'0', '6'}, {'0', '7'},
			{'0', '8'}, {'0', '9'}, {'0', 'a'}, {'0', 'b'}, {'0', 'c'}, {'0', 'd'}, {'0', 'e'}, {'0', 'f'},
			{'1', '0'}, {'1', '1'}, {'1', '2'}, {'1', '3'}, {'1', '4'}, {'1', '5'}, {'1', '6'}, {'1', '7'},
			{'1', '8'}, {'1', '9'}, {'1', 'a'}, {'1', 'b'}, {'1', 'c'}, {'1', 'd'}, {'1', 'e'}, {'1', 'f'},
			{'2', '0'}, {'2', '1'}, {'2', '2'}, {'2', '3'}, {'2', '4'}, {'2', '5'}, {'2', '6'}, {'2', '7'},
			{'2', '8'}, {'2', '9'}, {'2', 'a'}, {'2', 'b'}, {'2', 'c'}, {'2', 'd'}, {'2', 'e'}, {'2', 'f'},
			{'3', '0'}, {'3', '1'}, {'3', '2'}, {'3', '3'}, {'3', '4'}, {'3', '5'}, {'3', '6'}, {'3', '7'},
			{'3', '8'}, {'3', '9'}, {'3', 'a'}, {'3', 'b'}, {'3', 'c'}, {'3', 'd'}, {'3', 'e'}, {'3', 'f'},
			{'4', '0'}, {'4', '1'}, {'4', '2'}, {'4', '3'}, {'4', '4'}, {'4', '5'}, {'4', '6'}, {'4', '7'},
			{'4', '8'}, {'4', '9'}, {'4', 'a'}, {'4', 'b'}, {'4', 'c'}, {'4', 'd'}, {'4', 'e'}, {'4', 'f'},
			{'5', '0'}, {'5', '1'}, {'5', '2'}, {'5', '3'}, {'5', '4'}, {'5', '5'}, {'5', '6'}, {'5', '7'},
			{'5', '8'}, {'5', '9'}, {'5', 'a'}, {'5', 'b'}, {'5', 'c'}, {'5', 'd'}, {'5', 'e'}, {'5', 'f'},
			{'6', '0'}, {'6', '1'}, {'6', '2'}, {'6', '3'}, {'6', '4'}, {'6', '5'}, {'6', '6'}, {'6', '7'},
			{'6', '8'}, {'6', '9'}, {'6', 'a'}, {'6', 'b'}, {'6', 'c'}, {'6', 'd'}, {'6', 'e'}, {'6', 'f'},
			{'7', '0'}, {'7', '1'}, {'7', '2'}, {'7', '3'}, {'7', '4'}, {'7', '5'}, {'7', '6'}, {'7', '7'},
			{'7', '8'}, {'7', '9'}, {'7', 'a'}, {'7', 'b'}, {'7', 'c'}, {'7', 'd'}, {'7', 'e'}, {'7', 'f'},
			{'8', '0'}, {'8', '1'}, {'8', '2'}, {'8', '3'}, {'8', '4'}, {'8', '5'}, {'8', '6'}, {'8', '7'},
			{'8', '8'}, {'8', '9'}, {'8', 'a'}, {'8', 'b'}, {'8', 'c'}, {'8', 'd'}, {'8', 'e'}, {'8', 'f'},
			{'9', '0'}, {'9', '1'}, {'9', '2'}, {'9', '3'}, {'9', '4'}, {'9', '5'}, {'9', '6'}, {'9', '7'},
			{'9', '8'}, {'9', '9'}, {'9', 'a'}, {'9', 'b'}, {'9', 'c'}, {'9', 'd'}, {'9', 'e'}, {'9', 'f'},
			{'a', '0'}, {'a', '1'}, {'a', '2'}, {'a', '3'}, {'a', '4'}, {'a', '5'}, {'a', '6'}, {'a', '7'},
			{'a', '8'}, {'a', '9'}, {'a', 'a'}, {'a', 'b'}, {'a', 'c'}, {'a', 'd'}, {'a', 'e'}, {'a', 'f'},
			{'b', '0'}, {'b', '1'}, {'b', '2'}, {'b', '3'}, {'b', '4'}, {'b', '5'}, {'b', '6'}, {'b', '7'},
			{'b', '8'}, {'b', '9'}, {'b', 'a'}, {'b', 'b'}, {'b', 'c'}, {'b', 'd'}, {'b', 'e'}, {'b', 'f'},
			{'c', '0'}, {'c', '1'}, {'c', '2'}, {'c', '3'}, {'c', '4'}, {'c', '5'}, {'c', '6'}, {'c', '7'},
			{'c', '8'}, {'c', '9'}, {'c', 'a'}, {'c', 'b'}, {'c', 'c'}, {'c', 'd'}, {'c', 'e'}, {'c', 'f'},
			{'d', '0'}, {'d', '1'}, {'d', '2'}, {'d', '3'}, {'d', '4'}, {'d', '5'}, {'d', '6'}, {'d', '7'},
			{'d', '8'}, {'d', '9'}, {'d', 'a'}, {'d', 'b'}, {'d', 'c'}, {'d', 'd'}, {'d', 'e'}, {'d', 'f'},
			{'e', '0'}, {'e', '1'}, {'e', '2'}, {'e', '3'}, {'e', '4'}, {'e', '5'}, {'e', '6'}, {'e', '7'},
			{'e', '8'}, {'e', '9'}, {'e', 'a'}, {'e', 'b'}, {'e', 'c'}, {'e', 'd'}, {'e', 'e'}, {'e', 'f'},
			{'f', '0'}, {'f', '1'}, {'f', '2'}, {'f', '3'}, {'f', '4'}, {'f', '5'}, {'f', '6'}, {'f', '7'},
			{'f', '8'}, {'f', '9'}, {'f', 'a'}, {'f', 'b'}, {'f', 'c'}, {'f', 'd'}, {'f', 'e'}, {'f', 'f'}
	};

	// TODO think how to declare noexcept
	template<typename OutputIterator>
	OutputIterator octetToHex(const unsigned char o, const OutputIterator dest)
	{
		assert(o >= 0 && o <= 0xff);
		OutputIterator end = dest;
		const char *p = octetToHexTable[o];
		*end++ = *p;
		*end++ = *(p + 1);
		return end;
	}

	// TODO take advantage of using base.
	template<typename T, unsigned char base>
	constexpr std::size_t maxPrintedSize() noexcept
	{
		typedef typename std::make_unsigned<T>::type UnsignedT;
		return std::is_signed<T>::value ?
				// One character for sign.
				std::numeric_limits<UnsignedT>::digits + 1 :
				std::numeric_limits<UnsignedT>::digits;
	}

	// TODO use std::enable_if
	template<unsigned char base, typename T>
	inline constexpr char digitToChar(const T digit) noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");
		static_assert(base >= afc::number_limits::MIN_BASE && base <= afc::number_limits::MAX_BASE, "Unsupported base.");

		return base <= 10 ? '0' + digit : _impl::digitChars[digit];
	};

	// A hex digit must be passed in.
	template<typename T>
	constexpr char hexToChar(const T digit) noexcept { return digitToChar<16>(digit); }

	// TODO think of defining conditional noexcept.
	template<unsigned char base, typename T, typename OutputIterator>
	OutputIterator printNumber(const T value, OutputIterator dest);

	// TODO test this for all types and bases
	// TODO optimise performance (especially for binary bases)
	/* TODO modify iterator-based afc::printNumber so that it is possible to avoid copying digits
	 * to the intermediate buffer (in the reverse order).
	 */
	template<typename T, unsigned char base = 10>
	inline void printNumber(const T value, std::string &out)
	{
		char buf[maxPrintedSize<T, base>()];

		char * const begin = &buf[0];
		char * const end = printNumber<base>(value, begin);
		out.append(begin, end);
	}

	template<typename T, typename Iterator>
	inline Iterator printTwoDigits(const T value, Iterator dest)
	{
		assert(value >= 0 && value < 100);

		const std::uint_fast8_t high = value / 10;
		const std::uint_fast8_t low = value - high * 10;
		*dest++ = afc::digitToChar<10>(high);
		*dest++ = afc::digitToChar<10>(low);
		return dest;
	}

	template<typename T, typename Iterator>
	inline Iterator printFourDigits(const T value, Iterator dest)
	{
		assert(value >= 0 && value <= 9999);

		const std::uint_fast16_t hundreds = value / 100;
		dest = afc::printTwoDigits(hundreds, dest);
		dest = afc::printTwoDigits(value - hundreds * 100, dest);
		return dest;
	}
}

template<unsigned char base, typename T, typename Iterator>
Iterator afc::printNumber(const T value, register Iterator dest)
{
	static_assert(std::is_integral<T>::value, "Integral types are supported only.");
	static_assert(base >= afc::number_limits::MIN_BASE && base <= afc::number_limits::MAX_BASE, "Unsupported base.");

	typedef typename std::make_unsigned<T>::type UnsignedT;
	using std::numeric_limits;
	using namespace afc::_impl;

	// TODO use direct order and bulk append.
	// TODO calculate size using base.
	// The buffer that contains digits in the reverse order.
	static_assert(numeric_limits<UnsignedT>::digits - 1 == numeric_limits<typename std::make_signed<T>::type>::digits,
			"Unsupported number representation.");

	// The two's complement representation can take that even for signed values for the min value for base == 2.
	constexpr std::size_t maxDigitCount = numeric_limits<UnsignedT>::digits;
	char digits[maxDigitCount];
	unsigned count = 0;

	/** If value is equal to the min signed value then the negation of it is either
	 *  max signed value (for ones' complement and sign/magnitude signed representations) or
	 *  min signed value itself (for the two's complement representation). In the latter case
	 *  the min signed value is casted to the correspondent unsigned value correctly as if
	 *  it were properly negated.
	 */
	UnsignedT val = static_cast<UnsignedT>(value < 0 ? -value : value);

	while (val >= base) {
		const UnsignedT nextVal = val / base;
		digits[count++] = digitToChar<base>(val - nextVal * base);
		val = nextVal;
	}
	digits[count++] = digitToChar<base>(val);

	assert(count <= maxDigitCount);

	if (value < 0) {
		*dest++ = '-';
	}
	do {
		*dest++ = digits[--count];
	} while (count != 0);

	return dest;
}

#endif /*AFC_NUMBER_H_*/
