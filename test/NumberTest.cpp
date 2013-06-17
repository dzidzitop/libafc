#include "NumberTest.h"
#include "number.h"
#include "Stopwatch.h"
#include <cstdlib>
#include <climits>
#include "compile_time_math.h"

using namespace std;

afc::NumberTest::NumberTest()
{
	TEST_ADD(NumberTest::testParseInt);
	TEST_ADD(NumberTest::testParseUInt);
	TEST_ADD(NumberTest::testAppendToString);
	TEST_ADD(NumberTest::testToStringInt);
}

void afc::NumberTest::testParseInt()
{
	// TODO more tests (overflow!)
	TEST_ASSERT(parseInt("55", 10) == 55);
	TEST_ASSERT(parseInt("100") == 100);
	TEST_ASSERT(parseInt("-1", 16) == -1);
	TEST_ASSERT(parseInt("-FF", 16) == -0xFF);
	TEST_ASSERT(parseInt("ZEW", 36) == 45896);
	TEST_ASSERT(parseInt("+ZEx", 36) == 45897);
	TEST_ASSERT(parseInt("11111110", 2) == 254);
	TEST_ASSERT(parseInt("111133") == 111133);
	TEST_ASSERT(parseInt("0x7FFFFFFF") == 0x7FFFFFFF);
	TEST_ASSERT(parseInt("7FFfFfFF", 16) == 0x7FFFFFFF);
	TEST_ASSERT(parseInt("-0x80000000") == -0x7FFFFFFF-1);
	TEST_ASSERT(parseInt("2147483639") == 2147483639);
	TEST_ASSERT(parseInt("Kona", 27) == 411787);
	TEST_ASSERT(parseInt("12gb", 35) == 45896);
	TEST_ASSERT(parseInt("-10000000000000000000000000000000", 2) == -0x7FFFFFFF-1);

	if (BitCount<INT_MAX>::result == 31) {
		TEST_THROWS(parseInt("2147483649"), OverflowException &);
		TEST_THROWS(parseInt("80000000", 16), OverflowException &);
		TEST_THROWS(parseInt("-80000001", 16), UnderflowException &);
		TEST_THROWS(parseInt("13242378217387127812"), OverflowException &);
		TEST_THROWS(parseInt("-463724632764722"), UnderflowException &);
	}
	TEST_THROWS(parseInt("ZEW", 35), ParseException &);
	TEST_THROWS(parseInt("ZEW", 1), InvalidArgumentException &);
	TEST_THROWS(parseInt("+", 16), ParseException &);
	TEST_THROWS(parseInt("-", 10), ParseException &);
	TEST_THROWS(parseInt("U", 16), ParseException &);
	TEST_THROWS(parseInt("", 8), ParseException &);
	TEST_THROWS(parseInt("0Y10"), ParseException &);
	TEST_THROWS(parseInt("0p10"), ParseException &);
	TEST_THROWS(parseInt("0x"), ParseException &);
	TEST_THROWS(parseInt("0X"), ParseException &);
	TEST_THROWS(parseInt("11111110&", 2), ParseException &);
	TEST_THROWS(parseInt("1111^1110&"), ParseException &);
	TEST_THROWS(parseInt("!11111110", 2), ParseException &);

	TEST_ASSERT(parseInt("0") == 0);
	TEST_ASSERT(parseInt("010", 10) == 10);
	TEST_ASSERT(parseInt("10") == 10);
	TEST_ASSERT(parseInt("010") == 8);
	TEST_ASSERT(parseInt("0x10") == 16);
	TEST_ASSERT(parseInt("0X10") == 16);
	TEST_ASSERT(parseInt("0x10") == 16);
}

