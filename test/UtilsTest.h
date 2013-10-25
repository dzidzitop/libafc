#ifndef AFC_UTILSTEST_H_
#define AFC_UTILSTEST_H_

#include <cpptest.h>

namespace afc
{
	struct UtilsTest : public Test::Suite
	{
		UtilsTest();

		// TODO move this to a separate test case.
		void testStartsWith();
		// TODO move this to a separate test case.
		void testEndsWith();
		void testBinarySearch();
	};
}

#endif /*AFC_UTILSTEST_H_*/
