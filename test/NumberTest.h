#ifndef AFC_NUMBERTEST_H_
#define AFC_NUMBERTEST_H_

#include <cpptest.h>

namespace afc
{
	struct NumberTest : public Test::Suite
	{
		NumberTest();

		void testParseInt();
		void testParseUInt();
		void testAppendToStringInt();
		void testToStringInt();
	};
}

#endif /*AFC_NUMBERTEST_H_*/
