/* libafc - utils to facilitate C++ development.
Copyright (C) 2013 Dźmitry Laŭčuk

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
#ifndef AFCDATEUTIL_HPP_
#define AFCDATEUTIL_HPP_
#include <ctime>
#include <string>

namespace afc
{
	// a utf-8 string is expected
	bool parseISODateTime(const std::string &str, std::time_t &dest);
}

#endif /* AFCDATEUTIL_HPP_ */

