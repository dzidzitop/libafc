/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2013 Dźmitry Laŭčuk

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
#ifndef AFC_FAST_DIVISION_H_
#define AFC_FAST_DIVISION_H_

#include "compile_time_math.h"
#include <cassert>
#include <climits>

namespace afc
{
	// TODO add more comments and a reference to the paper used.

	template<unsigned divisor> inline unsigned divide(const unsigned dividend) throw();
	// division by 0 has no sense.
	template<> unsigned divide<0>(const unsigned dividend) throw();

	/**
	 * Implementation of
	 * while ((m_low>>1) < (m_high>>1) && sh_post > 0) {
	 * 	--sh_post;
	 * 	m_high >>= 1;
	 * 	m_low >>= 1;
	 * }
	 */
	template<unsigned long long m_low, unsigned long long m_high, int sh_post> class __UIntDivReductor
	{
	template<unsigned long long, unsigned long long, int> friend class __UIntDivReductor;
	template<unsigned> friend class __UIntDiv;
		static const unsigned long long m_low_result = ((m_low>>1) < (m_high>>1) && sh_post > 0) ?
															__UIntDivReductor<(m_low>>1), (m_high>>1), sh_post-1>::m_low_result :
															m_low;
		static const unsigned long long m_high_result = ((m_low>>1) < (m_high>>1) && sh_post > 0) ?
															__UIntDivReductor<(m_low>>1), (m_high>>1), sh_post-1>::m_high_result :
															m_high;
		static const unsigned long long sh_post_result = ((m_low>>1) < (m_high>>1) && sh_post > 0) ?
															__UIntDivReductor<(m_low>>1), (m_high>>1), sh_post-1>::sh_post_result :
															sh_post;
	};
	// Boundary case for __IntDivReductor to prevent infinite recursion
	template<int sh_post> struct __UIntDivReductor<0, 0, sh_post>
	{
	template<unsigned long long, unsigned long long, int> friend class __UIntDivReductor;
	template<unsigned> friend class __UIntDiv;
		static const unsigned long long m_low_result = 0, m_high_result = 0;
		static const unsigned long long sh_post_result = sh_post;
	};

	template<unsigned divisor> struct __UIntDiv
	{
	friend unsigned divide<divisor>(const unsigned) throw();
		static const unsigned N = BitCount<UINT_MAX>::result+1;
		static const unsigned long long pow_N = 1ull << N;
		static const unsigned prec = N;
		static const unsigned l = Log2<divisor>::ceil;
		static const unsigned long long m_low = (1ull<<(N+l)) / divisor;
		static const unsigned long long m_high = ((1ull<<(N+l)) + (1ull<<(N+l-prec))) / divisor;
		static const unsigned long long m = __UIntDivReductor<m_low, m_high, l>::m_high_result;
		static const int sh_post = __UIntDivReductor<m_low, m_high, l>::sh_post_result;
	};
}

// TODO compare with GCC-generated code for division by a constant.
template<unsigned divisor> inline unsigned afc::divide(const unsigned dividend) throw()
{
	typedef __UIntDiv<divisor> U;
	// TODO this could be calculated at compile time.
	unsigned sh_pre;

	// TODO include this into assert for the m > pow_N case.
	// TODO
	if (U::m > U::pow_N && (divisor&1) == 0) {
		// FIXME
		sh_pre = 0;
	} else {
		sh_pre = 0;
	}

	// TODO if (d == 2^l) else

	if (U::m > U::pow_N) {
		assert(sh_pre == 0);
		unsigned t1 = static_cast<unsigned>(((U::m - U::pow_N) * dividend) >> U::N);
		return (t1 + ((dividend-t1)>>1)) >> (U::sh_post-1);
	} else {
		return static_cast<unsigned>((U::m * (dividend>>sh_pre))>>U::N)>>U::sh_post;
	}
}

#endif /*AFC_FAST_DIVISION_H_*/
