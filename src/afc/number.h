/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2015 Dźmitry Laŭčuk

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
#include <type_traits>

#include "builtin.hpp"
#include "math_utils.h"

namespace afc
{
	namespace number_limits
	{
		const unsigned MIN_BASE = 2;
		const unsigned MAX_BASE = 36;
	}

	namespace _impl
	{
		// Return value is negated for negative limits.
		template<typename T, int base, bool positive>
		constexpr T safeLimit() noexcept
		{
			return positive ?
					(std::numeric_limits<T>::max() - base + 1) / base :
					(std::is_signed<T>::value ? -((std::numeric_limits<T>::min() + base - 1) / base) : 0);
		}

		// Return value is negated for negative limits.
		template<typename T, int base, bool positive>
		constexpr T mulLimit() noexcept
		{
			return positive ?
					std::numeric_limits<T>::max() / base :
					(std::is_signed<T>::value ? -(std::numeric_limits<T>::min() / base) : 0);
		}

		extern const char digitChars[number_limits::MAX_BASE];

		/* TODO check thoroughly if this table can provide better performance
		 * against bit manipulations on real load (cache miss effects).
		 */
		extern const char octetToHexTable[256][2];

		extern const unsigned char asciiToDigit[256];
	}

	enum class ParseMode
	{
		scan,
		all
	};

	// TODO think how to declare noexcept
	template<typename OutputIterator>
	OutputIterator octetToHex(const unsigned char o, const OutputIterator dest)
	{
		assert(o >= 0 && o <= 0xff);
		OutputIterator end = dest;
		const char *p = afc::_impl::octetToHexTable[o];
		*end = *p;
		++end;
		*end = *(p + 1);
		++end;
		return end;
	}

	template<unsigned char base, typename T>
	constexpr std::size_t printedSize(T val) noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");
		static_assert(std::is_unsigned<T>::value, "T must be an unsigned type.");

		return val < base ? 1 : 1 + printedSize<base, T>(val / base);
	}

	template<typename T, unsigned char base>
	constexpr std::size_t maxPrintedSize() noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");

		typedef typename std::make_unsigned<T>::type UnsignedT;
		return std::is_signed<T>::value ?
				// One character for sign.
				printedSize<base>(std::numeric_limits<UnsignedT>::max()) + 1 :
				printedSize<base>(std::numeric_limits<UnsignedT>::max());
	}

	template<unsigned char base, typename T>
	constexpr std::size_t digitCount(const T val)
	{
		static_assert(std::is_integral<T>::value, "An integral type is expected.");
		static_assert(base >= 2, "Base must be greater than or equal to two.");
		return (val < 0 && val > -static_cast<T>(base)) || (val >= 0 && val < base) ?
				1 : 1 + digitCount<base, T>(val / base);
	}

	template<unsigned char base, typename T>
	constexpr std::size_t maxDigitCount()
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");

		/* The min signed value contains at least as many digits as the max signed value
		 * for all signed type representations allowed in C++11. However, calculating
		 * it for both min and max just in case something changes. It is free of charge
		 * anyway.
		 */
		return std::is_signed<T>::value ?
				afc::math::max(digitCount<base>(std::numeric_limits<T>::min()), digitCount<base>(std::numeric_limits<T>::min())) :
				digitCount<base>(std::numeric_limits<T>::max());
	}

	// TODO use std::enable_if
	template<unsigned char base, typename T>
	constexpr char digitToChar(const T digit) noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");
		static_assert(base >= afc::number_limits::MIN_BASE && base <= afc::number_limits::MAX_BASE, "Unsupported base.");

		return base <= 10 ? '0' + digit : _impl::digitChars[digit];
	};

	// A hex digit must be passed in.
	template<typename T>
	constexpr char hexToChar(const T digit) noexcept { return digitToChar<16>(digit); }

	/* TODO modify iterator-based afc::printNumber so that it is possible to avoid copying digits
	 * to the intermediate buffer (in the reverse order).
	 */
	// TODO think of defining conditional noexcept.
	template<unsigned char base, typename T, typename OutputIterator>
	OutputIterator printNumber(const T value, OutputIterator dest);

	// TODO test this for all types and bases
	// TODO optimise performance (especially for binary bases)
	template<unsigned char base, typename T, typename Appender>
	inline void appendNumber(const T value, Appender appender)
	{
		char buf[maxPrintedSize<T, base>()];

		char * const begin = &buf[0];
		char * const end = printNumber<base>(value, begin);
		appender(begin, end);
	}

	// TODO think of defining conditional noexcept.
	template<unsigned char base, afc::ParseMode parseMode = ParseMode::all,
			typename T, typename Iterator, typename ErrorHandler>
	Iterator parseNumber(Iterator begin, Iterator end, T &result, ErrorHandler errorHandler);

	template<typename T, typename Iterator>
	inline Iterator printTwoDigits(const T value, Iterator dest)
	{
		assert(value >= 0 && value < 100);

		const std::uint_fast8_t high = value / 10;
		const std::uint_fast8_t low = value - high * 10;
		*dest = afc::digitToChar<10>(high);
		++dest;
		*dest = afc::digitToChar<10>(low);
		++dest;
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

	// TODO use direct order and bulk append.

	// The buffer that contains digits in the reverse order. The greatest digit is not stored here.
	char digits[maxDigitCount<base, T>() - 1];
	std::size_t i = 0;

	/** If value is equal to the min signed value then the negation of it is either
	 *  max signed value (for ones' complement and sign/magnitude signed representations) or
	 *  min signed value itself (for the two's complement representation). In the latter case
	 *  the min signed value is casted to the correspondent unsigned value correctly as if
	 *  it were properly negated.
	 */
	UnsignedT val;
	if (value >= 0) {
		val = value;
	} else {
		val = static_cast<UnsignedT>(-value);
		*dest = '-';
		++dest;
	}

	while (val >= base) {
		const UnsignedT nextVal = val / base;
		digits[i++] = digitToChar<base>(val - nextVal * base);
		val = nextVal;
	}
	assert(i <= sizeof(digits));

	// The highest digit.
	*dest = digitToChar<base>(val);
	++dest;
	while (i != 0) {
		*dest = digits[--i];
		++dest;
	}
	return dest;
}

