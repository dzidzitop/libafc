#include "number.h"
#include <climits>
#include "compile_time_math.h"

using std::string;
using namespace afc;
using namespace afc::number_limits;

#define DIP(x) INT_MAX-(INT_MAX/(x))*(x)
#define DIN(x) static_cast<unsigned>(INT_MIN-(INT_MIN/(x))*(x))
#define DUIP(x) UINT_MAX-(UINT_MAX/(x))*(x)

// have to use macros here, because GCC does not inline functions with many arguments.
#define checkIntRange(currentVal, negative, lastDigit, maxValidPrev, base, str)\
	const int diff = static_cast<int>(currentVal - maxValidPrev); /* safe because current val >= 0 and maxValPrev is in the positive int range. */\
	if (diff >= 0) {\
		if (diff != 0 || lastDigit > maxLastDigitInt[negative][base]) {\
			if (negative) {\
				throwUnderflowExceptionInt(str);\
			} else {\
				throwOverflowExceptionInt(str);\
			}\
		}\
	}

#define checkUIntRange(currentVal, lastDigit, maxValidPrev, base, str)\
	if (currentVal >= maxValidPrev) {\
		if (currentVal != maxValidPrev || lastDigit > maxLastDigitUInt[base]) {\
			throwOverflowExceptionUInt(str);\
		}\
	}

#define parseAndReturnIntBase(str, size, base, i, negative)\
	{const unsigned maxValidPrev = maxPrevInt[negative][base];\
	unsigned result = 0;\
	do {\
		const char c = str[i];\
		if ((c&CHAR_TO_DIGIT_DISALLOWED_BITS) != 0) {\
			throwParseExceptionInt(str);\
		}\
		const unsigned val = charToDigit[static_cast<unsigned char>(c)];\
		if (val >= base) {\
			throwParseExceptionInt(str);\
		}\
		/*TODO probably first N iterations could be made without this check. The same is for the BinaryBase version of the function.*/\
		checkIntRange(result, negative, val, maxValidPrev, base, str);\
		result = result * base + val;\
	} while (++i < size);\
	return negative ? -static_cast<int>(result) : result;}

#define parseAndReturnIntBinaryBase(str, size, shift, i, negative)\
	{const unsigned char base = 1 << shift;\
	const unsigned maxValidPrev = maxPrevInt[negative][base];\
	unsigned result = 0;\
	for (; i < size; ++i) {\
		const char c = str[i];\
		if ((c&CHAR_TO_DIGIT_DISALLOWED_BITS) != 0) {\
			throwParseExceptionInt(str);\
		}\
		const unsigned val = charToDigit[static_cast<unsigned char>(c)];\
		if (val >= base) {\
			throwParseExceptionInt(str);\
		}\
		/*TODO probably first N iterations could be made without this check.*/\
		checkIntRange(result, negative, val, maxValidPrev, base, str);\
		result = (result << shift) + val;\
	}\
	return negative ? -static_cast<int>(result) : result;}

#define parseAndReturnUIntBase(str, size, base)\
	{const unsigned maxValidPrev = maxPrevUInt[base];\
	unsigned result = 0;\
	size_t _i = 0;\
	do {\
		const char c = str[_i];\
		if ((c&CHAR_TO_DIGIT_DISALLOWED_BITS) != 0) {\
			throwParseExceptionUInt(str);\
		}\
		const unsigned val = charToDigit[static_cast<unsigned char>(c)];\
		if (val >= base) {\
			throwParseExceptionUInt(str);\
		}\
		/*TODO probably first N iterations could be made without this check. The same is for the BinaryBase version of the function.*/\
		checkUIntRange(result, val, maxValidPrev, base, str);\
		result = result * base + val;\
	} while (++_i < size);\
	return result;}

