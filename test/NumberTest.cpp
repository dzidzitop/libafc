#include "NumberTest.h"
#include <afc/number.h>

using namespace std;

afc::NumberTest::NumberTest()
{
	TEST_ADD(NumberTest::testAppendToStringInt);
}

void afc::NumberTest::testAppendToStringInt()
{
	{
		string result;
		appendToString<int>(123, 10, result);
		TEST_ASSERT(result == "123");
		appendToString<int>(-456, 10, result);
		TEST_ASSERT(result == "123-456");
	}
	{
		string result;
		appendToString<int>(123, result);
		TEST_ASSERT(result == "123");
		appendToString<int>(-456, result);
		TEST_ASSERT(result == "123-456");
		appendToString<int>(0, result);
		TEST_ASSERT(result == "123-4560");
	}
	{
		string result("aaa");
		appendToString<int>(411787, 10, result);
		TEST_ASSERT(result == "aaa411787");
		appendToString<int>(411787, 27, result);
		TEST_ASSERT(result == "aaa411787kona");
		appendToString<int>(45896, 36, result);
		TEST_ASSERT(result == "aaa411787konazew");
		appendToString<int>(45896, 35, result);
		TEST_ASSERT(result == "aaa411787konazew12gb");
	}
	{
		string result("aaa");
		TEST_THROWS(appendToString<int>(234, 1, result), InvalidArgumentException &);
		TEST_ASSERT(result == "aaa");
		TEST_THROWS(appendToString<int>(234, 0, result), InvalidArgumentException &);
		TEST_ASSERT(result == "aaa");
		TEST_THROWS(appendToString<int>(234, 37, result), InvalidArgumentException &);
		TEST_ASSERT(result == "aaa");
		TEST_THROWS(appendToString<int>(234, 255, result), InvalidArgumentException &);
		TEST_ASSERT(result == "aaa");
	}
	{
		string result;
		appendToString<int>(static_cast<int>(-0x80000000), 2, result);
		TEST_ASSERT(result == "-10000000000000000000000000000000");
	}
	{
		string result;
		appendToString(-20, result);
		TEST_ASSERT(result == "-20");
	}
}
