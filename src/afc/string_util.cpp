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
#include "string_util.hpp"

using std::string;

bool afc::startsWith(const std::string &str, const std::string &substr) noexcept
{
	const size_t m = substr.size();
	if (m > str.size()) {
		return false;
	}
	for (size_t i = 0, j = 0; i < m;) {
		if (str[i++] != substr[j++]) {
			return false;
		}
	}
	return true;
}

bool afc::endsWith(const std::string &str, const std::string &substr) noexcept
{
	const size_t n = str.size(), m = substr.size();
	if (m > n) {
		return false;
	}
	for (size_t i = n - m, j = 0; i < n;) {
		if (str[i++] != substr[j++]) {
			return false;
		}
	}
	return true;
}
