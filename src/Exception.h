#ifndef AFC_EXCEPTION_H_
#define AFC_EXCEPTION_H_

#include <exception>
#include <stdexcept>
#include <ostream>
#include <string>
#include <memory>
#include "StackTrace.h"

namespace afc
{
	class Exception : public virtual std::exception
	{
	template<typename T> friend void throwExceptionConcat(T &);
	public:
		// TODO add real support for specifying cause (root) exceptions. 'cause' is not used
		Exception(const string &what, const Exception * const cause);
		Exception(const Exception * const cause) : Exception(string(), cause) {};
		Exception(const Exception &ex)
#ifdef AFC_USE_STACK_TRACE
			: m_stackTrace(ex.m_stackTrace), m_message(ex.m_message) {}
#else
			: m_message(ex.m_message) {}
#endif
		~Exception() throw() {}

		virtual const char *what() const throw() {return m_message.c_str();}

		void printStackTrace(std::ostream& out = std::cerr) const;
	private:
#ifdef AFC_USE_STACK_TRACE
		std::shared_ptr<const StackTrace> m_stackTrace;
#endif
		const string m_message;
	};

	inline ostream &operator<<(ostream &out, const Exception &ex)
	{
		ex.printStackTrace(out);
		return out;
	}

	struct NullPointerException : public Exception, public std::logic_error
	{
		NullPointerException(const Exception * const cause = 0)
			: Exception(cause), std::logic_error("") {}
		NullPointerException(const string &what, const Exception * const cause = 0)
			: Exception(what, cause), std::logic_error(what) {}
	};

	struct ParseException : public Exception
	{
		ParseException(const Exception * const cause = 0)
			: Exception(cause) {}
		ParseException(const string &what, const Exception * const cause = 0)
			: Exception(what, cause) {}
	};

	struct MalformedFormatException : public Exception
	{
		MalformedFormatException(const Exception * const cause = 0)
			: Exception(cause) {}
		MalformedFormatException(const string &what, const Exception * const cause = 0)
			: Exception(what, cause) {}
	};

	struct UnsupportedFormatException : public Exception
	{
		UnsupportedFormatException(const Exception * const cause = 0)
			: Exception(cause) {}
		UnsupportedFormatException(const string &what, const Exception * const cause = 0)
			: Exception(what, cause) {}
	};

	struct InvalidArgumentException : public Exception, public std::invalid_argument
	{
		InvalidArgumentException(const Exception * const cause = 0)
			: Exception(cause), std::invalid_argument("") {}
		InvalidArgumentException(const string &what, const Exception * const cause = 0)
			: Exception(what, cause), std::invalid_argument(what) {}
	};

	struct RangeException : public Exception, public std::range_error
	{
		RangeException(const Exception * const cause = 0)
			: Exception(cause), std::range_error("") {}
		RangeException(const string &what, const Exception * const cause = 0)
			: Exception(what, cause), std::range_error(what) {}
	};

	struct OverflowException : public RangeException, public std::overflow_error
	{
		OverflowException(const Exception * const cause = 0)
			: RangeException(cause), std::overflow_error("") {}
		OverflowException(const string &what, const Exception * const cause = 0)
			: RangeException(what, cause), std::overflow_error(what) {}
	};

	struct UnderflowException : public RangeException, public std::underflow_error
	{
		UnderflowException(const Exception * const cause = 0)
			: RangeException(cause), std::underflow_error("") {}
		UnderflowException(const string &what, const Exception * const cause = 0)
			: RangeException(what, cause), std::underflow_error(what) {}
	};

	struct IOException : public Exception
	{
		IOException(const Exception * const cause = 0)
			: Exception(cause) {}
		IOException(const string &what, const Exception * const cause = 0)
			: Exception(what, cause) {}
	};

	struct IllegalStateException : public Exception
	{
		IllegalStateException(const Exception * const cause = 0)
			: Exception(cause) {}
		IllegalStateException(const string &what, const Exception * const cause = 0)
			: Exception(what, cause) {}
	};

	template<typename Ex> void throwException();
	template<typename Ex, typename Arg> void throwException(const Arg &arg);
	template<typename Ex, typename Arg1, typename Arg2> void throwExceptionConcat(const Arg1 &arg1, const Arg2 &arg2);
}

#endif /*AFC_EXCEPTION_H_*/
