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

	int parseInt(const std::string &str) throw(ParseException, OverflowException, UnderflowException);
	int parseInt(const std::string &str, unsigned base) throw(ParseException, InvalidArgumentException, OverflowException, UnderflowException);
	// TODO
	//static int parseInt(const char * const str, unsigned base = 0) throw(ParseException, InvalidArgumentException);
	// TODO test this
	int parseOctInt(const std::string &str) throw(ParseException, OverflowException, UnderflowException);
	// TODO test this
	int parseHexInt(const std::string &str) throw(ParseException, OverflowException, UnderflowException);
	// TODO test this
	// TODO optimise performance
	// TODO another impl. for base == 0
	unsigned parseUInt(const std::string &str) throw(ParseException, OverflowException);
	unsigned parseUInt(const std::string &str, unsigned base) throw(ParseException, InvalidArgumentException, OverflowException);
	// TODO
	//static int parseUInt(const char * const str, unsigned base = 0) throw(ParseException, InvalidArgumentException);
	// TODO test this
	unsigned parseOctUInt(const std::string &str) throw(ParseException, OverflowException);
	// TODO test this
	unsigned parseHexUInt(const std::string &str) throw(ParseException, OverflowException);

	void appendToString(const int i, std::string &out);
	// TODO test this
	void appendToString(const unsigned i, std::string &out);
	// TODO test this
	template<typename T> void appendToString(const T value, const unsigned char base, std::string &out);

	inline std::string toString(const int i, const unsigned base = 10) {string s; appendToString<int>(i, base, s); return s;}
	// TODO test this
	inline std::string toString(const unsigned i, const unsigned base = 10) {string s; appendToString<unsigned>(i, base, s); return s;}
}

namespace std
{
	inline string &operator+=(string &str, const int i) {afc::appendToString(i, str); return str;}
	inline string &operator+=(string &str, const unsigned i) {afc::appendToString(i, str); return str;}
	inline std::string operator+(const string &str, const int i) {string s(str); return s += i;}
	inline std::string operator+(const string &str, const unsigned i) {string s(str); return s += i;}
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

	size_t count = 0;
	UnsignedT val = std::is_signed<T>::value && value < 0 ? -value : value;

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
