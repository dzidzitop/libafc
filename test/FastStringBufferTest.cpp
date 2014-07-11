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
#include <string>
#include <afc/StringRef.hpp>
#include <limits>
#include <algorithm>
#include <utility>

using afc::FastStringBuffer;
using std::min;
using std::ptrdiff_t;
using std::size_t;
using std::string;
using afc::operator"" _s;
using std::numeric_limits;

CPPUNIT_TEST_SUITE_REGISTRATION(afc::FastStringBufferTest);

void afc::FastStringBufferTest::testNextStorageSize()
{
	FastStringBuffer<char> buf;

	buf.m_capacity = 0;
	CPPUNIT_ASSERT_EQUAL(size_t(2), buf.nextStorageSize(1));
	CPPUNIT_ASSERT_EQUAL(size_t(4), buf.nextStorageSize(2));
	CPPUNIT_ASSERT_EQUAL(size_t(4), buf.nextStorageSize(3));
	CPPUNIT_ASSERT_EQUAL(size_t(8), buf.nextStorageSize(4));
	CPPUNIT_ASSERT_EQUAL(size_t(8), buf.nextStorageSize(5));

	CPPUNIT_ASSERT_EQUAL(buf.maxSize() + 1, buf.nextStorageSize(buf.maxSize()));
	CPPUNIT_ASSERT_EQUAL(buf.maxSize() + 1, buf.nextStorageSize(buf.maxSize() - 1));
}

void afc::FastStringBufferTest::testMaxSize()
{
	struct S
	{
		char c;
		short s;
	};

	// sizeof(char) == 1 -> max size is max value of ptrsize_t. (The last character is reserved by '\0').
	const size_t expectedCharMaxSize = size_t(numeric_limits<ptrdiff_t>::max());
	const size_t expectedIntMaxSize = min(numeric_limits<size_t>::max() / sizeof(int) - 1,
			size_t(numeric_limits<ptrdiff_t>::max()));
	const size_t expectedStructMaxSize = min(numeric_limits<size_t>::max() / sizeof(S) - 1,
			size_t(numeric_limits<ptrdiff_t>::max()));

	CPPUNIT_ASSERT_EQUAL(expectedCharMaxSize, FastStringBuffer<char>().maxSize());
	CPPUNIT_ASSERT_EQUAL(expectedIntMaxSize, FastStringBuffer<int>().maxSize());
	CPPUNIT_ASSERT_EQUAL(expectedStructMaxSize, FastStringBuffer<S>().maxSize());
}

void afc::FastStringBufferTest::testMoveConstructor_EmptyBuffer()
{
	FastStringBuffer<char> from;

	CPPUNIT_ASSERT_EQUAL(size_t(0), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());

	const char * const str = from.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, from.c_str());

	FastStringBuffer<char> to(std::move(from));

	CPPUNIT_ASSERT_EQUAL(size_t(0), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), to.size());

	const char * const str2 = to.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str2[0]);
	CPPUNIT_ASSERT_EQUAL(str2, to.c_str());

	CPPUNIT_ASSERT_EQUAL(size_t(0), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());

	const char * const str3 = from.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str3[0]);
	CPPUNIT_ASSERT_EQUAL(str3, from.c_str());

	from.reserve(1);

	CPPUNIT_ASSERT_EQUAL(size_t(1), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());

	const char * const str4 = from.c_str();
	CPPUNIT_ASSERT(str4 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str4[0]);
	CPPUNIT_ASSERT_EQUAL(str4, from.c_str());
	// Ensuring new buffer is allocated.
	CPPUNIT_ASSERT(str4 != str3);

	// Ensuring from is to are detached objects.
	CPPUNIT_ASSERT(str4 != to.c_str());
	CPPUNIT_ASSERT_EQUAL(str2, to.c_str());
	CPPUNIT_ASSERT_EQUAL(size_t(0), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), to.size());

	to.reserve(2);
	to.append("a", 1);

	CPPUNIT_ASSERT_EQUAL(size_t(3), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(1), to.size());

	const char * const str5 = to.c_str();
	CPPUNIT_ASSERT(str5 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("a"), string(str5));
	CPPUNIT_ASSERT_EQUAL(str5, to.c_str());

	// Ensuring from is to are detached objects.
	CPPUNIT_ASSERT(str5 != from.c_str());
	CPPUNIT_ASSERT_EQUAL(str4, from.c_str());
	CPPUNIT_ASSERT_EQUAL(size_t(1), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());
}

