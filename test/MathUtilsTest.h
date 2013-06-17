#ifndef AFC_MATHUTILSTEST_H_
#define AFC_MATHUTILSTEST_H_

#include <cpptest.h>

namespace afc
{
	struct MathUtilsTest : public Test::Suite
	{
		MathUtilsTest();

		void testSameSign();
		void testIsEven();
		void testIsOdd();
		void testMean();
	};
}

#endif /*AFC_MATHUTILSTEST_H_*/
