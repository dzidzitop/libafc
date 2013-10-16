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

	// TODO support efficient decimal base
	// TODO test this for all types
	template<typename T> void appendToString(const T value, const unsigned char base, std::string &out);
	template<typename T> void appendToString(const T value, std::string &out)
			{ appendToString(value, static_cast<unsigned char>(10), out); }

	inline std::string toString(const int i, const unsigned char base = 10)
			{string s; appendToString<int>(i, base, s); return s;}
	// TODO test this
	inline std::string toString(const unsigned i, const unsigned char base = 10)
			{string s; appendToString<unsigned>(i, base, s); return s;}
}

// TODO support efficient implementation of binary bases
template<typename T> void afc::appendToString(const T value, const unsigned char base, string &out)
{
	static_assert(std::is_integral<T>::value, "Integral types are supported only.");

	typedef typename std::make_unsigned<T>::type UnsignedT;
	using std::numeric_limits;
	using namespace afc::number_limits;
	using namespace afc::_impl;

	if (base < 2 || base > MAX_BASE) {
		// TODO Merge this with void throwInvalidBaseException() in number.cpp.
		throw InvalidArgumentException("Base must be between 2 and 36.");
	}

	unsigned count = 0;
	UnsignedT val = std::is_signed<T>::value && value < 0 ? -value : value;

	// TODO use direct order and bulk append.
	// The buffer that contains digits in the reverse order.
	char digits[numeric_limits<UnsignedT>::digits];

	while (val >= base) {
		const UnsignedT nextVal = val / base;
		digits[count++] = digitToChar[val - nextVal*base];
		val = nextVal;
	}
	digits[count++] = digitToChar[val];
	assert(count <= static_cast<unsigned>(numeric_limits<UnsignedT>::digits));
	out.reserve(out.size() + count);
	if (std::is_signed<T>::value && value < 0) {
		out += '-';
	}
	do {
		out += digits[--count];
	} while (count != 0);
}

#endif /*AFC_NUMBER_H_*/
