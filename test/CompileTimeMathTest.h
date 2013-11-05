#ifndef AFC_COMPILETIMEMATHTEST_H_
#define AFC_COMPILETIMEMATHTEST_H_

#include <cpptest.h>

namespace afc
{
	struct CompileTimeMathTest : public Test::Suite
	{
		CompileTimeMathTest();

		void testBitCount();
		void testOnesCount();
		void testLeadZeroCount();
		void testLog2();
	};
}

#endif /*AFC_COMPILETIMEMATHTEST_H_*/
