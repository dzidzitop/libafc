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
#include "StackTrace.h"
#include <cstdlib>
#include <cstring>
#include <map>
#include <cassert>
#include <stdexcept>
#include <memory>

#include "platform.h"
#include "_demangle.h"

// TODO remove
#include <iostream>
// TODO remove
using std::cout;

#ifdef __GNUC__
	#ifdef AFC_STACK_TRACE_DEPTH
		#include <bfd.h>
	#endif
	#include <sys/stat.h>
	#include "utils.h"
#elif defined AFC_UNIX
	#error "what to include?"
#elif defined AFC_WIN32
	#error "not implemented yet"
#endif


using std::malloc;
using std::free;
using std::strlen;
using std::map;
using std::auto_ptr;
using std::endl;
using std::pair;
using std::ios_base;
using std::ios;
using std::strchr;
using std::strrchr;
//TODO remove this!
using std::cout;

namespace afc
{
#ifdef AFC_STACK_TRACE_DEPTH
	StackTrace::STACK_TRACE_DEPTH = AFC_STACK_TRACE_DEPTH;
#else
	const unsigned StackTrace::STACK_TRACE_DEPTH = 512;
#endif
	const unsigned StackTraceElement::NO_LINE = 0;

	const string StackTrace::emptyString;

#ifdef __GNUC__
	void * StackTrace::rawStackTrace[STACK_TRACE_DEPTH];
#endif

#if !defined AFC_WIN32_MINGW && defined AFC_WIN32
	#ifdef AFC_FUNCTION_NAME_MAX_LENGTH
		const unsigned StackTrace::FUNCTION_NAME_MAX_LENGTH = AFC_FUNCTION_NAME_MAX_LENGTH;
	#else
		const unsigned StackTrace::FUNCTION_NAME_MAX_LENGTH = 512;
	#endif

	IMAGEHLP_SYMBOL* const afc::StackTrace::imagehlpSymbolPtr = afc::StackTrace::initImagehlpSymbol();

	IMAGEHLP_SYMBOL* afc::StackTrace::initImagehlpSymbol()
	{
		IMAGEHLP_SYMBOL* imageSymbolPtr = reinterpret_cast<IMAGEHLP_SYMBOL*>(malloc(sizeof(IMAGEHLP_SYMBOL) + FUNCTION_NAME_MAX_LENGTH));
		memset(imageSymbolPtr, 0, sizeof(IMAGEHLP_SYMBOL));
		imageSymbolPtr->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
		imageSymbolPtr->MaxNameLength = FUNCTION_NAME_MAX_LENGTH;
		return imageSymbolPtr;
	}
#endif

	StackTrace::StackTrace(const unsigned topFramesToSkip) throw()
	{
#ifdef AFC_USE_STACK_TRACE
		// TODO FIXME handle allocation errors
		// topFramesToSkip + 1 in order to skip the frame related to the StackTrace constructor itself. This information is useless for an user.
		const size_t actualDepth = backtrace(rawStackTrace, STACK_TRACE_DEPTH, topFramesToSkip + 1);
		vector<AddrStatus> symbols;
		symbols.reserve(actualDepth);
		backtraceSymbols(rawStackTrace, actualDepth, symbols);
		for (size_t i = 0; i < actualDepth; ++i) {
			const AddrStatus elem = symbols[i];
			auto_ptr<StackTraceElement> entry;
			if (elem.success) {
				entry.reset(new StackTraceElement(elem.functionName, rawStackTrace[i], 0, elem.fileName, elem.line));
			} else {
				entry.reset(new StackTraceElement("", rawStackTrace[i], 0, 0, StackTraceElement::NO_LINE));
			}
			m_elements.push_back(entry.get());
			entry.release();
		}
		m_success = true;
#else
		m_success = false;
#endif
	}

	StackTrace::~StackTrace(void) throw()
	{
		for (size_t i = 0, n = m_elements.size(); i < n; ++i) {
			delete m_elements[i];
		}
	}

	void StackTrace::print(ostream &out, const string &linePrefix) const
	{
		if (!m_success) {
			out << "<stack trace is unavailable>";
			return;
		}
		const ios_base::fmtflags backup = out.flags();
		for (int i = 0, n = m_elements.size(); i < n; ++i) {
			out << linePrefix;
			assert(m_elements[i] != 0);
			const StackTraceElement &element = *m_elements[i];
			out << (!element.m_function.empty() ? element.m_function : "<unknown>") << " <" << element.m_address;
			if (element.m_offset != 0) {
				out.flags(ios::hex);
				assert(element.m_offset > 0);
				out << '+' << element.m_offset;
			}
			const string * const fileNamePtr = element.m_file.get();
			out << ">\tat " << (fileNamePtr == nullptr ? "<unknown source>" : *fileNamePtr);
			if (element.m_line != StackTraceElement::NO_LINE) {
				out.flags(ios::dec);
				out << ':' << element.m_line;
			}
			out << '\n';
		}
		out.flags(backup);
	}
}
