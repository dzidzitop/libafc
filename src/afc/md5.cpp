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
#include "md5.hpp"
#include <openssl/md5.h>
#include <cassert>
#include "ensure_ascii.hpp"

using namespace std;

namespace
{
	// A hex digit must be passed in.
	inline char toHex(const char b)
	{
		assert(b >= 0 && b < 16);
		if (b < 10) {
			return (char) ('0' + b);
		} else {
			return (char) ('a' + b - 10);
		}
	}
}

void afc::md5String(const unsigned char * const data, const size_t n, string &dest)
{
	unsigned char hash[MD5_DIGEST_LENGTH];
	MD5(data, n, hash);

	dest.reserve(dest.size() + MD5_DIGEST_LENGTH * 2);

	for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		const unsigned char b = hash[i];
		dest.push_back(toHex(b & 0xff >> 4));
		dest.push_back(toHex(b & 0xf));
	}
}
