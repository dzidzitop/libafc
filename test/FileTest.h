#ifndef AFC_FILETEST_H_
#define AFC_FILETEST_H_

#include <cpptest.h>

namespace afc
{
	// TODO add more tests
	struct FileTest : public Test::Suite
	{
		FileTest();

		void testCharPtrConstructor();
		void testCharPtrConstructor_EmptyPath();
		void testStringConstructor_EmptyPath();
		void testStringConstructor_AbsolutePathWithoutSeparators();
		void testStringConstructor_AbsolutePathWithSeparators();
		void testFileParentStringConstructor();
	};
}

#endif /*AFC_FILETEST_H_*/