// TODO optimise performance
template<unsigned char base, afc::ParseMode parseMode, typename T, typename Iterator, typename ErrorHandler>
Iterator afc::parseNumber(Iterator begin, Iterator end, T &dest, ErrorHandler errorHandler)
{
	static_assert(std::is_integral<T>::value, "Integral types are supported only.");
	static_assert(base >= afc::number_limits::MIN_BASE && base <= afc::number_limits::MAX_BASE, "Unsupported base.");
	static_assert(parseMode == ParseMode::all || parseMode == ParseMode::scan, "Unsupported parsing mode.");

	char c;
	T sign;
	T safeLimit;
	T result;

	Iterator p = begin;
	if (unlikely(p == end)) {
		goto error;
	}

	c = *p;
	if (std::is_signed<T>::value) {
		if (c == u8"-"[0]) {
			if (++p == end) {
				goto error;
			}
			c = *p;
			sign = -1;
			safeLimit = _impl::safeLimit<T, base, false>();
		} else {
			sign = 1;
			safeLimit = _impl::safeLimit<T, base, true>();
		}
	} else {
		safeLimit = _impl::safeLimit<T, base, true>();
	}

	if (c > 0xff) {
		goto error;
	}
	result = afc::_impl::asciiToDigit[static_cast<unsigned char>(c)];
	if (unlikely(result >= base)) {
		goto error;
	}

	while (++p != end) {
		c = *p;
		if (c > 0xff) {
			goto error;
		}
		const T digit = afc::_impl::asciiToDigit[static_cast<unsigned char>(c)];
		if (unlikely(digit >= base)) {
			switch (parseMode) {
			case ParseMode::all: goto error;
			case ParseMode::scan: goto loopEnd;
			}
		}

		if (likely(result <= safeLimit)) {
			result *= base;
			result += digit;
		} else {
			T mulLimit;
			T addLimit;
			if (std::is_signed<T>::value) {
				if (sign < 0) {
					mulLimit = _impl::mulLimit<T, base, false>();
					addLimit = std::numeric_limits<T>::min();
				} else {
					mulLimit = _impl::mulLimit<T, base, true>();
					addLimit = -std::numeric_limits<T>::max();
				}
			} else {
				mulLimit = _impl::mulLimit<T, base, true>();
				addLimit = std::numeric_limits<T>::max();
			}

			if (likely(result <= mulLimit)) {
				result *= base;
				if (std::is_signed<T>::value) {
					if (likely(addLimit + result + digit <= 0)) {
						result += digit;
						continue;
					}
				} else {
					if (likely(result <= addLimit - digit)) {
						result += digit;
						continue;
					}
				}
			}
			goto error;
		}
	}
loopEnd:
	if (std::is_signed<T>::value) {
		result *= sign; // Works even for min value for all sign encoding schemes.
	}
	dest = result;
	return p;
error:
	errorHandler(p);
	return p;
}

#endif /*AFC_NUMBER_H_*/
