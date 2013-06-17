#ifndef AFC_BACKTRACE_H_
#define AFC_BACKTRACE_H_

#include "platform.h"

#ifdef AFC_HAS_EXECINFO
	#include <execinfo.h>
#else
	#include <cstddef>
#endif

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
