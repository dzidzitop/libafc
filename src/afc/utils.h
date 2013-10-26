#ifndef AFC_UTILS_H_
#define AFC_UTILS_H_

#include <string>

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
}

#endif /*AFC_UTILS_H_*/
