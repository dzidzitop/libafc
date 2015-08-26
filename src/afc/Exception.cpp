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
#include "Exception.h"

#include <typeinfo>
#include <memory>
#include <cstdlib>
#include <functional>
#include <ostream>
#include "platform.h"
#include "_demangle.h"

using std::unique_ptr;
using std::free;
using std::function;

namespace afc
{
	void Exception::printStackTrace(std::ostream &out) const
	{
		using std::operator<<;

		const Exception &ex = *this;
		{ unique_ptr<char, function<void (void * const)>> demangledName(demangle(typeid(ex).name()), free);
			if (demangledName.get() != 0) {
				out << demangledName.get() << ": ";
			}
		}
		out << what();
#ifdef AFC_USE_STACK_TRACE
		if (m_stackTrace->hasInfo()) {
			out << " at:\n";
			m_stackTrace->print(out, "\t");
		}
#else
		out << '\n';
#endif
		// TODO uncomment after cause exceptions support is implemented
		/*if (m_cause.get() != 0) {
			out << "caused by " << *m_cause;
		}*/
	}
}
