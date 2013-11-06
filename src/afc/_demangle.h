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
#ifndef AFC_BACKTRACE_H_
#define AFC_BACKTRACE_H_

#include "platform.h"

#include <cstddef>
#include <string>
#include <vector>
#include <memory>

namespace afc
{
	char *demangle(const char * const name) throw();

	struct AddrStatus
	{
		bool success;
		std::shared_ptr<std::string> fileName;
		std::string functionName;
		size_t line;
	};

	bool backtraceSymbols(void ** const addresses, size_t size, std::vector<AddrStatus> &dest) throw();
}

#endif /*AFC_BACKTRACE_H_*/
