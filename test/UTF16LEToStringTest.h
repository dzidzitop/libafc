#ifndef AFC_UTF16LETOSTRINGTEST_H_
#define AFC_UTF16LETOSTRINGTEST_H_

#include <cpptest.h>

namespace afc
{
	// TODO add tests with surrogate pairs
	// TODO add tests for error cases
	struct UTF16LEToStringTest : public Test::Suite
	{
		UTF16LEToStringTest();

		void testEmptyString_toASCII();
		void testNonEmptyString_toASCII();
		void testMultiByteNonEmptyString_toASCII_withTransliteration();
		void testEmptyString_toUTF8();
		void testSimpleNonEmptyString_toUTF8();
		void testMultiByteNonEmptyString_toUTF8();
	};
}

#endif /* AFC_UTF16LETOSTRINGTEST_H_ */
