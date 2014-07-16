/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2014 Dźmitry Laŭčuk

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
#ifndef AFC_TOKENISER_HPP_
#define AFC_TOKENISER_HPP_
#include <string>
#include <cstddef>
#include <type_traits>
#include "platform.h"
#ifdef AFC_EXCEPTIONS_ENABLED
	#include "Exception.h"
#endif

namespace afc
{
	template<typename String = std::string>
	class Tokeniser
	{
		Tokeniser(const Tokeniser &) = delete;
		Tokeniser(Tokeniser &&) = delete;
		Tokeniser &operator=(const Tokeniser &) = delete;
		Tokeniser &operator=(Tokeniser &&) = delete;

		typedef typename String::value_type CharType;
	public:
		// A synonym of the type String::iterator.
		typedef typename String::iterator CharIterator;

		Tokeniser(String &str, const CharType delimiter)
				noexcept(std::is_nothrow_default_constructible<String>::value &&
						std::is_nothrow_copy_constructible<CharType>::value)
				: m_str(str), m_delimiter(delimiter), m_begin(0) {}
		Tokeniser(String &&str, const CharType delimiter)
				noexcept(std::is_nothrow_move_constructible<String>::value &&
						std::is_nothrow_copy_constructible<CharType>::value)
				: m_inputCopy(str), m_str(m_inputCopy), m_delimiter(delimiter), m_begin(0) {}

		bool hasNext() const noexcept {return m_begin != String::npos;}
		inline String next();
		inline void next(CharIterator &start, CharIterator &end);
		inline void skip();
		void skip(std::size_t n) { for (std::size_t i = n; i > 0; --i) { skip(); } }
	private:
		/* m_inputCopy is used only with temporary strings because they die early.
		   In other cases no copying of the input string is used. */
		String m_inputCopy;
		String &m_str;
		const CharType m_delimiter;
		std::size_t m_begin;
	};
}

template<typename String>
String afc::Tokeniser<String>::next()
{
#ifdef AFC_EXCEPTIONS_ENABLED
	if (m_begin == String::npos) {
		throwException<IllegalStateException>("Tokeniser::next is called when it has not more tokens");
	}
#endif

	const size_t end = m_str.find(m_delimiter, m_begin);
	if (end == String::npos) {
		auto token = m_str.substr(m_begin);
		m_begin = String::npos;
		return token;
	} else {
		auto token = m_str.substr(m_begin, end - m_begin);
		m_begin = end + 1;
		return token;
	}
}

template<typename String>
void afc::Tokeniser<String>::next(CharIterator &start, CharIterator &end)
{
#ifdef AFC_EXCEPTIONS_ENABLED
	if (m_begin == String::npos) {
		throwException<IllegalStateException>("Tokeniser::next is called when it has not more tokens");
	}
#endif

	start = m_str.begin() + m_begin;
	const std::size_t endIdx = m_str.find(m_delimiter, m_begin);
	if (endIdx == String::npos) {
		end = m_str.end();
		m_begin = String::npos;
	} else {
		end = m_str.begin() + endIdx;
		m_begin = endIdx + 1;
	}
}

template<typename String>
void afc::Tokeniser<String>::skip()
{
#ifdef AFC_EXCEPTIONS_ENABLED
	if (m_begin == String::npos) {
		throwException<IllegalStateException>("Tokeniser::skip is called when it has not more tokens");
	}
#endif

	const size_t end = m_str.find(m_delimiter, m_begin);
	m_begin = end == String::npos ? String::npos : m_begin = end + 1;
}

#endif /* AFC_TOKENISER_HPP_ */
