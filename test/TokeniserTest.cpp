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
#include "TokeniserTest.hpp"
#include <afc/Tokeniser.hpp>
#include <afc/Exception.h>
#include <string>
#include <utility>

using std::string;
using std::wstring;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::TokeniserTest);

void afc::TokeniserTest::testEmptyInputString()
{
	std::string s;
	std::pair<std::string::iterator, std::string::iterator> p;
	afc::Tokeniser<char, std::string::iterator> t(s.begin(), s.end(), '-');
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string(""), string(p.first, p.second));
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::Exception);
}

void afc::TokeniserTest::testSingleToken()
{
	std::string s("abcde");
	std::pair<std::string::iterator, std::string::iterator> p;
	afc::Tokeniser<char, std::string::iterator> t(s.begin(), s.end(), '-');
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("abcde"), string(p.first, p.second));
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::Exception);
}

void afc::TokeniserTest::testMultipleTokens()
{
	std::string s("ab+c+d-e");
	std::pair<std::string::iterator, std::string::iterator> p;
	afc::Tokeniser<char, std::string::iterator> t(s.begin(), s.end(), '+');
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("ab"), string(p.first, p.second));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("c"), string(p.first, p.second));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("d-e"), string(p.first, p.second));
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::Exception);
}

void afc::TokeniserTest::testMultipleTokensWithEmptyToken()
{
	std::string s("ab--de");
	std::pair<std::string::iterator, std::string::iterator> p;
	afc::Tokeniser<char, std::string::iterator> t(s.begin(), s.end(), '-');
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("ab"), string(p.first, p.second));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string(""), string(p.first, p.second));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("de"), string(p.first, p.second));
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::Exception);
}

void afc::TokeniserTest::testOnlyEmptyTokens()
{
	std::string s("--");
	std::pair<std::string::iterator, std::string::iterator> p;
	afc::Tokeniser<char, std::string::iterator> t(s.begin(), s.end(), '-');
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string(""), string(p.first, p.second));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string(""), string(p.first, p.second));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string(""), string(p.first, p.second));
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::Exception);
}

void afc::TokeniserTest::testWideStringMultipleTokens()
{
	std::wstring s(L"doing all right");
	std::pair<std::wstring::iterator, std::wstring::iterator> p;
	afc::Tokeniser<wchar_t, std::wstring::iterator> t(s.begin(), s.end(), L' ');
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT(wstring(p.first, p.second) == wstring(L"doing"));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT(wstring(p.first, p.second) == wstring(L"all"));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT(wstring(p.first, p.second) == wstring(L"right"));
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::Exception);
}

void afc::TokeniserTest::testInputIsNotRvalue()
{
	std::string s("doing all right");
	std::pair<std::string::iterator, std::string::iterator> p;
	afc::Tokeniser<char, std::string::iterator> t(s.begin(), s.end(), ' ');
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("doing"), string(p.first, p.second));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("all"), string(p.first, p.second));
	CPPUNIT_ASSERT(t.hasNext());
	p = t.next();
	CPPUNIT_ASSERT_EQUAL(string("right"), string(p.first, p.second));
	CPPUNIT_ASSERT(!t.hasNext());
	CPPUNIT_ASSERT_THROW(t.next(), afc::Exception);
}