void afc::NumberTest::testParseUInt()
{
	// TODO add edge test cases

	// default behaviour (base is not specified)
	TEST_ASSERT(parseUInt("0") == 0);
	TEST_ASSERT(parseUInt("1") == 1);
	TEST_ASSERT(parseUInt("1223213") == 1223213);
	TEST_ASSERT(parseUInt("3876540129") == 3876540129u);
	TEST_ASSERT(parseUInt("056") == 46);
	TEST_ASSERT(parseUInt("56") == 56);
	TEST_ASSERT(parseUInt("0x56") == 86);
	TEST_ASSERT(parseUInt("0x5F") == 95);
	TEST_ASSERT(parseUInt("0x5e") == 94);
	TEST_ASSERT(parseUInt("0xa") == 10);
	TEST_ASSERT(parseUInt("0xf34E8b") == 0xf34e8b);

	switch (static_cast<int>(BitCount<UINT_MAX>::result)) {
	case 32:
		TEST_ASSERT(parseUInt("0xffffffff") == UINT_MAX);
		TEST_ASSERT(parseUInt("4294967295") == UINT_MAX);
		TEST_ASSERT(parseUInt("4294967294") == UINT_MAX-1);
		TEST_ASSERT(parseUInt("037777777777") == UINT_MAX);
		TEST_ASSERT(parseUInt("037777777775") == UINT_MAX-2);
		TEST_THROWS(parseUInt("0x100000000"), OverflowException&);
		TEST_THROWS(parseUInt("4294967296"), OverflowException&);
		TEST_THROWS(parseUInt("4294967299"), OverflowException&);
		TEST_THROWS(parseUInt("040000000000"), OverflowException&);
		TEST_THROWS(parseUInt("040000000004"), OverflowException&);
		break;
	case 64:
		TEST_ASSERT(parseUInt("0xffffffffffffffff") == UINT_MAX);
		TEST_ASSERT(parseUInt("18446744073709551615") == UINT_MAX);
		TEST_ASSERT(parseUInt("18446744073709551614") == UINT_MAX-1);
		TEST_ASSERT(parseUInt("1777777777777777777777") == UINT_MAX);
		TEST_ASSERT(parseUInt("1777777777777777777775") == UINT_MAX-2);
		TEST_THROWS(parseUInt("0x10000000000000000"), OverflowException&);
		TEST_THROWS(parseUInt("18446744073709551616"), OverflowException&);
		TEST_THROWS(parseUInt("18446744073709551619"), OverflowException&);
		TEST_THROWS(parseUInt("02000000000000000000000"), OverflowException&);
		TEST_THROWS(parseUInt("02000000000000000000004"), OverflowException&);
		break;
	}

	// default behaviour (base is specified)
	for (unsigned i = 4; i < number_limits::MAX_BASE; ++i) {
		TEST_ASSERT(parseUInt("123", i) == 1 * i * i + 2 * i + 3);
	}
	TEST_ASSERT(parseUInt("Hello", 36) == 17*36*36*36*36 + 14*36*36*36 + 21*36*36 + 21*36 + 24);
	TEST_ASSERT(parseUInt("HelLo", 28) == 17*28*28*28*28 + 14*28*28*28 + 21*28*28 + 21*28 + 24);
	TEST_ASSERT(parseUInt("0100011", 2) == 35);

	// exceptional behaviour (base is not specified)
	TEST_THROWS(parseUInt("+0"), ParseException&);
	TEST_THROWS(parseUInt("-0"), ParseException&);
	TEST_THROWS(parseUInt("-1"), ParseException&);
	TEST_THROWS(parseUInt("245F"), ParseException&);
	TEST_THROWS(parseUInt("a"), ParseException&);
	TEST_THROWS(parseUInt("34e12"), ParseException&);
	TEST_THROWS(parseUInt("*32d@(99"), ParseException&);

	// exceptional behaviour (base is specified)
	TEST_THROWS(parseUInt("123", 0), InvalidArgumentException &);
	TEST_THROWS(parseUInt("123", 1), InvalidArgumentException &);
	TEST_THROWS(parseUInt("123", 2), ParseException &);
	TEST_THROWS(parseUInt("123", 3), ParseException &);
	TEST_THROWS(parseUInt("123", number_limits::MAX_BASE+1), InvalidArgumentException &);
	TEST_THROWS(parseUInt("123", number_limits::MAX_BASE+100), InvalidArgumentException &);
	TEST_THROWS(parseUInt("+324", 5), ParseException &);
	TEST_THROWS(parseUInt("-0", 34), ParseException &);
	TEST_THROWS(parseUInt("-d2891JJd", 29), ParseException &);
	TEST_THROWS(parseUInt("245F", 10), ParseException &);
	TEST_THROWS(parseUInt("a", 2), ParseException &);
	TEST_THROWS(parseUInt("34e12", 10), ParseException &);
	TEST_THROWS(parseUInt("*32d@(99", 35), ParseException &);
}

