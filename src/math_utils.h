#ifndef AFC_MATH_UTILS_H_
#define AFC_MATH_UTILS_H_

#include <cmath>
#include <climits>

namespace afc
{
	const unsigned INT_SIGN_BIT_MASK = ~0 ^ INT_MAX;

	inline int abs(const int i) throw()
	{
		return i >= 0 ? i : -i;
	}

	inline bool sameSign(const int i, const int j) throw()
	{
		// TODO could be better
		if (i == 0 || j == 0) {
			return (i ^ j) == 0;
		}
		return (i & INT_SIGN_BIT_MASK) ==  (j & INT_SIGN_BIT_MASK);
	}

	const int PARITY_MASK = 0x1;

	inline bool isEven(const int i) throw()
	{
		return (i & PARITY_MASK) == 0;
	}

	inline bool isOdd(const int i) throw()
	{
		return (i & PARITY_MASK) != 0;
	}

	inline unsigned mean(const unsigned x, const unsigned y) throw()
	{
		// TODO implement in a more optimal way for other CPUs and compilers
		//return (static_cast<unsigned long long>(x) + y)>>1;
		return (x>>1) + (y>>1) + (x&y&1);
	}

	// TODO using tables it could work faster
	inline const int intSqrt(const int x) throw()
	{
		return (int) floor(sqrt(x)+0.5);
	}

	inline const double round(const double val) throw()
	{
	#ifdef __GNUC__
		return ::round(val);
	#else
		return floor(val + 0.5);
	#endif
	}
}

#endif /*AFC_MATH_UTILS_H_*/
