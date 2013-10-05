#ifndef AFC_UTILS_H_
#define AFC_UTILS_H_

#include <string>
#include <vector>
#include "math_utils.h"
#include "Exception.h"

namespace afc
{
	std::string getExecPath();

	void trim(std::string &s);
	bool startsWith(const std::string &str, const std::string &substr) throw();
	bool endsWith(const std::string &str, const std::string &substr) throw();

	std::string systemCharset(void);

	std::string convertToUtf8(const char *src, const char *encoding);
	std::string convertToUtf8(const std::string &src, const char *encoding);
	std::string convertFromUtf8(const std::string &stc, const char *encoding);

	// code points have platform endianness, while characters are little-endian
	std::u16string stringToUTF16LE(const char * const src, const char * const encoding);
	std::u16string stringToUTF16LE(const std::string &src, const char * const encoding);
	std::string utf16leToString(const std::u16string &src, const char * const encoding);


	template<typename CharType = char> class Tokeniser
	{
		Tokeniser(const Tokeniser &) = delete;
		Tokeniser(Tokeniser &&) = delete;
		Tokeniser &operator=(const Tokeniser &) = delete;
		Tokeniser &operator=(Tokeniser &&) = delete;
	public:
		Tokeniser(const std::basic_string<CharType> &str, const CharType delimiter)
			: m_str(str), m_delimiter(delimiter), m_begin(0) {}
		Tokeniser(const std::basic_string<CharType> &&str, const CharType delimiter)
			: m_inputCopy(str), m_str(m_inputCopy), m_delimiter(delimiter), m_begin(0) {}

		bool hasNext() const throw() {return m_begin != std::basic_string<CharType>::npos;}
		std::basic_string<CharType> next();
	private:
		/* m_inputCopy is used only with temporary strings because they die early.
		   In other cases no copying of the input string is used. */
		const std::basic_string<CharType> m_inputCopy;
		const std::basic_string<CharType> &m_str;
		const CharType m_delimiter;
		size_t m_begin;
	};

	template<typename T, typename Less> bool binarySearch(const std::vector<T> &vector, const T &val, Less less, size_t &pos);

	template<typename T> inline void deleteElements(const std::vector<T *> &v);
}

template<typename CharType> std::basic_string<CharType> afc::Tokeniser<CharType>::next()
{
	if (m_begin == std::basic_string<CharType>::npos) {
		throwException<IllegalStateException>("Tokeniser::next is called when it has not more tokens");
	}
	size_t end = m_str.find(m_delimiter, m_begin);
	if (end == std::basic_string<CharType>::npos) {
		auto token = m_str.substr(m_begin);
		m_begin = std::basic_string<CharType>::npos;
		return token;
	} else {
		auto token = m_str.substr(m_begin, end - m_begin);
		m_begin = end + 1;
		return token;
	}
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