void afc::NumberTest::testAppendToString()
{
	{
		string result;
		appendToString(123, 10, result);
		TEST_ASSERT(result == "123");
		appendToString(-456, 10, result);
		TEST_ASSERT(result == "123-456");
	}
	{
		string result;
		appendToString(123, result);
		TEST_ASSERT(result == "123");
		appendToString(-456, result);
		TEST_ASSERT(result == "123-456");
		appendToString(0, result);
		TEST_ASSERT(result == "123-4560");
	}
	{
		string result("aaa");
		appendToString(411787, 10, result);
		TEST_ASSERT(result == "aaa411787");
		appendToString(411787, 27, result);
		TEST_ASSERT(result == "aaa411787kona");
		appendToString(45896, 36, result);
		TEST_ASSERT(result == "aaa411787konazew");
		appendToString(45896, 35, result);
		TEST_ASSERT(result == "aaa411787konazew12gb");
	}
	{
		string result("aaa");
		TEST_THROWS(appendToString(234, 1, result), InvalidArgumentException &);
		TEST_ASSERT(result == "aaa");
		TEST_THROWS(appendToString(234, 0, result), InvalidArgumentException &);
		TEST_ASSERT(result == "aaa");
		TEST_THROWS(appendToString(234, 37, result), InvalidArgumentException &);
		TEST_ASSERT(result == "aaa");
		TEST_THROWS(appendToString(234, UINT_MAX, result), InvalidArgumentException &);
		TEST_ASSERT(result == "aaa");
	}
	{
		string s;
		s += -23;
		s += +56454;
		s += -0xFF;
		TEST_ASSERT(s == "-2356454-255");
	}
	{
		TEST_ASSERT(string("qwe") + 567 + "eee" + 90 + 12 == "qwe567eee9012");
	}
	{
		string result;
		appendToString(static_cast<int>(-0x80000000), 2, result);
		TEST_ASSERT(result == "-10000000000000000000000000000000");
	}
	{
		string result;
		appendToString(-20, result);
		TEST_ASSERT(result == "-20");
	}

	/*{
	int result;
	Stopwatch c;
	string s("32142");
	c.start();
	for (int i = 0; i < 100000000; ++i) {
		result += parseInt(s);
	}
	c.stop().print();
	}
	{
		int result;
		Stopwatch c;
		c.start();
		for (int i = 0; i < 100000000; ++i) {
			result += atoi("32142");
		}
		c.stop().print();
	}
	{
	int result;
	Stopwatch c;
	string s("32142");
	c.start();
	for (int i = 0; i < 100000000; ++i) {
		result += parseUInt(s);
	}
	c.stop().print();
	}*/
}

void afc::NumberTest::testToStringInt()
{
	TEST_ASSERT(toString(1234309) == "1234309");
	TEST_ASSERT(toString(0xFF, 2) == "11111111");
	TEST_ASSERT(toString(-0) == "0");
	TEST_ASSERT(toString(1, 36) == "1");
	TEST_ASSERT(toString(-1, 36) == "-1");
	TEST_ASSERT(toString(-35*36-34, 36) == "-zy");
	
	TEST_THROWS(toString(1, 0), InvalidArgumentException &);
	TEST_THROWS(toString(1, 1), InvalidArgumentException &);
	TEST_THROWS(toString(1, 37), InvalidArgumentException &);
	TEST_THROWS(toString(1, 256), InvalidArgumentException &);
	TEST_THROWS(toString(1, 258), InvalidArgumentException &);
}
