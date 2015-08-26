/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2015 Dźmitry Laŭčuk

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
#ifndef AFC_EXCEPTION_H_
#define AFC_EXCEPTION_H_

#include <cstdlib>
#include <exception>
#include <memory>
#include <stdexcept>
#include <ostream>
#include <memory>
#include <utility>

#include "_demangle.h"
#include "platform.h"
#include "SimpleString.hpp"
#include "StackTrace.h"
#include "StringRef.hpp"

namespace afc
{
	class Exception
	{
		template<typename T> friend void throwExceptionConcat(T &);
	public:
		// TODO add real support for specifying cause (root) exceptions. 'cause' is not used
		explicit Exception(ConstStringRef what, const Exception * const cause = nullptr)
				: Exception(afc::String(what), cause) {}

		explicit Exception(afc::String &&what, const Exception * const cause = nullptr)
#ifdef AFC_USE_STACK_TRACE
				: m_stackTrace(new StackTrace(0)), m_message(std::move(what)) {}#else
#else
				: m_message(std::move(what)) {}
#endif

		explicit Exception(const Exception * const cause) : m_message() {};

		Exception(Exception &&ex)
#ifdef AFC_USE_STACK_TRACE
			: m_stackTrace(ex.m_stackTrace), m_message(std::move(ex.m_message)) {}
#else
			: m_message(std::move(ex.m_message)) {}
#endif
		~Exception() throw() {}

		virtual const char *what() const throw() { return m_message.c_str(); }

		void printStackTrace(std::ostream &out = std::cerr) const;
	private:
#ifdef AFC_USE_STACK_TRACE
		std::shared_ptr<const StackTrace> m_stackTrace;
#endif
		const afc::String m_message;
	};

	template<typename Ex>
	inline ostream &operator<<(ostream &out, const Ex &ex)
	{
		ex.printStackTrace(out);
		return out;
	}
}

#endif /*AFC_EXCEPTION_H_*/
