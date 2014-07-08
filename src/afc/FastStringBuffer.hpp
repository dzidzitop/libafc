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
#include <cstring>
#include <algorithm>
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
	private:
		FastStringBuffer(const FastStringBuffer &) = delete;
		FastStringBuffer &operator=(const FastStringBuffer &) = delete;
	public:
		FastStringBuffer(FastStringBuffer &&) = default;
		FastStringBuffer &operator=(FastStringBuffer &&) = default;

		FastStringBuffer() noexcept : m_buf(nullptr), m_bufEnd(nullptr), m_storageSize(0) {}

		~FastStringBuffer()
		{
			if (std::is_pod<CharType>::value) {
				// Nothing to do. POD structures do not need to be destructed.
			} else {
				std::for_each(std::addressof(m_buf[0]), m_bufEnd, [=](CharType &p) { p.~CharType(); });
			}
		}

		void reserve(const std::size_t n)
		{
			if (m_storageSize <= n) {
				expand(n);
			}
		}

		FastStringBuffer &append(const CharType * const str, const std::size_t n) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(str != nullptr);
			assert(size() + n < m_storageSize);

			/* Cannot use std::memcpy() here since str can be the internal buffer itself
			 * returned to the caller by ::c_str().
			 */
			m_bufEnd = std::copy_n(&str[0], n, m_bufEnd);
			return *this;
		}

		FastStringBuffer &append(const std::initializer_list<const CharType> str) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(size() + str.size() < m_storageSize);

			for (const char c : str) {
				*m_bufEnd++ = c;
			}
			return *this;
		}

		FastStringBuffer &operator+=(const CharType c) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(&m_buf[0] + capacity() > m_bufEnd);

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
			return m_storageSize - 1;
		}

		std::size_t size() const noexcept
		{
			// Works fine even if both are null pointers.
			return m_bufEnd - &m_buf[0];
		}

		constexpr std::size_t maxSize() const noexcept
		{
			// The element m_buf[maxSize()] is reserved for the terminating character.
			return std::numeric_limits<decltype(m_bufEnd - &m_buf[0])>::max() - 1;
		}
	private:
		void expand(std::size_t capacity);
		inline std::size_t nextStorageSize(std::size_t capacity);

		static const CharType empty[1];

		// If not nullptr then one character is reserved for the terminating character.
		std::unique_ptr<CharType[]> m_buf;
		CharType *m_bufEnd;
		std::size_t m_storageSize;
	};
}

template<typename CharType>
const CharType afc::FastStringBuffer<CharType>::empty[1] = {CharType(0)};

template<typename CharType>
inline std::size_t afc::FastStringBuffer<CharType>::nextStorageSize(const std::size_t capacity)
{
	// FastStringBuffer::reserve() does not expand storage if capacity == 0.
	assert(capacity > 0);

	const std::size_t maxSize = this->maxSize();
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

	const std::size_t maxStorageSize = maxSize + 1;
	const std::size_t requestedStorageSize = capacity + 1;

	/* Minimal next storage size is 2 (if n == 1) - one for the character requested,
	 * the other for the terminating character.
	 */
	std::size_t newStorageSize = std::max(std::size_t(1), m_storageSize);
	do {
		newStorageSize *= 2;
		if (newStorageSize <= m_storageSize || newStorageSize > maxStorageSize) {
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
	std::unique_ptr<CharType[]> newBuf(static_cast<CharType *>(::operator new(newStorageSize * sizeof(CharType))));
	const std::size_t size = this->size();
	if (size != 0) {
		if (std::is_pod<CharType>::value) {
			// POD values are copied by std::memcpy, which is efficient for all compilers/runtimes.
			std::memcpy(&newBuf[0], &m_buf[0], size * sizeof(CharType));
		} else {
			for (CharType *src = std::addressof(m_buf[0]), *dest = std::addressof(newBuf[0]);
					src != m_bufEnd; ++src, ++dest) {
				new (dest) CharType(*src);
			}
		}
	}
	m_bufEnd = &newBuf[size];
	m_buf.reset(newBuf.release());
	m_storageSize = newStorageSize;
}

#endif /* AFC_FASTSTRINGBUFFER_HPP_ */
