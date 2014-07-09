/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2014 Dźmitry Laŭčuk

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
#include "TokeniserTest.h"
#include <afc/Tokeniser.hpp>
#include <afc/Exception.h>
#include <string>

using std::string;
using std::wstring;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::TokeniserTest);

void afc::TokeniserTest::testEmptyInputString()
{
	afc::Tokeniser<string> t("", '-');
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string(""), t.next());
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testSingleToken()
{
	afc::Tokeniser<string> t("abcde", '-');
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("abcde"), t.next());
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testMultipleTokens()
{
	afc::Tokeniser<string> t("ab+c+d-e", '+');
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("ab"), t.next());
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("c"), t.next());
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("d-e"), t.next());
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testMultipleTokensWithEmptyToken()
{
	afc::Tokeniser<string> t("ab--de", '-');
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("ab"), t.next());
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string(""), t.next());
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("de"), t.next());
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testOnlyEmptyTokens()
{
	afc::Tokeniser<string> t("--", '-');
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string(""), t.next());
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string(""), t.next());
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string(""), t.next());
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testWideStringMultipleTokens()
{
	afc::Tokeniser<wstring> t(L"doing all right", L' ');
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT(t.next() == wstring(L"doing"));
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT(t.next() == wstring(L"all"));
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT(t.next() == wstring(L"right"));
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::IllegalStateException);
}

void afc::TokeniserTest::testInputIsNotRvalue()
{
	string str("doing all right");
	afc::Tokeniser<string> t(str, ' ');
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("doing"), t.next());
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("all"), t.next());
	CPPUNIT_ASSERT(t.hasNext());
	CPPUNIT_ASSERT_EQUAL(string("right"), t.next());
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::IllegalStateException);
}
