/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2015 Dźmitry Laŭčuk

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

#ifndef AFC_LOGGER_HPP_
#define AFC_LOGGER_HPP_

#include <cstddef>
#include <cstdio>
#include <initializer_list>
#include <type_traits>

// POSIX API.
#include <stdio.h>

#include "ensure_ascii.hpp" // needed to ensure that '\n' != EOF.
#include "number.h"
#include "SimpleString.hpp"
#include "StringRef.hpp"

namespace afc
{
	namespace logger
	{
		class FileLock
		{
		public:
			FileLock(std::FILE * const file) noexcept : m_file(file) { flockfile(file); }
			~FileLock() { funlockfile(m_file); }
		private:
			std::FILE * const m_file;
		};

		inline bool logText(const char * const s, const std::size_t n, FILE * const dest) noexcept
		{
			return std::fwrite(s, sizeof(char), n, dest) == n;
		}

		template<typename T>
		inline typename std::enable_if<std::is_integral<T>::value, bool>::type logPrint(T value, FILE * const dest) noexcept
		{
			char buf[afc::maxPrintedSize<T, 10>()];
			char *end = afc::printNumber<10>(value, buf);
			return logText(buf, end - buf, dest);
		}

		inline bool logPrint(const char c, FILE * const dest) noexcept
		{
			return std::fputc(c, dest) >= 0;
		}

		inline bool logPrint(const afc::ConstStringRef s, FILE * const dest) noexcept
		{
			return logText(s.value(), s.size(), dest);
		}

		inline bool logPrint(const afc::String &s, FILE * const dest) noexcept
		{
			return logText(s.data(), s.size(), dest);
		}

		inline bool logPrint(const bool b, FILE * const dest) noexcept
		{
			using afc::operator"" _s;
			return logPrint(b ? "true"_s : "false"_s, dest);
		}

		inline bool logPrint(const char * const s, FILE * const dest) noexcept
		{
			return std::fputs(s, dest) >= 0;
		}

		inline bool logPrint(const char * const s, std::size_t n, FILE * const dest) noexcept
		{
			return logText(s, n, dest);
		}

		inline bool logPrint(const std::pair<const char *, const char *> &s, FILE * const dest) noexcept
		{
			return logText(s.first, std::size_t(s.second - s.first), dest);
		}

		template<typename T>
		inline typename std::enable_if<std::is_floating_point<T>::value, bool>::type logPrint(T value, FILE * const dest) noexcept
		{
			// TODO improve performance.
			// TODO handle buffer overflow.
			char buf[128];
			std::size_t n = std::snprintf(buf, 128, "%Lf", static_cast<long double>(value));
			// TODO improve performance.
			return logPrint(buf, n, dest);
		}

		class Printer
		{
		public:
			/* There is no need in the virtual destructor since each Printer instances
			 * are allocated only on the stack.
			 */
			virtual bool operator()(std::FILE *dest) = 0;
		};

		template<typename T>
		class LogPrinter : public Printer
		{
			// The basic types are copied, non-basic are passed by reference.
			typedef typename std::conditional<std::is_pointer<T>::value || std::is_arithmetic<T>::value,
					const T, const T &>::type storage_type;
		public:
			LogPrinter(const T &value) noexcept : m_value(value) {}

			bool operator()(std::FILE *dest) { return logPrint(m_value, dest); }

			LogPrinter *address() noexcept { return this; }
		private:
			storage_type m_value;
		};

		template<typename T>
		inline LogPrinter<typename std::decay<const T>::type> logPrinter(const T &val) noexcept
		{
			return LogPrinter<typename std::decay<const T>::type>(val);
		}

		bool logInternalFmt(const char *format, std::initializer_list<Printer *> params, FILE *dest);

		template<bool flush, typename... Args>
		inline bool logToFileFmt(std::FILE * const dest, const char *format, const Args &...args)
		{ FileLock fileLock(dest);
			/* Passing polymorphic instances of Printer to logInternal.
			 *
			 * All the temporary objects live until logInternal returns so all pointers to
			 * local objects passed to logInternal are valid.
			 */
			return logInternalFmt(format, {logPrinter(args).address()...}, dest) &
					(flush ? std::fflush(dest) != EOF : true);
		}

		inline bool logToFileInternal(FILE *) noexcept { return true; }

		// TODO define noexcept
		template<typename Arg, typename... Args>
		inline bool logToFileInternal(FILE * const dest, const Arg &arg, const Args &...args)
		{
			return logPrint(arg, dest) && logToFileInternal(dest, args...);
		}

		template<bool flush, typename... Args>
		inline bool logToFile(std::FILE * const dest, const Args &...args)
				noexcept(noexcept(logToFileInternal(dest, args...)))
		{ FileLock fileLock(dest);
			return (logToFileInternal(dest, args...) && std::fputc('\n', dest) != EOF) &
					(flush ? std::fflush(dest) != EOF : true);
		}

		#ifdef NDEBUG
			template<typename... Args>
			inline bool logDebug(const Args &...args) noexcept { /* Nothing to do. */ return true; }

			template<typename... Args>
			inline bool logDebugFmt(const char *format, const Args &...args) { /* Nothing to do. */ return true; }
		#else
			template<typename... Args>
			inline bool logDebug(const Args &...args) noexcept(noexcept(logToFile<true>(stdout, args...)))
			{
				return logToFile<true>(stdout, args...);
			}

			template<typename... Args>
			inline bool logDebugFmt(const char * const format, const Args &...args)
			{
				return logToFileFmt<true>(stdout, format, args...);
			}
		#endif

		template<typename... Args>
		inline bool logError(const Args &...args) noexcept(noexcept(logToFile<false>(stderr, args...)))
		{
			return logToFile<false>(stderr, args...);
		}

		template<typename... Args>
		inline bool logErrorFmt(const char * const format, const Args &...args)
		{
			return logToFileFmt<false>(stderr, format, args...);
		}

		#ifdef AFC_TRACE
			template<typename... Args>
			inline bool logTrace(const Args &...args) noexcept(noexcept(logToFile<true>(stdout, args...)))
			{
				return logToFile<true>(stdout, args...);
			}

			template<typename... Args>
			inline bool logTraceFmt(const char * const format, const Args &...args)
			{
				return logToFileFmt<true>(stdout, format, args...);
			}
		#else
			template<typename... Args>
			inline bool logTrace(const Args &...args) noexcept { /* Nothing to do. */ return true; }

			template<typename... Args>
			inline bool logTraceFmt(const char *format, const Args &...args) { /* Nothing to do. */ return true; }
		#endif
	}
}

#endif /* AFC_LOGGER_HPP_ */
