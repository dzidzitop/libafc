#ifndef AFC_REPOSITORYTEST_H_
#define AFC_REPOSITORYTEST_H_

#include <cpptest.h>

namespace afc
{
	struct RepositoryTest : public Test::Suite
	{
		RepositoryTest();

		void testIntRepository();
		void testStringRepository();
		void testCustomComparator();
	};
}

#endif /*AFC_REPOSITORYTEST_H_*/
