#include "utils.h"
#include "platform.h"
#include <cctype>

using std::string;
using std::vector;
using std::isspace;

#ifdef AFC_LINUX
	#include <limits.h>
	#include <unistd.h>
#elif defined AFC_WIN32
	#include <windows.h>
#elif defined AFC_UNIX
	#include <limits.h>
	#define _PSTAT64
	#include <sys/pstat.h>
	#include <sys/types.h>
	#include <unistd.h>
#else
	#error "unsupported system"
#endif

namespace afc
{
#ifdef AFC_LINUX
	std::string getExecPath()
	{
		char result[PATH_MAX];
		ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
		return std::string(result, (count > 0) ? count : 0);
	}
#elif defined AFC_WIN32
	std::string getExecPath()
	{
		char result[MAX_PATH];
		return std::string(result, GetModuleFileName(0, result, MAX_PATH));
	}
#elif defined AFC_UNIX
	std::string getExecPath()
	{
		char result[PATH_MAX];
		struct pst_status ps;
		
		if (pstat_getproc(&ps, sizeof(ps), 0, getpid()) < 0) {
			return std::string();
		}
		if (pstat_getpathname(result, PATH_MAX, &ps.pst_fid_text) < 0) {
			return std::string();
		}
		return std::string(result);
	}
#else
	#error "unsupported system"
#endif
}

void afc::trim(string &s)
{
	size_t n = s.size();
	size_t start = 0;
	for (; start < n && isspace(s[start]); ++start) {/*nothing to do*/}
	size_t count = n - start;
	size_t end = n - 1;
	for (; count > 0 && isspace(s[end]); --end, --count) {/*nothing to do*/}
	if (count != n) {
		s = s.substr(start, count);
	}
}

bool afc::startsWith(const std::string &str, const std::string &substr) throw()
{
	const size_t m = substr.size();
	if (m > str.size()) {
		return false;
	}
	for (size_t i = 0, j = 0; i < m;) {
		if (str[i++] != substr[j++]) {
			return false;
		}
	}
	return true;
}

bool afc::endsWith(const std::string &str, const std::string &substr) throw()
{
	const size_t n = str.size(), m = substr.size();
	if (m > n) {
		return false;
	}
	for (size_t i = n - m, j = 0; i < n;) {
		if (str[i++] != substr[j++]) {
			return false;
		}
	}
	return true;
}

void afc::split(const string &s, vector<string> &out, const char delim, const bool skipEmptyTokens)
{
	out.clear();
	size_t end = 0;

	for (;;) {
		size_t start = end;
		if (skipEmptyTokens) {
			start = s.find_first_not_of(delim, start);
			if (start == string::npos) {
				return;
			}
		}

		end = s.find_first_of(delim, start);
		if (end == string::npos) {
			out.push_back(s.substr(start, s.size()-start));
			return;
		} else {
			out.push_back(s.substr(start, end-start));
			++end;
		}
	}
}
