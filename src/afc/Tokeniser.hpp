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

#include <algorithm>
#include <cstddef>
#include <string>
#include <type_traits>
#include <utility>
#include "platform.h"
#ifdef AFC_EXCEPTIONS_ENABLED
	#include "Exception.h"
#endif

namespace afc
{
	template<typename CharType, typename Iterator>
	class Tokeniser
	{
		Tokeniser(const Tokeniser &) = delete;
		Tokeniser(Tokeniser &&) = delete;
		Tokeniser &operator=(const Tokeniser &) = delete;
		Tokeniser &operator=(Tokeniser &&) = delete;
	public:
		Tokeniser(const Iterator begin, Iterator end, const CharType delimiter)
				noexcept(std::is_nothrow_copy_constructible<Iterator>::value &&
						std::is_nothrow_copy_constructible<CharType>::value)
				: m_curr(begin), m_end(end), m_delimiter(delimiter), m_hasNext(true) {}

		bool hasNext() const noexcept { return m_hasNext; }
		std::pair<Iterator, Iterator> next();
		void next(Iterator &begin, Iterator &end);
		void skip();
		void skip(const std::size_t n) { for (std::size_t i = n; i > 0; --i) { skip(); } }
	private:
		Iterator m_curr;
		Iterator m_end;
		const CharType m_delimiter;
		bool m_hasNext;
	};
}

template<typename CharType, typename Iterator>
inline std::pair<Iterator, Iterator> afc::Tokeniser<CharType, Iterator>::next()
{
#ifdef AFC_EXCEPTIONS_ENABLED
	if (!m_hasNext) {
		throwException<IllegalStateException>("Tokeniser::next is called when it has not more tokens");
	}
#endif

	std::pair<Iterator, Iterator> result(m_curr, std::find(m_curr, m_end, m_delimiter));
	m_curr = std::find(m_curr, m_end, m_delimiter);
	if (m_curr != m_end) {
		++m_curr;
	} else {
		m_hasNext = false;
	}
	return result;
}

template<typename CharType, typename Iterator>
inline void afc::Tokeniser<CharType, Iterator>::next(Iterator &begin, Iterator &end)
{
#ifdef AFC_EXCEPTIONS_ENABLED
	if (!m_hasNext) {
		throwException<IllegalStateException>("Tokeniser::next is called when it has not more tokens");
	}
#endif

	begin = m_curr;
	end = m_curr = std::find(m_curr, m_end, m_delimiter);
	if (m_curr != m_end) {
		++m_curr;
	} else {
		m_hasNext = false;
	}
}

template<typename CharType, typename Iterator>
inline void afc::Tokeniser<CharType, Iterator>::skip()
{
#ifdef AFC_EXCEPTIONS_ENABLED
	if (!m_hasNext) {
		throwException<IllegalStateException>("Tokeniser::skip is called when it has not more tokens");
	}
#endif

	m_curr = std::find(m_curr, m_end, m_delimiter);
	if (m_curr != m_end) {
		++m_curr;
	}
}

#endif /* AFC_TOKENISER_HPP_ */
