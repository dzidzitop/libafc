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
#include "FastStringBufferTest.hpp"
#include <afc/FastStringBuffer.hpp>
#include <cstddef>

using afc::FastStringBuffer;
using std::size_t;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::FastStringBufferTest);

void afc::FastStringBufferTest::testChar_EmptyBuffer_SizeFirst()
{
	FastStringBuffer<char> buf;

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	// Ensuring that buf::c_str() does not affect size/capacity.
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());
}

void afc::FastStringBufferTest::testChar_EmptyBuffer_CapacityFirst()
{
	FastStringBuffer<char> buf;

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	// Ensuring that buf::c_str() does not affect size/capacity.
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());
}

void afc::FastStringBufferTest::testChar_EmptyBuffer_CStrFirst()
{
	FastStringBuffer<char> buf;

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	// Ensuring that buf::c_str() does not affect size/capacity.
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());
}

void afc::FastStringBufferTest::testChar_EmptyBuffer_ReserveEmptySpace()
{
	FastStringBuffer<char> buf;
	buf.reserve(0);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());
}

void afc::FastStringBufferTest::testChar_EmptyBuffer_ReserveNonEmptySpace()
{
	FastStringBuffer<char> buf;
	buf.reserve(1);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(1), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve(3);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	const char * const str2 = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str2[0]);
	CPPUNIT_ASSERT_EQUAL(str2, buf.c_str());

	CPPUNIT_ASSERT(str != str2);
}

void afc::FastStringBufferTest::testChar_EmptyBuffer_ReserveSpace_TryToReduceCapacity()
{
	FastStringBuffer<char> buf;
	buf.reserve(3);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve(1);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	// Ensuring that the buffer is neither re-allocated nor modified.
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
}

void afc::FastStringBufferTest::testChar_EmptyBuffer_ReserveSpace_ThenReserveSameSpace()
{
	FastStringBuffer<char> buf;
	buf.reserve(3);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve(3);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	// Ensuring that the buffer is neither re-allocated nor modified.
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
}
