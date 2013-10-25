#include "string_util.hpp"

using std::string;

bool afc::startsWith(const std::string &str, const std::string &substr) noexcept
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

bool afc::endsWith(const std::string &str, const std::string &substr) noexcept
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
