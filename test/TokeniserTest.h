#ifndef AFC_TOKENISERTEST_H_
#define AFC_TOKENISERTEST_H_

#include <cpptest.h>

namespace afc
{
	struct TokeniserTest : public Test::Suite
	{
		TokeniserTest();

		void testEmptyInputString();
		void testSingleToken();
		void testMultipleTokens();
		void testMultipleTokensWithEmptyToken();
		void testOnlyEmptyTokens();
		void testWideStringMultipleTokens();
		void testInputIsNotRvalue();
	};
}

#endif /*AFC_TOKENISERTEST_H_*/
