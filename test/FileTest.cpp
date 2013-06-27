#include "FileTest.h"
#include <afc/File.h>

afc::FileTest::FileTest()
{
	TEST_ADD(FileTest::testCharPtrConstructor);
	TEST_ADD(FileTest::testCharPtrConstructor_EmptyPath);
	TEST_ADD(FileTest::testStringConstructor_EmptyPath);
	TEST_ADD(FileTest::testStringConstructor_AbsolutePathWithoutSeparators);
	TEST_ADD(FileTest::testStringConstructor_AbsolutePathWithSeparators);
	TEST_ADD(FileTest::testFileParentStringConstructor);
}

void afc::FileTest::testCharPtrConstructor()
{
	// TODO add more tests
	const File f("/hello/world/youyou_hey");
	TEST_ASSERT(f.path() == "/hello/world/youyou_hey");
	TEST_ASSERT_MSG(f.name() == "youyou_hey", f.name().c_str());
	TEST_ASSERT(&f.path() == &f.path());
	TEST_ASSERT(&f.name() == &f.name());

	const File parent = f.parent();
	TEST_ASSERT(parent.path() == "/hello/world");
	TEST_ASSERT(parent.name() == "world");
}

void afc::FileTest::testCharPtrConstructor_EmptyPath()
{
	const File f("");
	TEST_ASSERT_MSG(f.path().empty(), f.path().c_str());
	TEST_ASSERT_MSG(f.name().empty(), f.name().c_str());
	TEST_ASSERT(&f.path() == &f.path());
	TEST_ASSERT(&f.name() == &f.name());

	const File parent = f.parent();
	TEST_ASSERT(parent.path().empty());
	TEST_ASSERT(parent.name().empty());
}

void afc::FileTest::testStringConstructor_AbsolutePathWithSeparators()
{
	const File f(string("/hello/world/youyou_hey"));
	TEST_ASSERT(f.path() == "/hello/world/youyou_hey");
	TEST_ASSERT_MSG(f.name() == "youyou_hey", f.name().c_str());
	TEST_ASSERT(&f.path() == &f.path());
	TEST_ASSERT(&f.name() == &f.name());

	const File parent = f.parent();
	TEST_ASSERT(parent.path() == "/hello/world");
	TEST_ASSERT(parent.name() == "world");
}

void afc::FileTest::testStringConstructor_AbsolutePathWithoutSeparators()
{
	const File f(string("/hello"));
	TEST_ASSERT(f.path() == "/hello");
	TEST_ASSERT_MSG(f.name() == "hello", f.name().c_str());
	TEST_ASSERT(&f.path() == &f.path());
	TEST_ASSERT(&f.name() == &f.name());

	const File parent = f.parent();
	TEST_ASSERT(parent.path() == "/");
	TEST_ASSERT(parent.name() == "/");
}

void afc::FileTest::testStringConstructor_EmptyPath()
{
	const File f(string(""));
	TEST_ASSERT_MSG(f.path().empty(), f.path().c_str());
	TEST_ASSERT_MSG(f.name().empty(), f.name().c_str());
	TEST_ASSERT(&f.path() == &f.path());
	TEST_ASSERT(&f.name() == &f.name());

	const File parent = f.parent();
	TEST_ASSERT(parent.path().empty());
	TEST_ASSERT(parent.name().empty());
}

void afc::FileTest::testFileParentStringConstructor()
{
	// TODO add some tests
}
