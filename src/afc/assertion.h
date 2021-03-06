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
#ifndef AFC_ASSERT_H_
#define AFC_ASSERT_H_

#ifdef NDEBUG
	#define AFC_ASSERT(assertion, message) ((void) 0)
#else
	#define AFC_ASSERT(assertion, message) ((assertion) ? ((void) 0): afc::assertion(#assertion, #message))
#endif

namespace afc
{
#ifndef NDEBUG
	void assertion(const bool assertion, const char * const message = 0);
#else
	inline void assertion(const bool assertion, const char * const message = 0) {/*nothing to do*/}
#endif
}

#endif /*AFC_ASSERT_H_*/
