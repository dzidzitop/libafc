#include "UtilsTest.h"
#include <afc/string_util.hpp>

using std::string;
using std::vector;

afc::UtilsTest::UtilsTest()
{
	TEST_ADD(UtilsTest::testStartsWith);
	TEST_ADD(UtilsTest::testEndsWith);
}

void afc::UtilsTest::testStartsWith()
{
	TEST_ASSERT(startsWith("str", "s"));
	TEST_ASSERT(startsWith("str", "st"));
	TEST_ASSERT(startsWith("str", "str"));
	TEST_ASSERT(!startsWith("str", "str2"));
	TEST_ASSERT(!startsWith("str", "sTr"));
	TEST_ASSERT(!startsWith("str", "S"));
	TEST_ASSERT(startsWith("str", ""));
	TEST_ASSERT(startsWith("", ""));
	TEST_ASSERT(!startsWith("", "sWW"));
	TEST_ASSERT(!startsWith("z", "s"));
	TEST_ASSERT(!startsWith("z", "zz"));
	TEST_ASSERT(startsWith("z", "z"));
	TEST_ASSERT(startsWith("z", ""));
}

void afc::UtilsTest::testEndsWith()
{
	TEST_ASSERT(endsWith("str", "r"));
	TEST_ASSERT(endsWith("str", "tr"));
	TEST_ASSERT(endsWith("str", "str"));
	TEST_ASSERT(endsWith("str", ""));
	TEST_ASSERT(!endsWith("str", "rt"));
	TEST_ASSERT(!endsWith("str", "astr"));
	TEST_ASSERT(endsWith("s", ""));
	TEST_ASSERT(endsWith("s", "s"));
	TEST_ASSERT(!endsWith("s", "z"));
	TEST_ASSERT(!endsWith("s", "ss"));
	TEST_ASSERT(endsWith("", ""));
	TEST_ASSERT(!endsWith("", "z"));
	TEST_ASSERT(!endsWith("", "ss"));
}
