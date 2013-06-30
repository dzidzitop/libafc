#include "UTF16LEToStringTest.h"
#include <afc/utils.h>
#include <string>

using namespace std;
using namespace afc;

afc::UTF16LEToStringTest::UTF16LEToStringTest()
{
	TEST_ADD(UTF16LEToStringTest::testEmptyString_toASCII);
	TEST_ADD(UTF16LEToStringTest::testNonEmptyString_toASCII);
	TEST_ADD(UTF16LEToStringTest::testEmptyString_toUTF8);
	TEST_ADD(UTF16LEToStringTest::testSimpleNonEmptyString_toUTF8);
	TEST_ADD(UTF16LEToStringTest::testMultiByteNonEmptyString_toUTF8);
}

void afc::UTF16LEToStringTest::testEmptyString_toASCII()
{
	string s(utf16leToString(u16string(), "ascii"));

	TEST_ASSERT_MSG(s == "", s.c_str());
}

void afc::UTF16LEToStringTest::testNonEmptyString_toASCII()
{
	string s(utf16leToString(u16string(u"Hello, World!"), "ascii"));

	TEST_ASSERT_MSG(s == "Hello, World!", s.c_str());
}

void afc::UTF16LEToStringTest::testEmptyString_toUTF8()
{
	string s(utf16leToString(u16string(), "UTF-8"));

	TEST_ASSERT_MSG(s == u8"", s.c_str());
}

void afc::UTF16LEToStringTest::testSimpleNonEmptyString_toUTF8()
{
	string s(utf16leToString(u16string(u"Hello, World!"), "UTF-8"));

	TEST_ASSERT_MSG(s == u8"Hello, World!", s.c_str());
}

void afc::UTF16LEToStringTest::testMultiByteNonEmptyString_toUTF8()
{
	string s(utf16leToString(u16string(u"\u0160\u010da\u015bcie"), "UTF-8"));

	TEST_ASSERT_MSG(s == u8"\u0160\u010da\u015bcie", s.c_str());
}
