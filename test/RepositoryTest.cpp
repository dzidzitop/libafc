/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2014 Dźmitry Laŭčuk

libafc is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#include "RepositoryTest.h"
#include <afc/Repository.h>
#include <string>
#include <cstddef>

using std::string;
using std::size_t;

void afc::RepositoryTest::testIntRepository()
{
	Repository<int> rep;

	CPPUNIT_ASSERT_EQUAL(size_t(0), rep.size());
	CPPUNIT_ASSERT(rep.empty());

	const int &i = rep.get(5);

	CPPUNIT_ASSERT_EQUAL(size_t(1), rep.size());
	CPPUNIT_ASSERT(!rep.empty());

	const int &j = rep.get(5);

	CPPUNIT_ASSERT_EQUAL(size_t(1), rep.size());

	const int &k = rep.get(6);
	const int &l = rep.get(7);
	const int &m = rep.get(6);
	const int &n = rep.get(5);

	CPPUNIT_ASSERT_EQUAL(size_t(3), rep.size());
	CPPUNIT_ASSERT(!rep.empty());
	CPPUNIT_ASSERT_EQUAL(5, i);
	CPPUNIT_ASSERT_EQUAL(5, j);
	CPPUNIT_ASSERT_EQUAL(&j, &i);
	CPPUNIT_ASSERT_EQUAL(5, n);
	CPPUNIT_ASSERT_EQUAL(&i, &n);
	CPPUNIT_ASSERT_EQUAL(6, k);
	CPPUNIT_ASSERT_EQUAL(6, m);
	CPPUNIT_ASSERT_EQUAL(&m, &k);
	CPPUNIT_ASSERT_EQUAL(7, l);

	rep.clear();
	CPPUNIT_ASSERT_EQUAL(size_t(0), rep.size());
	CPPUNIT_ASSERT(rep.empty());
}

void afc::RepositoryTest::testStringRepository()
{
	Repository<string> rep;

	CPPUNIT_ASSERT_EQUAL(size_t(0), rep.size());
	CPPUNIT_ASSERT(rep.empty());

	const string &i = rep.get("hello");

	CPPUNIT_ASSERT_EQUAL(size_t(1), rep.size());
	CPPUNIT_ASSERT(!rep.empty());

	const string &j = rep.get("hello");

	CPPUNIT_ASSERT_EQUAL(size_t(1), rep.size());

	const string &k = rep.get("");
	const string &l = rep.get("world");
	const string &m = rep.get("");
	const string &n = rep.get("hello");

	CPPUNIT_ASSERT_EQUAL(size_t(3), rep.size());
	CPPUNIT_ASSERT(!rep.empty());
	CPPUNIT_ASSERT_EQUAL(string("hello"), i);
	CPPUNIT_ASSERT_EQUAL(string("hello"), j);
	CPPUNIT_ASSERT_EQUAL(&j, &i);
	CPPUNIT_ASSERT_EQUAL(string("hello"), n);
	CPPUNIT_ASSERT_EQUAL(&i, &n);
	CPPUNIT_ASSERT_EQUAL(string(), k);
	CPPUNIT_ASSERT_EQUAL(string(), m);
	CPPUNIT_ASSERT_EQUAL(&m, &k);
	CPPUNIT_ASSERT_EQUAL(string("world"), l);

	CPPUNIT_ASSERT(!rep.remove("World"));
	CPPUNIT_ASSERT_EQUAL(size_t(3), rep.size());
	CPPUNIT_ASSERT(!rep.empty());

	CPPUNIT_ASSERT(rep.remove("hello"));
	CPPUNIT_ASSERT_EQUAL(size_t(2), rep.size());
	CPPUNIT_ASSERT(!rep.empty());
	CPPUNIT_ASSERT(rep.remove("world"));
	CPPUNIT_ASSERT_EQUAL(size_t(1), rep.size());
	CPPUNIT_ASSERT(!rep.empty());
	CPPUNIT_ASSERT(rep.remove(""));
	CPPUNIT_ASSERT_EQUAL(size_t(0), rep.size());
	CPPUNIT_ASSERT(rep.empty());
	CPPUNIT_ASSERT(!rep.remove("h"));
	CPPUNIT_ASSERT_EQUAL(size_t(0), rep.size());
	CPPUNIT_ASSERT(rep.empty());
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

	CPPUNIT_ASSERT_EQUAL(size_t(4), rep.size());
	CPPUNIT_ASSERT_EQUAL(string("hey"), s1);
	CPPUNIT_ASSERT_EQUAL(string("hey"), s2);
	CPPUNIT_ASSERT_EQUAL(string("mein"), s3);
	CPPUNIT_ASSERT_EQUAL(string(), s4);
	CPPUNIT_ASSERT_EQUAL(string("mein"), s5);
	CPPUNIT_ASSERT_EQUAL(string(), s6);
	CPPUNIT_ASSERT_EQUAL(string("world"), s7);
	CPPUNIT_ASSERT_EQUAL(&s2, &s1);
	CPPUNIT_ASSERT_EQUAL(&s5, &s3);
	CPPUNIT_ASSERT_EQUAL(&s6, &s4);

	rep.remove("stop");

	CPPUNIT_ASSERT_EQUAL(size_t(3), rep.size());

	const string &s8 = rep.get("dawn");
	const string &s9 = rep.get("oppo");

	CPPUNIT_ASSERT_EQUAL(string("dawn"), s8);
	CPPUNIT_ASSERT_EQUAL(string("dawn"), s9);
	CPPUNIT_ASSERT_EQUAL(&s9, &s8);

	const string &s10 = rep.get("   ");

	CPPUNIT_ASSERT_EQUAL(string("hey"), s10);
	CPPUNIT_ASSERT_EQUAL(&s10, &s1);
}
