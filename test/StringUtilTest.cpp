#include "StringUtilTest.hpp"
#include <afc/string_util.hpp>

using std::string;

afc::StringUtilTest::StringUtilTest()
{
	TEST_ADD(StringUtilTest::testStartsWith);
	TEST_ADD(StringUtilTest::testEndsWith);
}

void afc::StringUtilTest::testStartsWith()
{
	TEST_ASSERT(startsWith(string("str"), string("s")));
	TEST_ASSERT(startsWith(string("str"), string("st")));
	TEST_ASSERT(startsWith(string("str"), string("str")));
	TEST_ASSERT(!startsWith(string("str"), string("str2")));
	TEST_ASSERT(!startsWith(string("str"), string("sTr")));
	TEST_ASSERT(!startsWith(string("str"), string("S")));
	TEST_ASSERT(startsWith(string("str"), string("")));
	TEST_ASSERT(startsWith(string(""), string("")));
	TEST_ASSERT(!startsWith(string(""), string("sWW")));
	TEST_ASSERT(!startsWith(string("z"), string("s")));
	TEST_ASSERT(!startsWith(string("z"), string("zz")));
	TEST_ASSERT(startsWith(string("z"), string("z")));
	TEST_ASSERT(startsWith(string("z"), string("")));
}

void afc::StringUtilTest::testEndsWith()
{
	TEST_ASSERT(endsWith(string("str"), string("r")));
	TEST_ASSERT(endsWith(string("str"), string("tr")));
	TEST_ASSERT(endsWith(string("str"), string("str")));
	TEST_ASSERT(endsWith(string("str"), string("")));
	TEST_ASSERT(!endsWith(string("str"), string("rt")));
	TEST_ASSERT(!endsWith(string("str"), string("astr")));
	TEST_ASSERT(endsWith(string("s"), string("")));
	TEST_ASSERT(endsWith(string("s"), string("s")));
	TEST_ASSERT(!endsWith(string("s"), string("z")));
	TEST_ASSERT(!endsWith(string("s"), string("ss")));
	TEST_ASSERT(endsWith(string(""), string("")));
	TEST_ASSERT(!endsWith(string(""), string("z")));
	TEST_ASSERT(!endsWith(string(""), string("ss")));
}
