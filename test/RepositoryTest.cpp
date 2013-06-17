#include "RepositoryTest.h"
#include "Repository.h"
#include <string>

using std::string;

afc::RepositoryTest::RepositoryTest()
{
	TEST_ADD(RepositoryTest::testIntRepository);
	TEST_ADD(RepositoryTest::testStringRepository);
	TEST_ADD(RepositoryTest::testCustomComparator);
}

void afc::RepositoryTest::testIntRepository()
{
	Repository<int> rep;

	TEST_ASSERT(rep.size() == 0);
	TEST_ASSERT(rep.empty());

	const int &i = rep.get(5);

	TEST_ASSERT(rep.size() == 1);
	TEST_ASSERT(!rep.empty());

	const int &j = rep.get(5);

	TEST_ASSERT(rep.size() == 1);

	const int &k = rep.get(6);
	const int &l = rep.get(7);
	const int &m = rep.get(6);
	const int &n = rep.get(5);

	TEST_ASSERT(rep.size() == 3);
	TEST_ASSERT(!rep.empty());
	TEST_ASSERT(i == 5);
	TEST_ASSERT(j == 5);
	TEST_ASSERT(&i == &j);
	TEST_ASSERT(n == 5);
	TEST_ASSERT(&n == &i);
	TEST_ASSERT(k == 6);
	TEST_ASSERT(m == 6);
	TEST_ASSERT(&k == &m);
	TEST_ASSERT(l == 7);

	rep.clear();
	TEST_ASSERT(rep.size() == 0);
	TEST_ASSERT(rep.empty());
}

void afc::RepositoryTest::testStringRepository()
{
	Repository<string> rep;

	TEST_ASSERT(rep.size() == 0);
	TEST_ASSERT(rep.empty());

	const string &i = rep.get("hello");

	TEST_ASSERT(rep.size() == 1);
	TEST_ASSERT(!rep.empty());

	const string &j = rep.get("hello");

	TEST_ASSERT(rep.size() == 1);

	const string &k = rep.get("");
	const string &l = rep.get("world");
	const string &m = rep.get("");
	const string &n = rep.get("hello");

	TEST_ASSERT(rep.size() == 3);
	TEST_ASSERT(!rep.empty());
	TEST_ASSERT(i == "hello");
	TEST_ASSERT(j == "hello");
	TEST_ASSERT(&i == &j);
	TEST_ASSERT(n == "hello");
	TEST_ASSERT(&n == &i);
	TEST_ASSERT(k == "");
	TEST_ASSERT(m == "");
	TEST_ASSERT(&k == &m);
	TEST_ASSERT(l == "world");

	TEST_ASSERT(!rep.remove("World"));
	TEST_ASSERT(rep.size() == 3);
	TEST_ASSERT(!rep.empty());

	TEST_ASSERT(rep.remove("hello"));
	TEST_ASSERT(rep.size() == 2);
	TEST_ASSERT(!rep.empty());
	TEST_ASSERT(rep.remove("world"));
	TEST_ASSERT(rep.size() == 1);
	TEST_ASSERT(!rep.empty());
	TEST_ASSERT(rep.remove(""));
	TEST_ASSERT(rep.size() == 0);
	TEST_ASSERT(rep.empty());
	TEST_ASSERT(!rep.remove("h"));
	TEST_ASSERT(rep.size() == 0);
	TEST_ASSERT(rep.empty());
}

namespace
{
	struct StringSizeOnlyLess
	{
		bool operator()(const string &s1, const string &s2) {return s1.size() < s2.size();}
	};
}

void afc::RepositoryTest::testCustomComparator()
{
	Repository<string, StringSizeOnlyLess> rep;

	const string &s1 = rep.get("hey");
	const string &s2 = rep.get("you");
	const string &s3 = rep.get("mein");
	const string &s4 = rep.get("");
	const string &s5 = rep.get("mein");
	const string &s6 = rep.get("");
	const string &s7 = rep.get("world");

	TEST_ASSERT(rep.size() == 4);
	TEST_ASSERT(s1 == "hey");
	TEST_ASSERT(s2 == "hey");
	TEST_ASSERT(s3 == "mein");
	TEST_ASSERT(s4 == "");
	TEST_ASSERT(s5 == "mein");
	TEST_ASSERT(s6 == "");
	TEST_ASSERT(s7 == "world");
	TEST_ASSERT(&s1 == &s2);
	TEST_ASSERT(&s3 == &s5);
	TEST_ASSERT(&s4 == &s6);

	rep.remove("stop");

	TEST_ASSERT(rep.size() == 3);

	const string &s8 = rep.get("dawn");
	const string &s9 = rep.get("oppo");

	TEST_ASSERT(s8 == "dawn");
	TEST_ASSERT(s9 == "dawn");
	TEST_ASSERT(&s8 == &s9);

	const string &s10 = rep.get("   ");

	TEST_ASSERT(s10 == "hey");
	TEST_ASSERT(&s10 == &s1);
}
