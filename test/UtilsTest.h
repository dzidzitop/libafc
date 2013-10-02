#ifndef AFC_UTILSTEST_H_
#define AFC_UTILSTEST_H_

#include <cpptest.h>

namespace afc
{
	struct UtilsTest : public Test::Suite
	{
		UtilsTest();

		void testConstants();
		void testTrim();
		void testStartsWith();
		void testEndsWith();
		void testBinarySearch();
	};
}

#endif /*AFC_UTILSTEST_H_*/
