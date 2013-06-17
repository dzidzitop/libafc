#include "TokeniserTest.h"
#include "../src/utils.h"
#include "Exception.h"

using std::string;

afc::TokeniserTest::TokeniserTest()
{
	TEST_ADD(TokeniserTest::testEmptyInputString);
	TEST_ADD(TokeniserTest::testSingleToken);
	TEST_ADD(TokeniserTest::testMultipleTokens);
	TEST_ADD(TokeniserTest::testMultipleTokensWithEmptyToken);
	TEST_ADD(TokeniserTest::testOnlyEmptyTokens);
	TEST_ADD(TokeniserTest::testWideStringMultipleTokens);
	TEST_ADD(TokeniserTest::testInputIsNotRvalue);
}

void afc::TokeniserTest::testEmptyInputString()
{
	afc::Tokeniser<char> t("", '-');
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "");
	TEST_ASSERT(t.hasNext() == false);
	TEST_THROWS(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testSingleToken()
{
	afc::Tokeniser<char> t("abcde", '-');
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "abcde");
	TEST_ASSERT(t.hasNext() == false);
	TEST_THROWS(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testMultipleTokens()
{
	afc::Tokeniser<char> t("ab+c+d-e", '+');
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "ab");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "c");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "d-e");
	TEST_ASSERT(t.hasNext() == false);
	TEST_THROWS(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testMultipleTokensWithEmptyToken()
{
	afc::Tokeniser<char> t("ab--de", '-');
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "ab");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "de");
	TEST_ASSERT(t.hasNext() == false);
	TEST_THROWS(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testOnlyEmptyTokens()
{
	afc::Tokeniser<char> t("--", '-');
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "");
	TEST_ASSERT(t.hasNext() == false);
	TEST_THROWS(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testWideStringMultipleTokens()
{
	afc::Tokeniser<wchar_t> t(L"doing all right", L' ');
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == L"doing");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == L"all");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == L"right");
	TEST_ASSERT(t.hasNext() == false);
	TEST_THROWS(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testInputIsNotRvalue()
{
	string str("doing all right");
	afc::Tokeniser<char> t(str, ' ');
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "doing");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "all");
	TEST_ASSERT(t.hasNext() == true);
	TEST_ASSERT(t.next() == "right");
	TEST_ASSERT(t.hasNext() == false);
	TEST_THROWS(t.next(), afc::IllegalStateException);
}
