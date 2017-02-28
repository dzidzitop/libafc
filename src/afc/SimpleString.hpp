/* libafc - utils to facilitate C++ development.
Copyright (C) 2015-2017 Dźmitry Laŭčuk

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
#include <type_traits>
#include <utility>

#include "builtin.hpp"
#include "StringRef.hpp"

#ifdef AFC_EXCEPTIONS_ENABLED
	#include <new>
#else
	#include <exception>
#endif

namespace afc
{
#ifdef AFC_EXCEPTIONS_ENABLED
	// TODO specify noreturn here.
	inline void badAlloc() { throw std::bad_alloc(); }
#else
	// TODO specify noreturn here.
	inline void badAlloc() noexcept { std::terminate(); }
#endif

	template<typename CharType>
	class SimpleString
	{
		static_assert(std::is_pod<CharType>::value, "POD char types are supported only.");
	public:
		SimpleString() noexcept : m_str(nullptr), m_size(0) {}
		SimpleString(const SimpleString &str) noexcept(noexcept(afc::badAlloc()));
		SimpleString(SimpleString &&str) noexcept
				: m_str(str.m_str), m_size(str.m_size) { str.m_str = nullptr; str.m_size = 0; }
		inline explicit SimpleString(const CharType *str) noexcept(noexcept(afc::badAlloc()));
		inline SimpleString(const CharType * const str, const std::size_t size) noexcept(noexcept(afc::badAlloc()));
		template<typename StrRef = ConstStringRef,
				typename = typename std::enable_if<std::is_same<CharType, char>::value && std::is_same<StrRef, ConstStringRef>::value>::type>
		inline explicit SimpleString(StrRef str) noexcept(noexcept(afc::badAlloc()));
		SimpleString(const CharType * const begin, const CharType * const end) noexcept(noexcept(afc::badAlloc()))
				: SimpleString(begin, end - begin) {}
		// TODO implement this
		template<typename Iterator>
		SimpleString(Iterator begin, Iterator end);

		SimpleString &operator=(const SimpleString &str) noexcept(noexcept(afc::badAlloc()))
				{ assign(str.m_str, str.m_size); return *this; }
		SimpleString &operator=(SimpleString &&str) noexcept
				{ m_str = str.m_str; str.m_str = nullptr; m_size = str.m_size; str.m_size = 0; return *this; }
		SimpleString &operator=(const CharType * const str) noexcept(noexcept(afc::badAlloc()))
				{ assert(str != nullptr); assign(str, std::strlen(str)); return *this; }
		template<typename StrRef = ConstStringRef,
				typename = typename std::enable_if<std::is_same<CharType, char>::value && std::is_same<StrRef, ConstStringRef>::value>::type>
		SimpleString &operator=(StrRef str) noexcept(noexcept(afc::badAlloc()))
				{ assign(str.value(), str.size()); return *this; }

		inline void assign(const CharType *begin, const CharType *end) noexcept(noexcept(afc::badAlloc()));
		inline void assign(CharType * const begin, CharType * const end) noexcept(noexcept(afc::badAlloc()))
				{ assign(const_cast<const CharType *>(begin), const_cast<const CharType *>(end)); }
		inline void assign(const CharType *str, const std::size_t size) noexcept(noexcept(afc::badAlloc()));
		template<typename Iterator>
		inline void assign(Iterator begin, Iterator end);

		SimpleString &attach(const CharType * const str, const std::size_t strSize) noexcept
				{ std::free(const_cast<CharType *>(m_str)); m_str = str; m_size = strSize; return *this; }
		const CharType *detach() noexcept { const CharType * const buf = m_str; m_str = nullptr; m_size = 0; return buf; }

		~SimpleString() { std::free(const_cast<CharType *>(m_str)); };

		template<typename T>
		inline static SimpleString move(T &src)
				noexcept(noexcept(std::declval<T>().size()) && noexcept(std::declval<T>().detach()))
		{
			const std::size_t size = src.size();
			return SimpleString(src.detach(), size, 0);
		}

		explicit operator const char *() const noexcept { return m_str; }

		const CharType *data() const noexcept { return m_str; }
		const CharType *c_str() const noexcept
		{
			if (m_str == nullptr) {
				return emptyString;
			} else {
				const_cast<CharType *>(m_str)[m_size] = '\0';
				return m_str;
			}
		}

		std::size_t size() const noexcept { return m_size; }
		bool empty() const noexcept { return m_size == 0; }

		const CharType &operator[](const std::size_t i) const noexcept { return m_str[i]; };

		const CharType *begin() const noexcept { return &m_str[0]; };
		const CharType *end() const noexcept { return &m_str[m_size]; };

		void clear() noexcept { std::free(const_cast<CharType *>(m_str)); m_str = nullptr; m_size = 0; }
	private:
		SimpleString(CharType * const data, std::size_t n, int) noexcept : m_str(data),  m_size(n) {}

		const CharType *m_str;
		// TODO m_strEnd should support for more efficient iteration
		std::size_t m_size;

		static const CharType emptyString[1];
	};

	template<typename CharType, typename Iterator>
	inline Iterator copy(const SimpleString<CharType> &s, Iterator dest) { return std::copy_n(s.data(), s.size(), dest); }

	typedef SimpleString<char> String;
	typedef SimpleString<char> U8String;
	typedef SimpleString<char16_t> U16String;
}

template<typename CharType>
const CharType afc::SimpleString<CharType>::emptyString[1] = {CharType(0)};

template<typename CharType>
afc::SimpleString<CharType>::SimpleString(const CharType * const str) noexcept(noexcept(afc::badAlloc()))
{
	assert(str != nullptr);

	m_size = std::strlen(str);
	m_str = static_cast<CharType *>(std::malloc(m_size * sizeof(CharType) + 1));
	if (unlikely(m_str == nullptr)) {
		badAlloc();
	}
	std::copy_n(str, m_size, const_cast<CharType *>(m_str));
}

template<typename CharType>
afc::SimpleString<CharType>::SimpleString(const SimpleString &str) noexcept(noexcept(afc::badAlloc()))
{
	const register std::size_t strSize = str.m_size;
	if (strSize == 0) {
		m_str = nullptr;
		m_size = 0;
	} else {
		m_str = static_cast<CharType *>(std::malloc(strSize * sizeof(CharType) + 1));
		if (unlikely(m_str == nullptr)) {
			badAlloc();
		}
		std::copy_n(str.m_str, strSize, const_cast<CharType *>(m_str));
		m_size = strSize;
	}
}

template<typename CharType>
afc::SimpleString<CharType>::SimpleString(const CharType * const str, const std::size_t size) noexcept(noexcept(afc::badAlloc()))
		: m_size(size)
{
	assert(str != nullptr);

	m_str = static_cast<CharType *>(std::malloc(size * sizeof(CharType) + 1));
	if (unlikely(m_str == nullptr)) {
		badAlloc();
	}
	std::copy_n(str, size, const_cast<CharType *>(m_str));
}

template<typename CharType>
template<typename StrRef, typename>
afc::SimpleString<CharType>::SimpleString(StrRef str) noexcept(noexcept(afc::badAlloc()))
		: m_size(str.size())
{
	m_str = static_cast<CharType *>(std::malloc(m_size * sizeof(CharType) + 1));
	if (unlikely(m_str == nullptr)) {
		badAlloc();
	}
	copy(str, const_cast<CharType *>(m_str));
}

template<typename CharType>
void afc::SimpleString<CharType>::assign(const CharType * const str, const std::size_t size) noexcept(noexcept(afc::badAlloc()))
{
	assert(str != nullptr);

	CharType * const newBuf = static_cast<CharType *>(std::malloc(size * sizeof(CharType) + 1));
	if (unlikely(newBuf == nullptr)) {
		badAlloc();
	}
	std::copy_n(str, size, newBuf);
	std::free(const_cast<CharType *>(m_str));
	m_str = newBuf;
	m_size = size;
}

template<typename CharType>
void afc::SimpleString<CharType>::assign(const CharType * const begin, const CharType * const end) noexcept(noexcept(afc::badAlloc()))
{
	const std::size_t newSize = end - begin;
	CharType *newBuf = static_cast<CharType *>(std::malloc(newSize * sizeof(CharType) + 1));
	if (unlikely(newBuf == nullptr)) {
		badAlloc();
	}
	std::copy(begin, end, newBuf);
	std::free(const_cast<CharType *>(m_str));
	m_str = newBuf;
	m_size = newSize;
}

template<typename CharType>
template<typename Iterator>
void afc::SimpleString<CharType>::assign(Iterator begin, Iterator end)
{
	struct PGuard {
		PGuard(CharType *p) : ptr(p) {}
		~PGuard() { std::free(ptr); }

		CharType *ptr;
	};
	const std::size_t newSize = std::distance(begin, end);
	CharType *newBuf = static_cast<CharType *>(std::malloc(newSize * sizeof(CharType) + 1));
	if (unlikely(newBuf == nullptr)) {
		badAlloc();
	}
	{ PGuard newBufGuard(newBuf);
		std::copy(begin, end, newBuf);

		newBufGuard.ptr = const_cast<CharType *>(m_str);
		m_str = newBuf;
		m_size = newSize;
	}
}

#endif /* AFC_SIMPLESTRING_HPP_ */
