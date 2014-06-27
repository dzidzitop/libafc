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
#include <type_traits>

namespace afc
{
	// Allows for efficient processing of string literals by resolving their size at compile time.
	template<typename CharType>
	class StringRefBase
	{
	private:
		// Facilitates type casting from StringRefBase<CharType> to StringRefBase<const CharType>.
		friend class StringRefBase<typename std::remove_const<CharType>::type>;
	public:
		/* TODO It is public only because GCC 4.7 does not seem to support friend user-defined literals.
		 * Make it private with granting access to operator"" _s.
		 */
		constexpr StringRefBase(CharType * const str, const size_t size) : m_str(str), m_size(size) {}
	public:
		StringRefBase(const StringRefBase &) = default;
		StringRefBase(StringRefBase &&) = default;
		StringRefBase &operator=(const StringRefBase &) = default;
		StringRefBase &operator=(StringRefBase &&) = default;
		~StringRefBase() = default;

		template<size_t n>
		constexpr StringRefBase(CharType (&str)[n]) : m_str(str), m_size(n - 1) {}

		constexpr operator StringRefBase<const CharType>() const
		{
			return StringRefBase<const CharType>(m_str, m_size);
		}

		explicit operator CharType *() { return m_str; }
		constexpr explicit operator const CharType *() const { return m_str; }

		CharType *value() { return m_str; }
		constexpr const CharType *value() const { return m_str; }
		constexpr size_t size() const { return m_size; }

		CharType &operator[](const size_t i) { return m_str[i]; };
		constexpr const CharType &operator[](const size_t i) const { return m_str[i]; };

		CharType *begin() { return &m_str[0]; };
		constexpr const CharType *begin() const { return &m_str[0]; };
		CharType *end() { return &m_str[m_size]; };
		constexpr const CharType *end() const { return &m_str[m_size]; };
	private:
		CharType *m_str;
		const size_t m_size;
	};

	typedef StringRefBase<char> StringRef;
	typedef StringRefBase<const char> ConstStringRef;

	constexpr ConstStringRef operator"" _s(const char * const str, const size_t n) { return ConstStringRef(str, n); };
}

#endif /* AFC_STRINGREF_HPP_ */