#define parseAndReturnUIntBinaryBase(str, size, shift, start)\
	{const unsigned char base = 1 << shift;\
	const unsigned maxValidPrev = maxPrevUInt[base];\
	unsigned result = 0;\
	for (size_t _i = start; _i < size; ++_i) {\
		const char c = str[_i];\
		if ((c&CHAR_TO_DIGIT_DISALLOWED_BITS) != 0) {\
			throwParseExceptionUInt(str);\
		}\
		const unsigned val = charToDigit[static_cast<unsigned char>(c)];\
		if (val >= base) {\
			throwParseExceptionUInt(str);\
		}\
		/*TODO probably first N iterations could be made without this check.*/\
		checkUIntRange(result, val, maxValidPrev, base, str);\
		result = (result << shift) + val;\
	}\
	return result;}

namespace
{
	// Max value of parsed int at previous iteration which could not cause overflow/undeflow.
	template<int x> struct MPI
	{
		enum { pos = INT_MAX/x, neg = -(INT_MIN/x) };
	};
	// Max value of parsed unsigned int at previous iteration which could not cause overflow.
	template<unsigned x> struct MPUI
	{
		enum {val = UINT_MAX/x};
	};

	const unsigned char CHAR_TO_DIGIT_SIZE = 0x80;
	const char CHAR_TO_DIGIT_DISALLOWED_BITS =  ~0x7f;
	const unsigned char UD = 0xff; // undefined digit
	// TODO test these tables thoroughly
	const unsigned char charToDigit[CHAR_TO_DIGIT_SIZE] =
		{UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD,
		 UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD,
		 UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD, UD,
		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, UD, UD, UD, UD, UD, UD,
		 UD, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
		 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, UD, UD, UD, UD, UD,
		 UD, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
		 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, UD, UD, UD, UD};

	const unsigned maxPrevInt[2][MAX_BASE+1] =
		{{0, 0, MPI<2>::pos, MPI<3>::pos, MPI<4>::pos, MPI<5>::pos, MPI<6>::pos, MPI<7>::pos, MPI<8>::pos, MPI<9>::pos,
		  MPI<10>::pos, MPI<11>::pos, MPI<12>::pos, MPI<13>::pos, MPI<14>::pos, MPI<15>::pos, MPI<16>::pos, MPI<17>::pos, MPI<18>::pos,
		  MPI<19>::pos, MPI<20>::pos, MPI<21>::pos, MPI<22>::pos, MPI<23>::pos, MPI<24>::pos, MPI<25>::pos, MPI<26>::pos, MPI<27>::pos,
		  MPI<28>::pos, MPI<29>::pos, MPI<30>::pos, MPI<31>::pos, MPI<32>::pos, MPI<33>::pos, MPI<34>::pos, MPI<35>::pos, MPI<36>::pos},
		 {0, 0, MPI<2>::neg, MPI<3>::neg, MPI<4>::neg, MPI<5>::neg, MPI<6>::neg, MPI<7>::neg, MPI<8>::neg, MPI<9>::neg,
		  MPI<10>::neg, MPI<11>::neg, MPI<12>::neg, MPI<13>::neg, MPI<14>::neg, MPI<15>::neg, MPI<16>::neg, MPI<17>::neg, MPI<18>::neg,
		  MPI<19>::neg, MPI<20>::neg, MPI<21>::neg, MPI<22>::neg, MPI<23>::neg, MPI<24>::neg, MPI<25>::neg, MPI<26>::neg, MPI<27>::neg,
		  MPI<28>::neg, MPI<29>::neg, MPI<30>::neg, MPI<31>::neg, MPI<32>::neg, MPI<33>::neg, MPI<34>::neg, MPI<35>::neg, MPI<36>::neg}};
	// TODO theorethically, it is possible to get rid of it
	const unsigned maxLastDigitInt[2][MAX_BASE+1] =
		{{0, 0, DIP(2), DIP(3), DIP(4), DIP(5), DIP(6), DIP(7), DIP(8), DIP(9),
		  DIP(10), DIP(11), DIP(12), DIP(13), DIP(14), DIP(15), DIP(16), DIP(17), DIP(18), DIP(19),
		  DIP(20), DIP(21), DIP(22), DIP(23), DIP(24), DIP(25), DIP(26), DIP(27), DIP(28), DIP(29),
		  DIP(30), DIP(31), DIP(32), DIP(33), DIP(34), DIP(35), DIP(36)},
		 {0, 0, DIN(2), DIN(3), DIN(4), DIN(5), DIN(6), DIN(7), DIN(8), DIN(9),
		  DIN(10), DIN(11), DIN(12), DIN(13), DIN(14), DIN(15), DIN(16), DIN(17), DIN(18), DIN(19),
		  DIN(20), DIN(21), DIN(22), DIN(23), DIN(24), DIN(25), DIN(26), DIN(27), DIN(28), DIN(29),
		  DIN(30), DIN(31), DIN(32), DIN(33), DIN(34), DIN(35), DIN(36)}};
	const unsigned maxPrevUInt[MAX_BASE+1] = {
		0, 0, MPUI<2>::val, MPUI<3>::val, MPUI<4>::val, MPUI<5>::val, MPUI<6>::val, MPUI<7>::val, MPUI<8>::val, MPUI<9>::val,
		MPUI<10>::val, MPUI<11>::val, MPUI<12>::val, MPUI<13>::val, MPUI<14>::val, MPUI<15>::val, MPUI<16>::val, MPUI<17>::val, MPUI<18>::val,
		MPUI<19>::val, MPUI<20>::val, MPUI<21>::val, MPUI<22>::val, MPUI<23>::val, MPUI<24>::val, MPUI<25>::val, MPUI<26>::val, MPUI<27>::val,
		MPUI<28>::val, MPUI<29>::val, MPUI<30>::val, MPUI<31>::val, MPUI<32>::val, MPUI<33>::val, MPUI<34>::val, MPUI<35>::val, MPUI<36>::val
	};
	// TODO theorethically, it is possible to get rid of it
	const unsigned maxLastDigitUInt[MAX_BASE+1] = {
		0, 0, DUIP(2), DUIP(3), DUIP(4), DUIP(5), DUIP(6), DUIP(7), DUIP(8), DUIP(9),
		DUIP(10), DUIP(11), DUIP(12), DUIP(13), DUIP(14), DUIP(15), DUIP(16), DUIP(17), DUIP(18), DUIP(19),
		DUIP(20), DUIP(21), DUIP(22), DUIP(23), DUIP(24), DUIP(25), DUIP(26), DUIP(27), DUIP(28), DUIP(29),
		DUIP(30), DUIP(31), DUIP(32), DUIP(33), DUIP(34), DUIP(35), DUIP(36)
	};

