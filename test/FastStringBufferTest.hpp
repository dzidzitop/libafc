/* libafc - utils to facilitate C++ development.
Copyright (C) 2014 Dźmitry Laŭčuk

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
#ifndef AFC_FASTSTRINGBUFFERTEST_HPP_
#define AFC_FASTSTRINGBUFFERTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class FastStringBufferTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(FastStringBufferTest);
		CPPUNIT_TEST(testNextStorageSize);
		CPPUNIT_TEST(testMaxSize);

		CPPUNIT_TEST(testChar_EmptyBuffer_SizeFirst);
		CPPUNIT_TEST(testChar_EmptyBuffer_CapacityFirst);
		CPPUNIT_TEST(testChar_EmptyBuffer_CStrFirst);
		CPPUNIT_TEST(testChar_EmptyBuffer_ReserveEmptySpace);
		CPPUNIT_TEST(testChar_EmptyBuffer_ReserveNonEmptySpace);
		CPPUNIT_TEST(testChar_EmptyBuffer_ReserveSpace_TryToReduceCapacity);
		CPPUNIT_TEST(testChar_EmptyBuffer_ReserveSpace_TryToReduceCapacityToZero);
		CPPUNIT_TEST(testChar_EmptyBuffer_ReserveSpace_ThenReserveSameSpace);

		CPPUNIT_TEST(testChar_AppendCharArray_SingleAppend_EmptyArray);
		CPPUNIT_TEST(testChar_AppendCharArray_SingleAppend_NonEmptyArray);
		CPPUNIT_TEST(testChar_AppendCharArray_MultipleAppends);
		CPPUNIT_TEST(testChar_AppendCharArray_MultipleAppends_WithEmptyArray);
		CPPUNIT_TEST(testChar_AppendCharArray_MultipleAppends_WithTerminatingChars);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testNextStorageSize();
		void testMaxSize();

		void testChar_EmptyBuffer_SizeFirst();
		void testChar_EmptyBuffer_CapacityFirst();
		void testChar_EmptyBuffer_CStrFirst();
		void testChar_EmptyBuffer_ReserveEmptySpace();
		void testChar_EmptyBuffer_ReserveNonEmptySpace();
		void testChar_EmptyBuffer_ReserveSpace_TryToReduceCapacity();
		void testChar_EmptyBuffer_ReserveSpace_TryToReduceCapacityToZero();
		void testChar_EmptyBuffer_ReserveSpace_ThenReserveSameSpace();

		void testChar_AppendCharArray_SingleAppend_EmptyArray();
		void testChar_AppendCharArray_SingleAppend_NonEmptyArray();
		void testChar_AppendCharArray_MultipleAppends();
		void testChar_AppendCharArray_MultipleAppends_WithEmptyArray();
		void testChar_AppendCharArray_MultipleAppends_WithTerminatingChars();
	};
}

#endif /* AFC_FASTSTRINGBUFFERTEST_HPP_ */
