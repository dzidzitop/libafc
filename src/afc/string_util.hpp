/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2015 Dźmitry Laŭčuk

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
#ifndef AFC_STRING_UTIL_HPP_
#define AFC_STRING_UTIL_HPP_

#include <cstddef>
#include <iterator>

namespace afc
{
	// TODO define conditional noexcept;
	template<typename StringIterator, typename SubstringIterator>
	bool endsWith(StringIterator strBegin, StringIterator strEnd,
			SubstringIterator substrBegin, SubstringIterator substrEnd)
	{
		const std::size_t m = std::distance(substrBegin, substrEnd), n = std::distance(strBegin, strEnd);

		if (m > n) {
			return false;
		}

		for (auto p = strBegin + (n - m), q = substrBegin; p != strEnd; ++p, ++q) {
			if (*p != *q) {
				return false;
			}
		}
		return true;
	}
}

#endif /* AFC_STRING_UTIL_HPP_ */