void afc::FastStringBufferTest::testMoveConstructor_NonEmptyBuffer()
{
	FastStringBuffer<char> from;

	from.reserve(4);
	from.append("abc", 3);

	CPPUNIT_ASSERT_EQUAL(size_t(7), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(3), from.size());

	const char * const str = from.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("abc"), string(str));
	CPPUNIT_ASSERT_EQUAL(str, from.c_str());

	FastStringBuffer<char> to(std::move(from));

	CPPUNIT_ASSERT_EQUAL(size_t(7), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(3), to.size());

	const char * const str2 = to.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("abc"), string(str2));
	CPPUNIT_ASSERT_EQUAL(str2, to.c_str());

	CPPUNIT_ASSERT_EQUAL(size_t(0), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());

	const char * const str3 = from.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str3[0]);
	CPPUNIT_ASSERT_EQUAL(str3, from.c_str());
}

void afc::FastStringBufferTest::testMoveAssignment_FromEmptyToEmpty()
{
	FastStringBuffer<char> from, to;

	to = std::move(from);

	CPPUNIT_ASSERT_EQUAL(size_t(0), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());

	const char * const str3 = from.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str3[0]);
	CPPUNIT_ASSERT_EQUAL(str3, from.c_str());

	CPPUNIT_ASSERT_EQUAL(size_t(0), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), to.size());

	const char * const str2 = to.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str2[0]);
	CPPUNIT_ASSERT_EQUAL(str2, to.c_str());

	from.reserve(1);

	CPPUNIT_ASSERT_EQUAL(size_t(1), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());

	const char * const str4 = from.c_str();
	CPPUNIT_ASSERT(str4 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str4[0]);
	CPPUNIT_ASSERT_EQUAL(str4, from.c_str());
	// Ensuring new buffer is allocated.
	CPPUNIT_ASSERT(str4 != str3);

	// Ensuring from is to are detached objects.
	CPPUNIT_ASSERT(str4 != to.c_str());
	CPPUNIT_ASSERT_EQUAL(str2, to.c_str());
	CPPUNIT_ASSERT_EQUAL(size_t(0), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), to.size());

	to.reserve(2);
	to.append("a", 1);

	CPPUNIT_ASSERT_EQUAL(size_t(3), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(1), to.size());

	const char * const str5 = to.c_str();
	CPPUNIT_ASSERT(str5 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("a"), string(str5));
	CPPUNIT_ASSERT_EQUAL(str5, to.c_str());

	// Ensuring from is to are detached objects.
	CPPUNIT_ASSERT(str5 != from.c_str());
	CPPUNIT_ASSERT_EQUAL(str4, from.c_str());
	CPPUNIT_ASSERT_EQUAL(size_t(1), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());
}

void afc::FastStringBufferTest::testMoveAssignment_FromNonEmptyToEmpty()
{
	FastStringBuffer<char> from, to;

	from.reserve(4);
	from.append("abc", 3);

	const char * const fromBuf = from.c_str();

	to = std::move(from);

	CPPUNIT_ASSERT_EQUAL(size_t(7), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(3), to.size());

	const char * const str = to.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("abc"), string(str));
	CPPUNIT_ASSERT_EQUAL(str, to.c_str());
	// Ensuring no re-allocation happened.
	CPPUNIT_ASSERT_EQUAL(str, fromBuf);

	CPPUNIT_ASSERT_EQUAL(size_t(0), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), from.size());

	const char * const str2 = from.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str2[0]);
	CPPUNIT_ASSERT_EQUAL(str2, from.c_str());
}

void afc::FastStringBufferTest::testMoveAssignment_FromEmptyToNonEmpty()
{
	FastStringBuffer<char> from, to;

	to.reserve(4);
	to.append("abc", 3);

	const char * const toBuf = to.c_str();

	to = std::move(from);

	CPPUNIT_ASSERT_EQUAL(size_t(7), from.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(3), from.size());

	const char * const str = from.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("abc"), string(str));
	CPPUNIT_ASSERT_EQUAL(str, from.c_str());
	// Ensuring no re-allocation happened.
	CPPUNIT_ASSERT_EQUAL(str, toBuf);

	CPPUNIT_ASSERT_EQUAL(size_t(0), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(0), to.size());

	const char * const str2 = to.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str2[0]);
	CPPUNIT_ASSERT_EQUAL(str2, to.c_str());
}

