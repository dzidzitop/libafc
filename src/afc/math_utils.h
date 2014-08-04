/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2014 Dźmitry Laŭčuk

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
#ifndef AFC_MATH_UTILS_H_
#define AFC_MATH_UTILS_H_

#include <cmath>
#include <climits>
#include <limits>
#include <type_traits>

namespace afc
{
	namespace math {
		template<typename T>
		constexpr T min(T a, T b) { return a < b ? a : b; }

		template<typename T>
		constexpr T max(T a, T b) { return a < b ? b : a; }

		template<typename T>
		T ceilPow2(const T a)
		{
			static_assert(std::is_integral<T>::value, "T must be an integral type.");
			static_assert(std::is_unsigned<T>::value, "T must be an unsigned type.");
			T x = a - 1;
			for (unsigned i = 1; i <= std::numeric_limits<T>::digits / 2; i *= 2) {
				x |= (x >> i);
			}
			return x + 1;
		}
	}

	const unsigned INT_SIGN_BIT_MASK = ~0 ^ INT_MAX;

	inline constexpr bool sameSign(const int i, const int j) throw()
	{
		// TODO (i == 0 || j == 0) ? (i ^ j) == 0 could be better
		return (i == 0 || j == 0) ? (i ^ j) == 0 :
				(i & INT_SIGN_BIT_MASK) ==  (j & INT_SIGN_BIT_MASK);
	}

	const int PARITY_MASK = 0x1;

	inline constexpr bool isEven(const int i) throw()
	{
		return (i & PARITY_MASK) == 0;
	}

	inline constexpr bool isOdd(const int i) throw()
	{
		return (i & PARITY_MASK) != 0;
	}

	template<typename T>
	constexpr T mean(const T x, const T y) noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");
		return std::is_unsigned<T>::value ?
				(x >> 1) + (y >> 1) + (x & y & 1) :
				// TODO implement it more efficiently for signed integers.
				(x > y ? y + (x - y) / 2 : x + (y - x) / 2);
	}

	template<typename T>
	constexpr bool isPow2(const T x) noexcept
	{
		static_assert(std::is_integral<T>::value, "T must be an integral type.");
		typedef typename std::make_unsigned<T>::type uT;
		return x > 0 && (uT(x) & (uT(x) - 1)) == 0;
	}

	// TODO using tables it could work faster
	inline const int intSqrt(const int x) throw()
	{
		return (int) floor(sqrt(x)+0.5);
	}
}

#endif /* AFC_MATH_UTILS_H_ */
