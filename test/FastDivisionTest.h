#ifndef AFC_FASTDIVISIONTEST_H_
#define AFC_FASTDIVISIONTEST_H_

#include <cpptest.h>

namespace afc
{
	// TODO add more tests
	struct FastDivisionTest : public Test::Suite
	{
		FastDivisionTest();
		
		void testDivideBy10();
	};
}

#endif /*AFC_FASTDIVISIONTEST_H_*/
