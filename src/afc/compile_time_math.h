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
#ifndef AFC_COMPILE_TIME_MATH_H_
#define AFC_COMPILE_TIME_MATH_H_

#include <climits>

namespace afc
{
	template<unsigned val> struct BitCount
	{
		enum {result = BitCount<(val>>1)>::result+1};
	};
	template<> struct BitCount<1>
	{
		enum {result = 1};
	};
	template<> struct BitCount<0>
	{
		enum {result = 1};
	};

	template<unsigned val> struct OnesCount
	{
		enum {result = (val&1) + OnesCount<(val>>1)>::result};
	};
	template<> struct OnesCount<0>
	{
		enum {result = 0};
	};

	template<unsigned val> struct LeadZeroCount
	{
	private:
		static const unsigned rightmostOneValue = 1 << (BitCount<UINT_MAX>::result - 1);
	public:
		enum {result = (val&LeadZeroCount::rightmostOneValue) == 0 ? LeadZeroCount<(val>>1)>::result - 1 : 0};
	};
	template<> struct LeadZeroCount<0>
	{
		enum {result = BitCount<UINT_MAX>::result};
	};

	template<unsigned val> struct Log2
	{
		enum
		{
			floor = static_cast<unsigned>(BitCount<UINT_MAX>::result) - 1 - LeadZeroCount<val>::result,
			ceil = static_cast<unsigned>(BitCount<UINT_MAX>::result) - LeadZeroCount<val-1>::result,
		};
	};
	template<> struct Log2<0>;

	template<int x, int y> struct IntMin
	{
		enum {result = x < y ? x : y};
	};

	template<int x, int y> struct IntMax
	{
		enum {result = x < y ? y : x};
	};
}

#endif /*AFC_COMPILE_TIME_MATH_H_*/
