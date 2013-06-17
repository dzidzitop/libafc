#ifndef AFC_FILETEST_H_
#define AFC_FILETEST_H_

#include <cpptest.h>

namespace afc
{
	struct FileTest : public Test::Suite
	{
		FileTest();

		void testCharPtrConstructor();
		void testStringConstructor();
		void testFileParentStringConstructor();
	};
}

#endif /*AFC_FILETEST_H_*/
