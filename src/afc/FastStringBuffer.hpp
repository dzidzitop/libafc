/* libafc - utils to facilitate C++ development.
Copyright (C) 2014-2019 Dźmitry Laŭčuk

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
#ifndef AFC_FASTSTRINGBUFFER_HPP_
#define AFC_FASTSTRINGBUFFER_HPP_

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#ifdef AFC_FASTSTRINGBUFFER_DEBUG
	#include <iterator>
#endif
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>

#include "builtin.hpp"
#include "math_utils.h"
#include "StringRef.hpp"
#ifdef AFC_EXCEPTIONS_ENABLED
	#include <new>
#else
	#include <exception>
#endif

namespace afc
{
	enum class AllocMode
	{
		pow2,
		accurate
	};

	// A buffer that assumes that the caller handles the capacity of the buffer manually.
	template<typename CharType, afc::AllocMode allocMode = afc::AllocMode::pow2>
	class FastStringBuffer
	{
		/* Supporting consistent and still efficient implementation for non-POD types is
		 * impossible due to the fact non-POD values must be destructed accurately,
		 * including the C-string terminator that is created by FastStringBuffer by
		 * request for the sake of performance. Once created, it must be shifted to
		 * the end of the buffer for each ::append() invocation, which is expensive.
		 */
		static_assert(std::is_pod<CharType>::value, "Non-POD types are not supported as CharType.");

		/* Copying/passing/destroying arrays is implemented differently in C++ than
		 * for * other types (e.g. the array-decay rule). In addition, it is difficult
		 * to imagine a case when a buffer of arrays is created, so they are not supported
		 * as implementations of CharType.
		 */
		static_assert(!std::is_array<CharType>::value, "Fixed-size arrays are not supported as CharType.");

		friend class FastStringBufferTest;

#ifdef AFC_EXCEPTIONS_ENABLED
		[[noreturn]]
		static void badAlloc() { throw std::bad_alloc(); }
#else
		[[noreturn]]
		static void badAlloc() noexcept { std::terminate(); }
#endif
	private:
		FastStringBuffer(const FastStringBuffer &) = delete;
		FastStringBuffer &operator=(const FastStringBuffer &) = delete;
	public:
		FastStringBuffer() noexcept : m_buf(nullptr), m_bufEnd(nullptr), m_capacity(0) {}
		explicit FastStringBuffer(const std::size_t initialCapacity) noexcept(noexcept(badAlloc()))
		{
			if (likely(initialCapacity > 0)) {
				const std::size_t storageSize = nextStorageSize(initialCapacity);
				m_capacity = storageSize - 1;
				// Alignment of the block allocated is suitable for CharType elements.
				register void * const ptr = std::malloc(storageSize * sizeof(CharType));
				if (likely(ptr != nullptr)) {
					m_bufEnd = m_buf = static_cast<CharType *>(ptr);
				} else {
					badAlloc();
				}
			} else {
				m_buf = m_bufEnd = nullptr;
				m_capacity = 0;
			}
		}

		// Moves content from o to this FastStringBuffer and resets the state of o.
		FastStringBuffer(FastStringBuffer &&o) noexcept : m_buf(o.m_buf), m_bufEnd(o.m_bufEnd),
				m_capacity(o.m_capacity)
		{
			o.m_buf = nullptr;
			o.m_bufEnd = nullptr;
			o.m_capacity = 0;
		}

		// Swaps these two buffers.
		FastStringBuffer &operator=(FastStringBuffer &&o) noexcept
		{
			std::swap(m_buf, o.m_buf);
			std::swap(m_bufEnd, o.m_bufEnd);
			std::swap(m_capacity, o.m_capacity);
			return *this;
		}

		~FastStringBuffer() { std::free(m_buf); };

		void reserve(const std::size_t n) noexcept(noexcept(badAlloc()))
		{
			if (m_capacity < n) {
				expand(n);
			}
		}
		void reserveForOne() noexcept(noexcept(badAlloc()))
		{
			if (m_buf + m_capacity == m_bufEnd) {
				expand();
			}
		}

		template<typename Iterator>
		FastStringBuffer &append(const Iterator from, const Iterator to) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(size() + (to - from) <= m_capacity);

			/* Cannot use std::memcpy() here since str can be the internal buffer itself
			 * returned to the caller by ::c_str().
			 */
			m_bufEnd = std::copy(from, to, m_bufEnd);
			return *this;
		}

		FastStringBuffer &append(const CharType * const str, const std::size_t n) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(str != nullptr);
			assert(size() + n <= m_capacity);

			/* Cannot use std::memcpy() here since str can be the internal buffer itself
			 * returned to the caller by ::c_str().
			 */
			m_bufEnd = std::copy_n(std::addressof(str[0]), n, m_bufEnd);
			return *this;
		}

		FastStringBuffer &append(const std::initializer_list<const CharType> values) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(size() + values.size() <= m_capacity);

			m_bufEnd = std::copy_n(values.begin(), values.size(), m_bufEnd);
			return *this;
		}

		FastStringBuffer &append(afc::ConstStringRef str) noexcept { return append(str.value(), str.size()); }

		FastStringBuffer &append(const CharType c) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(m_buf + capacity() > m_bufEnd);

			*m_bufEnd++ = c;
			return *this;
		}

		const CharType *data() const noexcept { return m_buf == nullptr ? empty : m_buf; }

		const CharType *c_str() const noexcept(noexcept(CharType(0)))
		{
			if (m_buf == nullptr) {
				return empty;
			} else {
				// Terminating the string with the null character.
				*m_bufEnd = CharType(0);
				return m_buf;
			}
		}

		CharType *begin() noexcept { return m_buf; }
		const CharType *begin() const noexcept { return m_buf; }
		CharType *end() noexcept { return m_bufEnd; }
		const CharType *end() const noexcept { return m_bufEnd; }

		std::size_t capacity() const noexcept
		{
			// The last element is reserved for the terminating character.
			return m_capacity;
		}

		std::size_t size() const noexcept
		{
			// Works fine even if both are null pointers.
			return m_bufEnd - m_buf;
		}

		void resize(const std::size_t newSize) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr || newSize == 0);
			assert(newSize <= m_capacity);

			m_bufEnd = m_buf + newSize;
		}
		void clear() noexcept { m_bufEnd = m_buf; }
		CharType *detach() noexcept { CharType * const result = m_buf; m_buf = m_bufEnd = nullptr; return result; }

		std::size_t maxSize() const noexcept { return maxCapacity(); }

