#include "FileTest.h"
#include <afc/File.h>

afc::FileTest::FileTest()
{
	TEST_ADD(FileTest::testCharPtrConstructor);
	TEST_ADD(FileTest::testStringConstructor);
	TEST_ADD(FileTest::testFileParentStringConstructor);
}

void afc::FileTest::testCharPtrConstructor()
{
	// TODO add more tests
	const File f("/hello/world/youyou_hey");
	TEST_ASSERT(f.path() == "/hello/world/youyou_hey");
	TEST_ASSERT(f.name() == "youyou_hey");
	TEST_ASSERT(&f.path() == &f.path());
	TEST_ASSERT(&f.name() == &f.name());

	File parent = f.parent();
	TEST_ASSERT(parent.path() == "/hello/world");
	TEST_ASSERT(parent.name() == "world");
}

void afc::FileTest::testStringConstructor()
{
	// TODO add some tests
}

void afc::FileTest::testFileParentStringConstructor()
{
	// TODO add some tests
}
