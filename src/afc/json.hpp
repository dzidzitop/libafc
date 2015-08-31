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

// Dedicated to Anžalika Žuk. She gave me strength and patience to implement this toolkit.

#ifndef AFC_JSON_HPP_
#define AFC_JSON_HPP_

#include <algorithm>
#include <cctype>
#include <iterator>
#include <type_traits>

#include <afc/builtin.hpp>

namespace afc
{
namespace json
{
	namespace _impl
	{
		template<typename I>
		constexpr bool isRandomAccessIterator() noexcept
		{
			typedef typename std::iterator_traits<I>::iterator_category ICategory;
			return std::is_same<ICategory, std::random_access_iterator_tag>::value;
		}
	}

	enum SpacePolicy {
		spaces,
		noSpaces,
		leadingSpaces,
		trailingSpaces
	};

	// TODO define noexcept
	template<typename Iterator>
	inline Iterator skipSpaces(Iterator begin, Iterator end) {
		return std::find_if(begin, end, [](const char c) { return !std::isspace(c); });
	}

	template<SpacePolicy spacePolicy, typename Iterator>
	inline Iterator skipLeadingSpaces(Iterator begin, Iterator end)
			noexcept(noexcept(skipSpaces<Iterator>(begin, end))) {
		return (spacePolicy == spaces || spacePolicy == leadingSpaces) ?
				skipSpaces(begin, end) :
				begin;
	}

	template<SpacePolicy spacePolicy, typename Iterator>
	inline Iterator skipTrailingSpaces(Iterator begin, Iterator end) {
		return (spacePolicy == spaces || spacePolicy == trailingSpaces) ?
				skipSpaces(begin, end) :
				begin;
	}

	// TODO define noexcept
	template<typename Iterator, typename ObjectBodyParser, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseObject(Iterator begin, Iterator end, ObjectBodyParser objectBodyParser, ErrorHandler &errorHandler)
	{
		Iterator i = skipLeadingSpaces<spacePolicy>(begin, end);
		if (unlikely(i == end)) {
			goto prematureEnd;
		}
		if (unlikely(*i != u8"{"[0])) {
			goto malformedJson;
		}

		i = objectBodyParser(++i, end, errorHandler);
		if (unlikely(!errorHandler.valid())) {
			return end;
		}
		if (unlikely(i == end)) {
			goto prematureEnd;
		}
		if (unlikely(*i != u8"}"[0])) {
			goto malformedJson;
		}
		return skipTrailingSpaces<spacePolicy>(++i, end);
	prematureEnd:
		errorHandler.prematureEnd();
		return end;
	malformedJson:
		errorHandler.malformedJson(i);
		return end;
	}

	template<typename Iterator, typename ArrayElementParser, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseArray(Iterator begin, Iterator end, ArrayElementParser arrayElementParser, ErrorHandler &errorHandler)
	{
		Iterator i = skipLeadingSpaces<spacePolicy>(begin, end);
		if (unlikely(i == end)) {
			goto prematureEnd;
		}
		if (unlikely(*i != u8"["[0])) {
			goto malformedJson;
		}
		if (unlikely(++i == end)) {
			goto prematureEnd;
		}
		if (*i == u8"]"[0]) {
			return skipTrailingSpaces<spacePolicy>(++i, end);
		}

		for (;;) {
			i = arrayElementParser(i, end, errorHandler);
			if (unlikely(!errorHandler.valid())) {
				return end;
			}
			if (unlikely(i == end)) {
				goto prematureEnd;
			}
			const char c = *i;

			if (c == u8"]"[0]) {
				return skipTrailingSpaces<spacePolicy>(++i, end);
			}
			if (likely(c == u8","[0])) {
				++i;
			} else {
				goto malformedJson;
			}
		}
	prematureEnd:
		errorHandler.prematureEnd();
		return end;
	malformedJson:
		errorHandler.malformedJson(i);
		return end;
	}

	// TODO define noexcept
	template<typename Iterator, typename StringParser, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseString(Iterator begin, Iterator end, StringParser stringParser, ErrorHandler &errorHandler)
	{
		Iterator i = skipLeadingSpaces<spacePolicy>(begin, end);
		if (unlikely(i == end)) {
			goto prematureEnd;
		}
		if (unlikely(*i != u8"\""[0])) {
			goto malformedJson;
		}

		i = stringParser(++i, end, errorHandler);
		if (unlikely(!errorHandler.valid())) {
			return end;
		}
		if (unlikely(i == end)) {
			goto prematureEnd;
		}
		if (unlikely(*i != u8"\""[0])) {
			goto malformedJson;
		}
		return skipTrailingSpaces<spacePolicy>(++i, end);
	prematureEnd:
		errorHandler.prematureEnd();
		return end;
	malformedJson:
		errorHandler.malformedJson(i);
		return end;
	}

	// TODO define noexcept
	template<typename Iterator, typename NumberParser, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseNumber(Iterator begin, Iterator end, NumberParser numberParser, ErrorHandler &errorHandler)
	{
		Iterator i = skipLeadingSpaces<spacePolicy>(begin, end);

		i = numberParser(i, end, errorHandler);
		// i must be equal to end in case of the error. No need to check for validity.

		return skipTrailingSpaces<spacePolicy>(i, end);
	}

