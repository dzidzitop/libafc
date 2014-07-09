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
#include "FastDivisionTest.hpp"
#include <afc/fast_division.h>
#include <climits>

CPPUNIT_TEST_SUITE_REGISTRATION(afc::FastDivisionTest);

void afc::FastDivisionTest::testDivideBy10()
{
	for (unsigned i = 0; i < 1026u; ++i) {
		CPPUNIT_ASSERT_EQUAL(i/10u, divide<10>(i));
	}
	CPPUNIT_ASSERT_EQUAL(123142343u, divide<10>(1231423432));
	CPPUNIT_ASSERT_EQUAL(3423323u, divide<10>(34233232));
	CPPUNIT_ASSERT_EQUAL(UINT_MAX/10u, divide<10>(UINT_MAX));
	CPPUNIT_ASSERT_EQUAL((UINT_MAX-1)/10u, divide<10>(UINT_MAX-1));
	// some kind of random numbers
	for (unsigned i = 12312; i < UINT_MAX/10; i += UINT_MAX/12343) {
		CPPUNIT_ASSERT_EQUAL(i/10u, divide<10>(i));
	}
}
