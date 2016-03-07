/* libafc - utils to facilitate C++ development.
Copyright (C) 2016 Dźmitry Laŭčuk

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
#ifndef AFC_STRINGTEST_H_
#define AFC_STRINGTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class StringTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(StringTest);
		CPPUNIT_TEST(testCopyConstructor_EmptyString);
		CPPUNIT_TEST(testCopyConstructor_NonEmptyString);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testCopyConstructor_EmptyString();
		void testCopyConstructor_NonEmptyString();
	};
}

#endif /*AFC_STRINGTEST_H_*/