#ifdef AFC_FASTSTRINGBUFFER_DEBUG
		class Tail;
		friend class Tail;

		class Tail : public std::iterator<std::random_access_iterator_tag, CharType>
		{
			friend class FastStringBuffer;

			Tail(CharType *ptr);
		public:
			Tail(const Tail &o);
			~Tail();

			Tail &operator=(const Tail &o);

			const CharType &operator *() const noexcept { assert(!m_returned); return *m_ptr; }
			CharType &operator *() noexcept { assert(!m_returned); return *m_ptr; }

			Tail &operator++() noexcept { assert(!m_returned); ++m_ptr; return *this; }
			Tail operator++(int) noexcept { assert(!m_returned); Tail result(*this); ++m_ptr; return result; }

			Tail operator-(const std::ptrdiff_t n) const noexcept { assert(!m_returned); return Tail(m_ptr - n); }
		private:
			CharType *m_ptr;
			std::shared_ptr<long> m_copyCount;
			mutable bool m_returned;
		};

		Tail borrowTail() noexcept { return Tail(m_bufEnd); };
		void returnTail(const Tail &tail) noexcept;
#else
		typedef CharType * Tail;

		Tail borrowTail() noexcept { return m_bufEnd; };
		void returnTail(const Tail tail) noexcept { assert(tail <= m_buf + m_capacity); m_bufEnd = tail; };
#endif
	private:
		inline void expand() noexcept(noexcept(badAlloc()));
		void expand(std::size_t capacity) noexcept(noexcept(badAlloc()));
		// The expected new capacity is passed in, not the current capacity.
		std::size_t nextStorageSize(std::size_t capacity) noexcept(noexcept(badAlloc()));

		static constexpr std::size_t maxCapacity() noexcept
		{
			/* The maximal size of an array that can be allocated within the address space
			 * whose index of the element after the last element equals to max value of ptrdiff_t
			 * (i.e. is addressable by ptrdiff_t)
			 *
			 * The element m_buf[maxCapacity()] is reserved for the terminating character.
			 */
			return afc::math::min<const std::size_t>(std::numeric_limits<std::size_t>::max() / sizeof(CharType) - 1,
					std::size_t(std::numeric_limits<std::ptrdiff_t>::max()));
		}

		static const CharType empty[1];

		// If not nullptr then one character is reserved for the terminating character.
		// Non-[] deleter is specified since placement allocation is used.
		CharType *m_buf;
		CharType *m_bufEnd;
		std::size_t m_capacity;
	};
}

template<typename CharType, afc::AllocMode allocMode>
const CharType afc::FastStringBuffer<CharType, allocMode>::empty[1] = {CharType(0)};

template<typename CharType, afc::AllocMode allocMode>
inline std::size_t afc::FastStringBuffer<CharType, allocMode>::nextStorageSize(const std::size_t capacity) noexcept(noexcept(badAlloc()))
{
	static_assert(allocMode == afc::AllocMode::pow2 || allocMode == afc::AllocMode::accurate, "Unsupported allocMode.");

	// FastStringBuffer::reserve() does not expand storage if capacity == 0.
	assert(capacity > 0);

	if (allocMode == afc::AllocMode::pow2) {
		constexpr std::size_t maxStorageSize = maxCapacity() + 1;
		const std::size_t requestedStorageSize = capacity + 1;

		// FastStringBuffer::reserve(std::size_t) does not expand storage if capacity == 0.
		/* Since newStorageSize is always a power of two, the first value that
		 * newStorageSize * 2 overflows with is (2^(n-1) * 2) mod 2^n = 0
		 */
		static_assert((std::numeric_limits<std::size_t>::max() / 2 + 1) * 2 == 0,
				"Wrong assumption on overflow rules.");

		/* Minimal next storage size is 2 (if n == 1) - one for the character requested,
		 * the other for the terminating character.
		 */
		const std::size_t newStorageSize = afc::math::ceilPow2(requestedStorageSize);

		if (newStorageSize == 0 || newStorageSize >= maxStorageSize) {
			// Overflow. Reducing storage size to max allowed.
			if (capacity > maxCapacity()) {
				// The capacity requested is greater than max size of this FastStringBuffer.
				badAlloc();
			}
			return maxStorageSize;
		}

		return newStorageSize;
	} else { // accurate mode
		if (capacity > maxCapacity()) {
			// The capacity requested is greater than max size of this FastStringBuffer.
			badAlloc();
		}
		return capacity + 1;
	}
}