	const unsigned LETTER_CASE_BIT = 32;
	const unsigned char HEX_SHIFT = 4;
	const unsigned char OCT_SHIFT = 3;

	void throwInvalidBaseException() {throwException<InvalidArgumentException>("Base must be between 2 and 36.");}
	void throwParseEmptyStringException() {throwException<ParseException>("Empty string is not a number.");}
	void throwParseExceptionInt(const string &str) {throwExceptionConcat<ParseException>(str, " is not an integer.");}
	void throwParseExceptionUInt(const string &str) {throwExceptionConcat<ParseException>(str, " is not an unsigned integer.");}
	void throwUnderflowExceptionInt(const string &str) {throwExceptionConcat<UnderflowException>(str, " is too small for the integer range.");}
	void throwOverflowExceptionInt(const string &str) {throwExceptionConcat<OverflowException>(str, " is too large for the integer range.");}
	void throwOverflowExceptionUInt(const string &str) {throwExceptionConcat<OverflowException>(str, " is too large for the unsigned integer range.");}

	inline int parseSign(const string &str, const size_t size, unsigned &negative) throw(afc::ParseException)
	{
		if (size == 0) {
			throwParseEmptyStringException();
		}
		switch (str[0]) {
		case '-':
			negative = 1;
			return 1;
		case '+':
			negative = 0;
			return 1;
		default:
			negative = 0;
			return 0;
		}
	}
}

