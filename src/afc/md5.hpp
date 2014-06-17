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
#ifndef AFCMD5_HPP_
#define AFCMD5_HPP_
#include <ctime>
#include <string>
#include <cstddef>

namespace afc
{
	/*
	 * Calculates the MD5 hash of a given binary data and writes it in the lower-case hex format
	 * as an ASCII sequence to the destination string.
	 *
	 * @param data the binary data whose MD5 hash is to be calculated.
	 * @param n the size of the binary data.
	 * @param dest the destination string. It is expected to be ASCII-compatible.
	 */
	void md5String(const unsigned char *data, size_t n, std::string &dest);
}

#endif /* AFCMD5_HPP_ */