	// TODO define noexcept
	template<typename Iterator, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline typename std::enable_if<!_impl::isRandomAccessIterator<Iterator>(), Iterator>::type
	parseBoolean(Iterator begin, Iterator end, bool &dest, ErrorHandler &errorHandler)
	{
		Iterator i = skipLeadingSpaces<spacePolicy>(begin, end);

		char c;

		if (unlikely(i == end)) {
			goto prematureEnd;
		}

		c = *i;
		if (c == u8"t"[0]) {
			if (unlikely(++i == end)) goto prematureEnd;
			if (unlikely(*i != u8"r"[0])) goto malformedJson;
			if (unlikely(++i == end)) goto prematureEnd;
			if (unlikely(*i != u8"u"[0])) goto malformedJson;
			if (unlikely(++i == end)) goto prematureEnd;
			if (unlikely(*i != u8"e"[0])) goto malformedJson;
			dest = true;
		} else if (c == u8"f"[0]) {
			if (unlikely(++i == end)) goto prematureEnd;
			if (unlikely(*i != u8"a"[0])) goto malformedJson;
			if (unlikely(++i == end)) goto prematureEnd;
			if (unlikely(*i != u8"l"[0])) goto malformedJson;
			if (unlikely(++i == end)) goto prematureEnd;
			if (unlikely(*i != u8"s"[0])) goto malformedJson;
			if (unlikely(++i == end)) goto prematureEnd;
			if (unlikely(*i != u8"e"[0])) goto malformedJson;
			dest = false;
		} else {
			goto malformedJson;
		}

		return skipTrailingSpaces<spacePolicy>(++i, end);
	prematureEnd:
		errorHandler.prematureEnd();
		return end;
	malformedJson:
		errorHandler.malformedJson(i);
		return end;
	}

	// TODO define noexcept
	template<typename Iterator, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline typename std::enable_if<_impl::isRandomAccessIterator<Iterator>(), Iterator>::type
	parseBoolean(Iterator begin, Iterator end, bool &dest, ErrorHandler &errorHandler)
	{
		Iterator i = skipLeadingSpaces<spacePolicy>(begin, end);

		char c;

		auto n = end - i;
		if (n < 4) {
			goto prematureEnd;
		}

		c = *i;
		if (c == u8"t"[0]) {
			if (unlikely(*(++i) != u8"r"[0] || *(++i) != u8"u"[0] || *(++i) != u8"e"[0])) {
				goto malformedJson;
			}
			++i;
			dest = true;
		} else if (c == u8"f"[0]) {
			if (n < 5) {
				goto prematureEnd;
			}
			if (unlikely(*(++i) != u8"a"[0] || *(++i) != u8"l"[0] || *(++i) != u8"s"[0] || *(++i) != u8"e"[0])) {
				goto malformedJson;
			}
			++i;
			dest = false;
		} else {
			goto malformedJson;
		}

		return skipTrailingSpaces<spacePolicy>(i, end);
	prematureEnd:
		errorHandler.prematureEnd();
		return end;
	malformedJson:
		errorHandler.malformedJson(i);
		return end;
	}

	// TODO define noexcept
	template<typename Iterator, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseColon(Iterator begin, Iterator end, ErrorHandler &errorHandler)
	{
		Iterator i = skipLeadingSpaces<spacePolicy>(begin, end);
		if (unlikely(i == end)) {
			errorHandler.prematureEnd();
			return end;
		}
		if (unlikely(*i != u8":"[0])) {
			errorHandler.malformedJson(i);
			return i;
		}
		return skipTrailingSpaces<spacePolicy>(++i, end);
	}

	template<typename Iterator, typename CharDestination, typename ErrorHandler>
	inline const char *parseCharsToUTF8(Iterator begin, Iterator end, CharDestination dest, ErrorHandler &errorHandler)
	{
		Iterator i = begin;
		if (unlikely(i == end)) {
			goto prematureEnd;
		}

		do {
			char c = *i;

			if (c == u8"\""[0]) {
				return i;
			}

			if (likely(c != u8"\\"[0])) {
				goto callDest;
			}

			++i;
			if (unlikely(i == end)) {
				goto prematureEnd;
			}

			c = *i;
			if (c == u8"\""[0] || c == u8"\\"[0] || c == u8"/"[0]) {
				// c is already valid.
				goto callDest;
			} else if (c == u8"b"[0]) {
				c = u8"\b"[0];
			} else if (c == u8"f"[0]) {
				c = u8"\f"[0];
			} else if (c == u8"n"[0]) {
				c = u8"\n"[0];
			} else if (c == u8"r"[0]) {
				c = u8"\r"[0];
			} else if (c == u8"t"[0]) {
				c = u8"\t"[0];
			} else if (c == u8"u"[0]) {
				// TODO support unicode escapes.
				goto malformedJson;
			} else {
				goto malformedJson;
			}

		callDest:
			dest(c);
		} while (++i != end);

		return i;
	prematureEnd:
		errorHandler.prematureEnd();
		return end;
	malformedJson:
		errorHandler.malformedJson(i);
		return end;
	}
}
}

#endif /* AFC_JSON_HPP_ */