void afc::FastStringBufferTest::testMoveAssignment_FromNonEmptyToNonEmpty()
{
	FastStringBuffer<char> from, to;

	from.reserve(4);
	from.append("abc", 3);

	to.reserve(2);
	to.append("me", 2);

	const char * const fromBuf = from.c_str();
	const char * const toBuf = to.c_str();

	to = std::move(from);

	CPPUNIT_ASSERT_EQUAL(size_t(7), to.capacity());
	CPPUNIT_ASSERT_EQUAL(size_t(3), to.size());

	const char * const str = to.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("abc"), string(str));
	CPPUNIT_ASSERT_EQUAL(str, to.c_str());
	// Ensuring no re-allocation happened.
	CPPUNIT_ASSERT_EQUAL(str, fromBuf);

	const char * const str2 = from.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("me"), string(str2));
	CPPUNIT_ASSERT_EQUAL(str2, from.c_str());
	// Ensuring no re-allocation happened.
	CPPUNIT_ASSERT_EQUAL(str2, toBuf);
}

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

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());

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

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());

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
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str2[0]);
	CPPUNIT_ASSERT_EQUAL(str2, buf.c_str());

	CPPUNIT_ASSERT(str != str2);

	buf.reserve(5);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	// Capacity grows as 2^n-1.
	CPPUNIT_ASSERT_EQUAL(size_t(7), buf.capacity());

	const char * const str3 = buf.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str3[0]);
	CPPUNIT_ASSERT_EQUAL(str3, buf.c_str());

	CPPUNIT_ASSERT(str2 != str3);
}

void afc::FastStringBufferTest::testChar_EmptyBuffer_ReserveSpace_TryToReduceCapacity()
{
	FastStringBuffer<char> buf;

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());

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

void afc::FastStringBufferTest::testChar_EmptyBuffer_ReserveSpace_TryToReduceCapacityToZero()
{
	FastStringBuffer<char> buf;

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());

	buf.reserve(3);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve(0);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	// Ensuring that the buffer is neither re-allocated nor modified.
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
}

void afc::FastStringBufferTest::testChar_EmptyBuffer_ReserveSpace_ThenReserveSameSpace()
{
	FastStringBuffer<char> buf;

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.capacity());

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

void afc::FastStringBufferTest::testChar_AppendCharArray_SingleAppend_EmptyArray()
{
	FastStringBuffer<char> buf;
	buf.reserve(3);

	buf.append("Joy", 0);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str[0]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve(4);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(7), buf.capacity());

	const char * const str2 = buf.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str2[0]);
	CPPUNIT_ASSERT(str2 != str); // Ensuring re-allocation happens.

	buf.reserve(3);

	CPPUNIT_ASSERT_EQUAL(size_t(0), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(7), buf.capacity());

	const char * const str3 = buf.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL('\0', str3[0]);
	CPPUNIT_ASSERT_EQUAL(str3, str2); // Ensuring no re-allocation happens.
}


void afc::FastStringBufferTest::testChar_AppendCharArray_SingleAppend_NonEmptyArray()
{
	FastStringBuffer<char> buf;
	buf.reserve(3);

	buf.append("Joy", 3);

	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("Joy"), string(str));
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve(4);

	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(7), buf.capacity());

	const char * const str2 = buf.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("Joy"), string(str2));
	CPPUNIT_ASSERT(str2 != str); // Ensuring re-allocation happens.

	buf.reserve(3);

	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(7), buf.capacity());

	const char * const str3 = buf.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("Joy"), string(str3));
	CPPUNIT_ASSERT_EQUAL(str3, str2); // Ensuring no re-allocation happens.

	// Ensuring ::c_str() influences neither size nor capacity.
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(7), buf.capacity());
}

void afc::FastStringBufferTest::testChar_AppendCharArray_MultipleAppends()
{
	FastStringBuffer<char> buf;
	buf.reserve(3);

	buf.append("Bom", 3);

	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("Bom"), string(str));
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve(20);

	buf.append("Bibom", 5);

	CPPUNIT_ASSERT_EQUAL(size_t(8), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(31), buf.capacity());

	const char * const str2 = buf.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("BomBibom"), string(str2));
	CPPUNIT_ASSERT_EQUAL(str2, buf.c_str());
	CPPUNIT_ASSERT(str2 != str); // Ensuring re-allocation happens.

	buf.append(" is classics! Really.", " is classics!"_s.size());

	CPPUNIT_ASSERT_EQUAL("BomBibom is classics!"_s.size(), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(31), buf.capacity());

	const char * const str3 = buf.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("BomBibom is classics!"), string(str3));
	CPPUNIT_ASSERT_EQUAL(str3, buf.c_str());
	CPPUNIT_ASSERT_EQUAL(str2, str3); // Ensuring no re-allocation happens.

	// Ensuring ::c_str() influences neither size nor capacity.
	CPPUNIT_ASSERT_EQUAL("BomBibom is classics!"_s.size(), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(31), buf.capacity());
}

