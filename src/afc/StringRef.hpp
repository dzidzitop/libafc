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
#ifndef AFC_STRINGREF_HPP_
#define AFC_STRINGREF_HPP_

#include <cstddef>

namespace afc
{
	// Allows for efficient processing of string literals by resolving their size at compile time.
	class ConstStringRef
	{
	private:
		friend constexpr ConstStringRef operator"" _s(const char *, std::size_t);

		constexpr ConstStringRef(const char * const str, const std::size_t size) : m_str(str), m_size(size) {}
	public:
		ConstStringRef(const ConstStringRef &) = default;
		ConstStringRef(ConstStringRef &&) = default;
		ConstStringRef &operator=(const ConstStringRef &) = default;
		ConstStringRef &operator=(ConstStringRef &&) = default;
		~ConstStringRef() = default;

		constexpr operator const char *() const { return m_str; }

		constexpr const char *value() const { return m_str; }
		constexpr std::size_t size() const { return m_size; }

		constexpr const char &operator[](const std::size_t i) const { return m_str[i]; };

		constexpr const char *begin() const { return &m_str[0]; };
		constexpr const char *end() const { return &m_str[m_size]; };
	private:
		const char * const m_str;
		const std::size_t m_size;
	};

	constexpr ConstStringRef operator"" _s(const char * const str, const std::size_t n) { return ConstStringRef(str, n); };
}

#endif /* AFC_STRINGREF_HPP_ */
