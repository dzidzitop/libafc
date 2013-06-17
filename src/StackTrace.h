#ifndef AFC_STACKTRACE_H_
#define AFC_STACKTRACE_H_

#include <string>
#include <vector>
#include <iostream>
#include <memory>

namespace afc
{
	using std::string;
	using std::vector;
	using std::ostream;
	using std::endl;
	using std::clog;

	class StackTraceElement;

	class StackTrace
	{
	friend ostream &operator<<(ostream &out, const StackTrace &stackTrace);
	public:
		explicit StackTrace(const unsigned topFramesToSkip = 0) throw();
		~StackTrace() throw();

		bool hasInfo() const throw() {return m_success;}

		void print(ostream &out = clog, const string &linePrefix = emptyString) const;

		static const unsigned STACK_TRACE_DEPTH;
	private:
#ifdef AFC_USE_STACK_TRACE
		int backtrace(void ** const addresses, const std::size_t maxCount, const unsigned topFramesToSkip = 0) throw();
#endif
		// TODO m_elements' constructor could throw exceptions which violates the constructor's throw() declaration
		vector<StackTraceElement *> m_elements;
		bool m_success;

		static const string emptyString;

		/* Addresses to process. Static to eliminate problems in case of low memory available. */
		// TODO FIXME make it non-static to make it working for the multi-threaded programs, too
		static void * rawStackTrace[];
	};

	inline ostream &operator<<(ostream &out, const StackTrace &stackTrace)
	{
		stackTrace.print(out);
		return out;
	}

	// TODO fill stack trace only when it is needed. Addresses is always enough to get textual descriptions of the frames.
	class StackTraceElement
	{
	friend class StackTrace;
	public:
		// Nothing is copied. 'function' is expected to be a non-shared C-string allocated by malloc.
		StackTraceElement(const string &function, const void * const address, const int offset,
				const std::shared_ptr<string> file, const unsigned line)
			: m_function(function), m_address(address), m_offset(offset), m_file(file), m_line(line){}
		~StackTraceElement() throw() {};

		static const unsigned NO_LINE;
	private:
		const string m_function;
		const void * const m_address;
		const int m_offset;
		const std::shared_ptr<string> m_file;
		const unsigned m_line;
	};
}

#endif /*AFC_STACKTRACE_H_*/
