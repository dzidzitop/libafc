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
#include "assertion.h"
#include "StackTrace.h"
#include <iostream>
#include <exception>

using std::cerr;
using std::endl;
using std::terminate;

#ifndef NDEBUG
	void afc::assertion(const bool assertion, const char * const message)
	{
		if (assertion) {
			return;
		}
		cerr << "Assertion failure";
		if (message != 0) {
			cerr << " (" << message << ')';
		}
		cerr << " at:\n";
		StackTrace(1).print(cerr);
		terminate();
	}
#endif
