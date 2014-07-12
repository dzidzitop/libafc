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
#ifndef AFC_FASTSTRINGBUFFER_HPP_
#define AFC_FASTSTRINGBUFFER_HPP_

#include <cstddef>
#include <cassert>
#include <initializer_list>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>
#include <cstring>
#include <algorithm>
#include "math_utils.h"
#ifdef AFC_EXCEPTIONS_ENABLED
	#include "Exception.h"
#else
	#include <exception>
#endif

namespace afc
{
	// A buffer that assumes that the caller handles the capacity of the buffer manually.
	template<typename CharType>
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
	private:
		FastStringBuffer(const FastStringBuffer &) = delete;
		FastStringBuffer &operator=(const FastStringBuffer &) = delete;
	public:
		FastStringBuffer() noexcept : m_buf(nullptr), m_bufEnd(nullptr), m_capacity(0) {}

		// Moves content from o to this FastStringBuffer and resets the state of o.
		FastStringBuffer(FastStringBuffer &&o) noexcept : m_buf(o.m_buf.release()),
				m_bufEnd(o.m_bufEnd), m_capacity(o.m_capacity)
		{
			o.m_bufEnd = nullptr;
			o.m_capacity = 0;
		}

		// Swaps these two buffers.
		FastStringBuffer &operator=(FastStringBuffer &&o) noexcept
		{
			m_buf.swap(o.m_buf);
			std::swap(m_bufEnd, o.m_bufEnd);
			std::swap(m_capacity, o.m_capacity);
			return *this;
		}

		/* Nothing to do other than default. POD structures do not need to be destructed,
		 * and the inner storage is freed by std::unique_ptr that wraps it.
		 */
		~FastStringBuffer() = default;

		void reserve(const std::size_t n)
		{
			if (m_capacity < n) {
				expand(n);
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

		FastStringBuffer &append(const std::initializer_list<const CharType> str) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(size() + str.size() <= m_capacity);

			for (const char c : str) {
				*m_bufEnd++ = c;
			}
			return *this;
		}

		FastStringBuffer &append(const CharType c) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(m_buf.get() + capacity() > m_bufEnd);

			*m_bufEnd++ = c;
			return *this;
		}

		const CharType *c_str() const noexcept
		{
			if (m_buf == nullptr) {
				return empty;
			} else {
				// Terminating the string with the null character.
				*m_bufEnd = CharType(0);
				return m_buf.get();
			}
		}

		std::size_t capacity() const noexcept
		{
			// The last element is reserved for the terminating character.
			return m_capacity;
		}

		std::size_t size() const noexcept
		{
			// Works fine even if both are null pointers.
			return m_bufEnd - m_buf.get();
		}

		std::size_t maxSize() const noexcept { return maxCapacity(); }
	private:
		void expand(std::size_t capacity);
		// The expected new capacity is passed in, not the current capacity.
		inline std::size_t nextStorageSize(std::size_t capacity);

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
		std::unique_ptr<CharType> m_buf;
		CharType *m_bufEnd;
		std::size_t m_capacity;
	};
}

template<typename CharType>
const CharType afc::FastStringBuffer<CharType>::empty[1] = {CharType(0)};

template<typename CharType>
inline std::size_t afc::FastStringBuffer<CharType>::nextStorageSize(const std::size_t capacity)
{
	// FastStringBuffer::reserve() does not expand storage if capacity == 0.
	assert(capacity > 0);

	constexpr std::size_t maxSize = maxCapacity();
	/* To prevent overflows, checking (without arithmetic operations) that
	 * the capacity requested fits hard limits of this FastStringBuffer.
	 */
	if (capacity > maxSize) {
#ifdef AFC_EXCEPTIONS_ENABLED
		throwException<OverflowException>("Capacity to reserve exceeds max size allowed.");
#else
		std::terminate();
#endif
	}

	constexpr std::size_t maxStorageSize = maxSize + 1;
	const std::size_t requestedStorageSize = capacity + 1;

	/* Minimal next storage size is 2 (if n == 1) - one for the character requested,
	 * the other for the terminating character.
	 */
	std::size_t newStorageSize = m_capacity + 1;
	do {
		assert(afc::isPow2(newStorageSize));

		newStorageSize *= 2;

		/* Since newStorageSize is always a power of two, the first value that
		 * newStorageSize * 2 overflows with is (2^(n-1) * 2) mod 2^n = 0
		 */
		static_assert((std::numeric_limits<std::size_t>::max() / 2 + 1) * 2 == 0,
				"Wrong assumption on overflow rules.");

		if (newStorageSize == 0 || newStorageSize >= maxStorageSize) {
			// Overflow. Reducing storage size to max allowed.
			return maxStorageSize;
		}
	} while (newStorageSize < requestedStorageSize);

	return newStorageSize;
}

template<typename CharType>
void afc::FastStringBuffer<CharType>::expand(const std::size_t capacity)
{
	const std::size_t newStorageSize = nextStorageSize(capacity);

	/* The old buffer is deleted (and replaced with the new buffer) iff no exception is thrown
	 * while copying data from the old buffer to the new one. Otherwise this FastStringBuffer
	 * remains unmodified.
	 */
	// Alignment of the block allocated is suitable for CharType elements.
	std::unique_ptr<CharType> newBuf(static_cast<CharType *>(::operator new(newStorageSize * sizeof(CharType))));
	const std::size_t size = this->size();
	if (size != 0) {
		// POD values are copied by std::memcpy, which is efficient for all compilers/runtimes.
		std::memcpy(newBuf.get(), m_buf.get(), size * sizeof(CharType));
	}
	m_bufEnd = newBuf.get() + size;
	m_buf.reset(newBuf.release());
	m_capacity = newStorageSize - 1;
}

#endif /* AFC_FASTSTRINGBUFFER_HPP_ */
