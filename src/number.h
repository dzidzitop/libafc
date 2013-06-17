#ifndef AFC_NUMBER_H_
#define AFC_NUMBER_H_

#include <string>
#include "Exception.h"

namespace afc
{
	namespace number_limits
	{
		const unsigned MIN_BASE = 2;
		const unsigned MAX_BASE = 36;
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
	void appendToString(const int i, const unsigned base, std::string &out);
	// TODO test this
	void appendToString(const unsigned i, std::string &out);
	// TODO test this
	void appendToString(const unsigned i, const unsigned base, std::string &out);

	inline std::string toString(const int i, const unsigned base = 10) {string s; appendToString(i, base, s); return s;}
	// TODO test this
	inline std::string toString(const unsigned i, const unsigned base = 10) {string s; appendToString(i, base, s); return s;}
}

namespace std
{
	inline string &operator+=(string &str, const int i) {afc::appendToString(i, str); return str;}
	inline string &operator+=(string &str, const unsigned i) {afc::appendToString(i, str); return str;}
	inline std::string operator+(const string &str, const int i) {string s(str); return s += i;}
	inline std::string operator+(const string &str, const unsigned i) {string s(str); return s += i;}
}

#endif /*AFC_NUMBER_H_*/