template<typename CharType, afc::AllocMode allocMode>
void afc::FastStringBuffer<CharType, allocMode>::expand() noexcept(noexcept(badAlloc()))
{
	static_assert(allocMode == afc::AllocMode::pow2 || allocMode == afc::AllocMode::accurate, "Unsupported allocMode.");

	if (m_capacity == maxCapacity()) {
		// The capacity requested is greater than max size of this FastStringBuffer.
#ifdef AFC_EXCEPTIONS_ENABLED
		throw Exception("Capacity to reserve exceeds max size allowed."_s);
#else
		std::terminate();
#endif
	}

	register std::size_t newCapacity;

	if (allocMode == afc::AllocMode::pow2) {
		newCapacity = m_capacity * 2 + 1; // Never overflows.
		assert(newCapacity <= maxCapacity());
	} else { // accurate mode
		newCapacity = m_capacity + 1;
	}

	/* The old buffer is deleted (and replaced with the new buffer) iff no exception is thrown
	 * while copying data from the old buffer to the new one. Otherwise this FastStringBuffer
	 * remains unmodified.
	 */
	// Alignment of the block allocated is suitable for CharType elements.
	// POD values are copied bitwise, if needed, which is efficient for all compilers/runtimes.
	register void * const newBuf = std::realloc(m_buf, (newCapacity + 1) * sizeof(CharType));

	if (likely(newBuf != nullptr)) {
		register const std::size_t size = this->size();
		m_buf = static_cast<CharType *>(newBuf);
		m_bufEnd = m_buf + size;
		m_capacity = newCapacity;
	} else {
		badAlloc();
	}
}

template<typename CharType, afc::AllocMode allocMode>
void afc::FastStringBuffer<CharType, allocMode>::expand(const std::size_t capacity) noexcept(noexcept(badAlloc()))
{
	register const std::size_t newStorageSize = nextStorageSize(capacity);

	/* The old buffer is deleted (and replaced with the new buffer) iff no exception is thrown
	 * while copying data from the old buffer to the new one. Otherwise this FastStringBuffer
	 * remains unmodified.
	 */
	// Alignment of the block allocated is suitable for CharType elements.
	// POD values are copied bitwise, if needed, which is efficient for all compilers/runtimes.
	register void * const newBuf = std::realloc(m_buf, newStorageSize * sizeof(CharType));

	if (likely(newBuf != nullptr)) {
		register const std::size_t size = this->size();
		m_buf = static_cast<CharType *>(newBuf);
		m_bufEnd = m_buf + size;
		m_capacity = newStorageSize - 1;
	} else {
		badAlloc();
	}
}

#ifdef AFC_FASTSTRINGBUFFER_DEBUG
template<typename CharType, afc::AllocMode allocMode>
afc::FastStringBuffer<CharType, allocMode>::Tail::Tail(CharType *ptr)
		: m_ptr(ptr), m_copyCount(new long(1L)), m_returned(false)
{
	assert(ptr != nullptr);
}

template<typename CharType, afc::AllocMode allocMode>
afc::FastStringBuffer<CharType, allocMode>::Tail::Tail(const Tail &o)
		: m_ptr(o.m_ptr), m_copyCount(o.m_copyCount), m_returned(false)
{
	assert(!o.m_returned);
	++(*m_copyCount);
}

template<typename CharType, afc::AllocMode allocMode>
afc::FastStringBuffer<CharType, allocMode>::Tail::~Tail()
{
	if (--(*m_copyCount) == 0) {
		assert(m_returned);
	} else {
		assert(!m_returned);
	}
}

template<typename CharType, afc::AllocMode allocMode>
typename afc::FastStringBuffer<CharType, allocMode>::Tail &
afc::FastStringBuffer<CharType, allocMode>::Tail::operator=(const Tail &o)
{
	assert(!o.m_returned);
	assert(!o.m_returned);
	--(*m_copyCount);
	m_copyCount = o.m_copyCount;
	++(*m_copyCount);
	m_ptr = o.m_ptr;
	return *this;
}

template<typename CharType, afc::AllocMode allocMode>
void afc::FastStringBuffer<CharType, allocMode>::returnTail(const Tail &tail) noexcept
{
	// Asserts a tail can be returned only once.
	assert(!tail.m_returned);
	tail.m_returned = true;
	m_bufEnd = tail.m_ptr;
};
#endif

#endif /* AFC_FASTSTRINGBUFFER_HPP_ */
