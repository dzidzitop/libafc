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
#ifndef AFC_UTILS_H_
#define AFC_UTILS_H_

#include <string>
#include <langinfo.h>
#include "ensure_ascii.hpp"
#include <cassert>
#include <mutex>

namespace afc
{
	std::string getExecPath();
	std::string getHomeDir();
	std::string getDataDir();

	inline std::string systemCharset(void) { return std::string(nl_langinfo(CODESET)); }

	std::string convertToUtf8(const char *src, const char *encoding);
	std::string convertToUtf8(const std::string &src, const char *encoding);
	std::string convertFromUtf8(const char *src, const char *encoding);
	std::string convertFromUtf8(const std::string &src, const char *encoding);

	// code points have platform endianness, while characters are little-endian
	std::u16string stringToUTF16LE(const char * const src, const char * const encoding);
	std::u16string stringToUTF16LE(const std::string &src, const char * const encoding);
	std::string utf16leToString(const std::u16string &src, const char * const encoding);

	// A hex digit must be passed in.
	inline char toHex(const char c)
	{
		assert(c >= 0 && c < 16);
		if (c < 10) {
			return (char) ('0' + c);
		} else {
			return (char) ('a' + c - 10);
		}
	}

	struct UnlockGuard
	{
		UnlockGuard(std::mutex &mutex) : m_mutex(mutex) { m_mutex.unlock(); };
		~UnlockGuard() { m_mutex.lock(); }
	private:
		UnlockGuard(const UnlockGuard &) = delete;
		UnlockGuard(UnlockGuard &&) = delete;
		UnlockGuard &operator=(const UnlockGuard &) = delete;
		UnlockGuard &operator=(UnlockGuard &&) = delete;

		std::mutex &m_mutex;
	};
}

#endif /*AFC_UTILS_H_*/
