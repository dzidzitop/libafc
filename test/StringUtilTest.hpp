#ifndef AFC_STRINGUTILTEST_H_
#define AFC_STRINGUTILTEST_H_

#include <cpptest.h>

namespace afc
{
	struct StringUtilTest : public Test::Suite
	{
		StringUtilTest();

		void testStartsWith();
		void testEndsWith();
	};
}

#endif /*AFC_STRINGUTILTEST_H_*/
