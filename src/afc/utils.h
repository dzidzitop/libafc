#ifndef AFC_UTILS_H_
#define AFC_UTILS_H_

#include <string>
#include <vector>
#include "math_utils.h"
#include "Exception.h"

namespace afc
{
	std::string getExecPath();
	std::string getHomeDir();
	std::string getDataDir();

	std::string systemCharset(void);

	std::string convertToUtf8(const char *src, const char *encoding);
	std::string convertToUtf8(const std::string &src, const char *encoding);
	std::string convertFromUtf8(const char *stc, const char *encoding);
	std::string convertFromUtf8(const std::string &stc, const char *encoding);

	// code points have platform endianness, while characters are little-endian
	std::u16string stringToUTF16LE(const char * const src, const char * const encoding);
	std::u16string stringToUTF16LE(const std::string &src, const char * const encoding);
	std::string utf16leToString(const std::u16string &src, const char * const encoding);

	template<typename T, typename Less> bool binarySearch(const std::vector<T> &vector, const T &val, Less less, size_t &pos);

	template<typename T> inline void deleteElements(const std::vector<T *> &v);
}

template<typename T, typename Less> bool afc::binarySearch(const std::vector<T> &vector, const T &val, Less less, size_t &pos)
{
	int lower = 0, upper = vector.size()-1;
	while (lower <= upper){
		const size_t mid = mean(lower, upper);
		const T &x = vector[mid];
		if (less(val, x)) {
			upper = mid-1;
		} else if (less(x, val)) {
			lower = mid+1;
		} else { // x == val
			pos = mid;
			return true;
		}
	}
	return false;
}

template <typename T> inline void afc::deleteElements(std::vector<T *> &v)
{
	for (typename std::vector<T>::size_type i = 0, n = v.size(); i < n; ++i) {
		delete v[i];
	}
}

#endif /*AFC_UTILS_H_*/
