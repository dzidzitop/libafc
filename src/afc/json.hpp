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

#include <afc/builtin.hpp>

namespace afc
{
namespace json
{
	enum SpacePolicy {
		spaces,
		noSpaces,
		noTrailingSpaces,
	};

	// TODO define noexcept
	template<typename Iterator>
	inline Iterator skipSpaces(Iterator begin, Iterator end) {
		return std::find_if(begin, end, [](const char c) { return !std::isspace(c); });
	}

	// TODO define noexcept
	template<typename Iterator, typename ObjectBodyParser, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseObject(Iterator begin, Iterator end, ObjectBodyParser objectBodyParser, ErrorHandler &errorHandler)
	{
		Iterator i = begin;
		if (spacePolicy != noSpaces) {
			i = skipSpaces(i, end);
		}
		if (unlikely(i == end)) {
			errorHandler.prematureEnd();
			return end;
		}
		if (unlikely(*i != u8"{"[0])) {
			errorHandler.malformedJson(i);
			return end;
		}

		i = objectBodyParser(++i, end, errorHandler);
		if (unlikely(!errorHandler.valid())) {
			return end;
		}
		// TODO check performance (excessive branches)
		if (unlikely(i == end)) {
			errorHandler.prematureEnd();
			return end;
		}
		if (unlikely(*i != u8"}"[0])) {
			errorHandler.malformedJson(i);
			return end;
		}
		++i;
		if (spacePolicy == spaces) {
			i = skipSpaces(i, end);
		}
		return i;
	}

	template<typename Iterator, typename ArrayElementParser, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseArray(Iterator begin, Iterator end, ArrayElementParser arrayElementParser, ErrorHandler &errorHandler)
	{
		Iterator i = begin;
		if (spacePolicy != noSpaces) {
			i = skipSpaces(i, end);
		}
		if (unlikely(i == end)) {
			errorHandler.prematureEnd();
			return end;
		}
		if (unlikely(*i != u8"["[0])) {
			errorHandler.malformedJson(i);
			return end;
		}
		if (unlikely(++i == end)) {
			errorHandler.prematureEnd();
			return end;
		}
		if (*i == u8"]"[0]) {
			return ++i;
		}

		for (;;) {
			i = arrayElementParser(i, end, errorHandler);
			if (unlikely(!errorHandler.valid())) {
				return end;
			}
			if (unlikely(i == end)) {
				errorHandler.prematureEnd();
				return end;
			}
			const char c = *i;

			if (c == u8"]"[0]) {
				++i;
				if (spacePolicy == spaces) {
					i = skipSpaces(i, end);
				}
				return i;
			}
			if (likely(c == u8","[0])) {
				++i;
			} else {
				errorHandler.malformedJson(i);
				return end;
			}
		}
	}

	// TODO define noexcept
	template<typename Iterator, typename StringParser, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseString(Iterator begin, Iterator end, StringParser stringParser, ErrorHandler &errorHandler)
	{
		Iterator i = begin;
		if (spacePolicy != noSpaces) {
			i = skipSpaces(i, end);
		}
		if (unlikely(i == end)) {
			return end;
		}
		if (unlikely(*i != u8"\""[0])) {
			errorHandler.malformedJson(i);
			return end;
		}

		i = stringParser(++i, end, errorHandler);
		if (unlikely(!errorHandler.valid())) {
			return end;
		}
		if (unlikely(i == end)) {
			errorHandler.prematureEnd();
			return end;
		}
		if (unlikely(*i != u8"\""[0])) {
			errorHandler.malformedJson(i);
			return end;
		}
		++i;
		if (spacePolicy == spaces) {
			i = skipSpaces(i, end);
		}
		return i;
	}

	// TODO define noexcept
	template<typename Iterator, typename NumberParser, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseNumber(Iterator begin, Iterator end, NumberParser numberParser, ErrorHandler &errorHandler)
	{
		Iterator i = begin;
		if (spacePolicy != noSpaces) {
			i = skipSpaces(i, end);
		}

		i = numberParser(i, end, errorHandler);
		if (unlikely(!errorHandler.valid())) {
			return end;
		}

		// TODO check performance (excessive branches)
		if (spacePolicy == spaces) {
			i = skipSpaces(i, end);
		}
		return i;
	}

	// TODO define noexcept
	template<typename Iterator, typename ErrorHandler, SpacePolicy spacePolicy = spaces>
	inline Iterator parseColon(Iterator begin, Iterator end, ErrorHandler &errorHandler)
	{
		Iterator i = begin;
		if (spacePolicy != noSpaces) {
			i = skipSpaces(i, end);
		}
		if (unlikely(i == end)) {
			errorHandler.prematureEnd();
			return end;
		}
		if (unlikely(*i != u8":"[0])) {
			errorHandler.malformedJson(i);
			return i;
		}
		++i;
		if (spacePolicy == spaces) {
			i = skipSpaces(i, end);
		}
		return i;
	}

	template<typename Iterator, typename CharDestination, typename ErrorHandler>
	inline const char *parseCharsToUTF8(Iterator begin, Iterator end, CharDestination dest, ErrorHandler &errorHandler)
	{
		if (unlikely(begin == end)) {
			errorHandler.prematureEnd();
			return end;
		}

		Iterator i = begin;
		do {
			char c = *i;

			if (c == u8"\""[0]) {
				return i;
			}

			++i;
			if (likely(c != u8"\\"[0])) {
				dest(c);
				continue;
			}

			if (unlikely(i == end)) {
				errorHandler.prematureEnd();
				return end;
			}

			c = *i;
			++i;
			if (c == u8"\""[0] || c == u8"\\"[0] || c == u8"/"[0]) {
				dest(c);
			} else if (c == u8"b"[0]) {
				dest(u8"\b"[0]);
			} else if (c == u8"f"[0]) {
				dest(u8"\f"[0]);
			} else if (c == u8"n"[0]) {
				dest(u8"\n"[0]);
			} else if (c == u8"r"[0]) {
				dest(u8"\r"[0]);
			} else if (c == u8"t"[0]) {
				dest(u8"\t"[0]);
			} else if (c == u8"u"[0]) {
				// TODO support unicode escapes.
				errorHandler.malformedJson(begin);
				return end;
			} else {
				errorHandler.malformedJson(i);
				return end;
			}
		} while (i != end);

		return i;
	}
}
}

#endif /* AFC_JSON_HPP_ */
