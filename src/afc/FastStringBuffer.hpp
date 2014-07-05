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

		FastStringBuffer() noexcept : m_buf(nullptr), m_capacity(0), m_size(0) {}
		~FastStringBuffer() { delete[] m_buf; }

		void reserve(const std::size_t n)
		{
			if (m_capacity < n) {
				expand(n);
			}
		}

		FastStringBuffer &append(const CharType * const str, const std::size_t n) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(str != nullptr);
			assert(m_size + n <= m_capacity);
			for (std::size_t i = 0; i < n; ++i) {
				m_buf[m_size++] = str[i];
			}
			return *this;
		}

		FastStringBuffer &append(const std::initializer_list<const CharType> str) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(m_size + str.size() <= m_capacity);
			for (const char c : str) {
				m_buf[m_size++] = c;
			}
			return *this;
		}

		FastStringBuffer &operator+=(const CharType c) noexcept
		{
			// assert() can throw an exception, but this is fine with debug code.
			assert(m_buf != nullptr);
			assert(m_size < m_capacity);
			m_buf[m_size++] = c;
			return *this;
		}

		const CharType *c_str() const noexcept
		{
			if (m_buf == nullptr) {
				return empty;
			} else {
				// Terminating the string with the null character.
				m_buf[m_size] = CharType(0);
				return m_buf;
			}
		}

		std::size_t capacity() const noexcept { return m_capacity; }
		std::size_t size() const noexcept { return m_size; }
	private:
		void expand(const std::size_t n);

		static const CharType empty[1];

		// If not nullptr then one character is reserved for '\0'.
		CharType *m_buf;
		std::size_t m_capacity;
		std::size_t m_size;
	};
}

template<typename CharType>
const CharType afc::FastStringBuffer<CharType>::empty[1] = {CharType(0)};

template<typename CharType>
void afc::FastStringBuffer<CharType>::expand(const std::size_t n)
{
	std::size_t newBufSize = m_capacity + 1;
	do {
		// TODO handler oveflow
		newBufSize *= 2;
	} while (newBufSize < n);

	/* The old buffer is deleted (and replaced with the new buffer) iff no exception is thrown
	 * while copying data from the old buffer to the new one. Otherwise this FastStringBuffer
	 * remains unmodified.
	 */
	CharType * const newBuf = new CharType[newBufSize];
	if (m_buf != nullptr) {
		for (std::size_t i = 0; i < m_size; ++i) {
			newBuf[i] = m_buf[i];
		}
		// It is safe to delete the old buffer here.
		delete m_buf;
	}
	m_buf = newBuf;
	m_capacity = newBufSize - 1;
}

#endif /* AFC_FASTSTRINGBUFFER_HPP_ */
