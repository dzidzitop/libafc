/* libafc - utils to facilitate C++ development.
Copyright (C) 2015 Dźmitry Laŭčuk

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
#ifndef AFC_SIMPLESTRING_HPP_
#define AFC_SIMPLESTRING_HPP_

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <iterator>
#include <memory>

#include "builtin.hpp"
#include "StringRef.hpp"
#include "utils.h"

namespace afc
{
	// TODO make a template out of this class
	class SimpleString
	{
	public:
		SimpleString() noexcept : m_str(nullptr), m_size(0) {}
		SimpleString(const SimpleString &str) noexcept(noexcept(afc::badAlloc()))
				: SimpleString(str.m_str, str.m_size) {}
		SimpleString(SimpleString &&str) noexcept
				: m_str(str.m_str), m_size(str.m_size) { str.m_str = nullptr; str.m_size = 0; }
		inline explicit SimpleString(const char *str) noexcept(noexcept(afc::badAlloc()));
		inline SimpleString(const char * const str, const std::size_t size) noexcept(noexcept(afc::badAlloc()));
		inline explicit SimpleString(const ConstStringRef &str) noexcept(noexcept(afc::badAlloc()));
		SimpleString(const char *begin, const char *end) noexcept(noexcept(afc::badAlloc()))
				: SimpleString(begin, end - begin) {}
		// TODO implement this
		template<typename Iterator>
		SimpleString(Iterator begin, Iterator end);

		SimpleString &operator=(const SimpleString &str) noexcept(noexcept(afc::badAlloc()))
				{ assign(str.m_str, str.m_size); return *this; }
		SimpleString &operator=(SimpleString &&str) noexcept
				{ m_str = str.m_str; str.m_str = nullptr; m_size = str.m_size; str.m_size = 0; return *this; }
		SimpleString &operator=(const char *str) noexcept(noexcept(afc::badAlloc()))
				{ assert(str != nullptr); assign(str, std::strlen(str)); return *this; }
		SimpleString &operator=(const ConstStringRef &str) noexcept(noexcept(afc::badAlloc()))
				{ assign(str.value(), str.size()); return *this; }

		inline void assign(const char *begin, const char *end) noexcept(noexcept(afc::badAlloc()));
		inline void assign(char *begin, char *end) noexcept(noexcept(afc::badAlloc()))
				{ assign(const_cast<const char *>(begin), const_cast<const char *>(end)); }
		inline void assign(const char *str, const std::size_t size) noexcept(noexcept(afc::badAlloc()));
		template<typename Iterator>
		inline void assign(Iterator begin, Iterator end);

		SimpleString &attach(const char * const str, const std::size_t strSize) noexcept
				{ std::free(const_cast<char *>(m_str)); m_str = str; m_size = strSize; return *this; }

		~SimpleString() { std::free(const_cast<char *>(m_str)); };

		explicit operator const char *() const noexcept { return m_str; }

		const char *data() const noexcept { return m_str; }
		const char *c_str() const noexcept
		{
			if (m_str == nullptr) {
				return SimpleString::EmptyCStr<char>::value;
			} else {
				const_cast<char *>(m_str)[m_size] = '\0';
				return m_str;
			}
		}

		std::size_t size() const noexcept { return m_size; }
		bool empty() const noexcept { return m_size == 0; }

		const char &operator[](const std::size_t i) const noexcept { return m_str[i]; };

		const char *begin() const noexcept { return &m_str[0]; };
		const char *end() const noexcept { return &m_str[m_size]; };

		void clear() noexcept { std::free(const_cast<char *>(m_str)); m_str = nullptr; m_size = 0; }
	private:
		const char *m_str;
		// TODO m_strEnd should support for more efficient iteration
		std::size_t m_size;

		template<typename T> // just to init it in the header file.
		struct EmptyCStr {
			static const T value[1];
		};
	};

	template<typename Iterator>
	inline Iterator copy(const SimpleString &s, Iterator dest) { return std::copy_n(s.data(), s.size(), dest); }
}

template<typename T>
const T afc::SimpleString::EmptyCStr<T>::value[1] = {T(0)};

afc::SimpleString::SimpleString(const char * const str) noexcept(noexcept(afc::badAlloc()))
{
	assert(str != nullptr);

	m_size = std::strlen(str);
	m_str = static_cast<char *>(std::malloc(m_size * sizeof(char) + 1));
	if (unlikely(m_str == nullptr)) {
		badAlloc();
	}
	std::copy_n(str, m_size, const_cast<char *>(m_str));
}

afc::SimpleString::SimpleString(const char * const str, const std::size_t size) noexcept(noexcept(afc::badAlloc()))
		: m_size(size)
{
	assert(str != nullptr);

	m_str = static_cast<char *>(std::malloc(size * sizeof(char) + 1));
	if (unlikely(m_str == nullptr)) {
		badAlloc();
	}
	std::copy_n(str, size, const_cast<char *>(m_str));
}

afc::SimpleString::SimpleString(const afc::ConstStringRef &str) noexcept(noexcept(afc::badAlloc()))
		: m_size(str.size())
{
	m_str = static_cast<char *>(std::malloc(m_size * sizeof(char) + 1));
	if (unlikely(m_str == nullptr)) {
		badAlloc();
	}
	copy(str, const_cast<char *>(m_str));
}

void afc::SimpleString::assign(const char * const str, const std::size_t size) noexcept(noexcept(afc::badAlloc()))
{
	assert(str != nullptr);

	char * const newBuf = static_cast<char *>(std::malloc(size * sizeof(char) + 1));
	if (unlikely(newBuf == nullptr)) {
		badAlloc();
	}
	std::copy_n(str, size, newBuf);
	std::free(const_cast<char *>(m_str));
	m_str = newBuf;
	m_size = size;
}

void afc::SimpleString::assign(const char * const begin, const char * const end) noexcept(noexcept(afc::badAlloc()))
{
	const std::size_t newSize = end - begin;
	char *newBuf = static_cast<char *>(std::malloc(newSize * sizeof(char) + 1));
	if (unlikely(newBuf == nullptr)) {
		badAlloc();
	}
	std::copy(begin, end, newBuf);
	std::free(const_cast<char *>(m_str));
	m_str = newBuf;
	m_size = newSize;
}

template<typename Iterator>
void afc::SimpleString::assign(Iterator begin, Iterator end)
{
	struct PGuard {
		PGuard(char *p) : ptr(p) {}
		~PGuard() { std::free(ptr); }

		char *ptr;
	};
	const std::size_t newSize = std::distance(begin, end);
	char *newBuf = static_cast<char *>(std::malloc(newSize * sizeof(char) + 1));
	if (unlikely(newBuf == nullptr)) {
		badAlloc();
	}
	{ PGuard newBufGuard(newBuf);
		std::copy(begin, end, newBuf);

		newBufGuard.ptr = const_cast<char *>(m_str);
		m_str = newBuf;
		m_size = newSize;
	}
}

#endif /* AFC_SIMPLESTRING_HPP_ */