void afc::FastStringBufferTest::testChar_AppendCharArray_MultipleAppends_WithEmptyArray()
{
	FastStringBuffer<char> buf;
	buf.reserve(4);

	buf.append("Bom", 3);

	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(7), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("Bom"), string(str));
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve("Bom is good!"_s.size());

	buf.append("Bibom", 0); // Nothing is appended.

	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(15), buf.capacity());

	const char * const str2 = buf.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("Bom"), string(str2));
	CPPUNIT_ASSERT_EQUAL(str2, buf.c_str());
	CPPUNIT_ASSERT(str2 != str); // Ensuring re-allocation happens.

	// Ensuring ::c_str() influences neither size nor capacity.
	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(15), buf.capacity());

	buf.append(" is good! Really.", " is good!"_s.size());

	CPPUNIT_ASSERT_EQUAL(size_t(12), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(15), buf.capacity());

	const char * const str3 = buf.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("Bom is good!"), string(str3));
	CPPUNIT_ASSERT_EQUAL(str3, buf.c_str());
	CPPUNIT_ASSERT_EQUAL(str2, str3); // Ensuring no re-allocation happens.

	// What happens if final buf size equals to capacity?
	buf.append("!11", 3);

	CPPUNIT_ASSERT_EQUAL(size_t(15), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(15), buf.capacity());

	const char * const str4 = buf.c_str();
	CPPUNIT_ASSERT(str4 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("Bom is good!!11"), string(str4));
	CPPUNIT_ASSERT_EQUAL(str4, buf.c_str());
	CPPUNIT_ASSERT_EQUAL(str3, str4); // Ensuring no re-allocation happens.
}

void afc::FastStringBufferTest::testChar_AppendCharArray_MultipleAppends_WithTerminatingChars()
{
	FastStringBuffer<char> buf;
	buf.reserve(4);

	buf.append("B\0m", 3);

	CPPUNIT_ASSERT_EQUAL(size_t(3), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(7), buf.capacity());

	const char * const str = buf.c_str();
	CPPUNIT_ASSERT(str != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("B\0m", 3), string(str, 3));
	CPPUNIT_ASSERT_EQUAL('\0', str[3]);
	CPPUNIT_ASSERT_EQUAL(str, buf.c_str());

	buf.reserve(30);

	buf.append("Bib\0m\0", 6);

	CPPUNIT_ASSERT_EQUAL(size_t(9), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(31), buf.capacity());

	const char * const str2 = buf.c_str();
	CPPUNIT_ASSERT(str2 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("B\0mBib\0m\0", 9), string(str2, 9));
	CPPUNIT_ASSERT_EQUAL('\0', str2[9]);
	CPPUNIT_ASSERT_EQUAL(str2, buf.c_str());
	CPPUNIT_ASSERT(str2 != str); // Ensuring re-allocation happens.

	// Ensuring ::c_str() influences neither size nor capacity.
	CPPUNIT_ASSERT_EQUAL(size_t(9), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(31), buf.capacity());

	buf.append(" is good! Really.", " is good!"_s.size());

	constexpr size_t expectedSize = "B\0mBib\0m\0 is good!"_s.size();

	CPPUNIT_ASSERT_EQUAL(expectedSize, buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(31), buf.capacity());

	const char * const str3 = buf.c_str();
	CPPUNIT_ASSERT(str3 != nullptr);
	CPPUNIT_ASSERT_EQUAL(string("B\0mBib\0m\0 is good!", expectedSize), string(str3, expectedSize));
	CPPUNIT_ASSERT_EQUAL('\0', str2[expectedSize]);
	CPPUNIT_ASSERT_EQUAL(str3, buf.c_str());
	CPPUNIT_ASSERT_EQUAL(str2, str3); // Ensuring no re-allocation happens.

	// Ensuring ::c_str() influences neither size nor capacity.
	CPPUNIT_ASSERT_EQUAL(size_t(expectedSize), buf.size());
	CPPUNIT_ASSERT_EQUAL(size_t(31), buf.capacity());
}