int afc::parseOctInt(const string &str) throw(ParseException, OverflowException, UnderflowException)
{
	const size_t size = str.size();
	unsigned negative;
	size_t i = parseSign(str, size, negative);
	if (size == i) {
		throwParseExceptionInt(str);
	}
	if (str[i] == '0') {
		++i;
	}
	parseAndReturnIntBinaryBase(str, size, OCT_SHIFT, i, negative);
}

int afc::parseHexInt(const string &str) throw(ParseException, OverflowException, UnderflowException)
{
	const size_t size = str.size();
	unsigned negative;
	size_t i = parseSign(str, size, negative);
	if (size == i) {
		throwParseExceptionInt(str);
	}
	if (str[i] == '0') {
		++i;
		if (size > i && (str[i]|LETTER_CASE_BIT) == 'x') {
			++i;
			if (size == i) {
				throwParseExceptionInt(str);
			}
		}
	}
	parseAndReturnIntBinaryBase(str, size, HEX_SHIFT, i, negative);
}

int afc::parseInt(const string &str) throw(ParseException, OverflowException, UnderflowException)
{
	const size_t size = str.size();
	unsigned negative;
	size_t i = parseSign(str, size, negative);
	if (size == i) {
		throwParseExceptionInt(str);
	}
	if (str[i] != '0') {
		parseAndReturnIntBase(str, size, 10, i, negative);
	}
	++i;
	unsigned char shift;
	if (size > i && (str[i]|LETTER_CASE_BIT) == 'x') {
		++i;
		if (size == i) {
			throwParseExceptionInt(str);
		}
		shift = HEX_SHIFT;
	} else {
		shift = OCT_SHIFT;
	}
	parseAndReturnIntBinaryBase(str, size, shift, i, negative);
}

// TODO it is slow again!
int afc::parseInt(const string &str, unsigned base) throw(ParseException, InvalidArgumentException, OverflowException, UnderflowException)
{
	if (base < MIN_BASE || base > MAX_BASE) {
		throwInvalidBaseException();
	}
	const size_t size = str.size();
	unsigned negative;
	size_t i = parseSign(str, size, negative);
	if (size == i) {
		throwParseExceptionInt(str);
	}
	parseAndReturnIntBase(str, size, base, i, negative);
}

unsigned afc::parseOctUInt(const string &str) throw(ParseException, OverflowException)
{
	const size_t size = str.size();
	if (size == 0) {
		throwParseEmptyStringException();
	}
	parseAndReturnUIntBinaryBase(str, size, OCT_SHIFT, 0);
}

unsigned afc::parseHexUInt(const string &str) throw(ParseException, OverflowException)
{
	const size_t size = str.size();
	if (size == 0) {
		throwParseEmptyStringException();
	}
	size_t i = 0;
	if (str[i] == '0') {
		++i;
		if (size > i && (str[i]|LETTER_CASE_BIT) == 'x') {
			++i;
			if (size == i) {
				throwParseExceptionUInt(str);
			}
		}
	}
	parseAndReturnUIntBinaryBase(str, size, HEX_SHIFT, i);
}

unsigned afc::parseUInt(const string &str) throw(ParseException, OverflowException)
{
	const size_t size = str.size();
	if (size == 0) {
		throwParseEmptyStringException();
	}
	if (str[0] != '0') {
		parseAndReturnUIntBase(str, size, 10);
	}

	size_t i = 1;
	unsigned char shift;
	if (size > i && (str[i]|LETTER_CASE_BIT) == 'x') {
		++i;
		if (size == i) {
			throwParseExceptionUInt(str);
		}
		shift = HEX_SHIFT;
	} else {
		shift = OCT_SHIFT;
	}
	parseAndReturnUIntBinaryBase(str, size, shift, i);
}

// TODO optimise and verify this!
unsigned afc::parseUInt(const string &str, unsigned base) throw(ParseException, InvalidArgumentException, OverflowException)
{
	if (base < MIN_BASE || base > MAX_BASE) {
		throwInvalidBaseException();
	}
	const size_t size = str.size();
	if (size == 0) {
		throwParseEmptyStringException();
	}
	parseAndReturnUIntBase(str, size, base);
}
