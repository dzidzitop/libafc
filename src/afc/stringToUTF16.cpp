#include "utils.h"
#include <iconv.h>
#include <errno.h>
#include <climits>
#include <cstddef>

#include "Exception.h"
#include "cpu/primitive.h"

using namespace afc;
using namespace std;

namespace
{
	// RAII iconv wrapper
	class Iconv
	{
	public:
		Iconv(const string &srcEncoding)
		{
			ctx = iconv_open("UTF-16LE", srcEncoding.c_str());
			if (ctx == reinterpret_cast<iconv_t>(-1)) {
				const int err = errno;
				switch (err) {
				case EMFILE:
					throw IllegalStateException("Maximum allowed number of files descriptors are currently open by this process.");
				case ENFILE:
					throw IllegalStateException("Too many files are currently open in the system.");
				case ENOMEM:
					throw IllegalStateException("Insufficient storage space is available.");
				case EINVAL:
					throw IllegalStateException("The conversion from " + srcEncoding +
							" to UTF-16LE is not supported by the implementation.");
				default:
					throw IllegalStateException("Unable to initialise encoding context. errno: " + err);
				}
			}
		}

		~Iconv()
		{
			iconv_close(ctx);
		}

		size_t operator()(char ** const srcBuf, size_t * const srcBytesLeft, char ** const destBuf, size_t * const destBytesLeft)
		{
			const size_t count = iconv(ctx, srcBuf, srcBytesLeft, destBuf, destBytesLeft);
			if (count == static_cast<size_t>(-1)) {
				const int err = errno;
				switch (err) {
				case E2BIG:
					throw IllegalStateException("There is not sufficient room at *destBuf");
				case EILSEQ:
					throw MalformedFormatException("An invalid multibyte sequence has been encountered in the input.");
				case EINVAL:
					throw MalformedFormatException("An incomplete multibyte sequence has been encountered in the input.");
				default:
					throw IllegalStateException("Unable to convert *srcBuf. errno: " + err);
				}
			}
			return count;
		}
	private:
		iconv_t ctx;
	};
}

u16string stringToUTF16LE(const string &src, const string &encoding)
{
	if (src.empty()) {
		return u16string();
	}
	Iconv conv(encoding);
	char * srcBuf = const_cast<char *>(src.c_str()); // for some reason iconv takes non-const source buffer
	size_t srcSize = src.size();
	const size_t destSize = 4 * srcSize; // max length of a UTF16-LE character is 4 bytes
	size_t destCharsLeft = destSize;
	unique_ptr<char[]> destBuf(new char[destSize]);
	char * mutableDestBuf = destBuf.get(); // iconv modifies the pointers to the buffers

	conv(&srcBuf, &srcSize, &mutableDestBuf, &destCharsLeft);

	const size_t bufSize = destSize - destCharsLeft;
	if (isOdd(bufSize)) {
		throw MalformedFormatException("Unsupported character sequence");
	}

	// converting the char buffer to u16string
	const char * const buf = destBuf.get();
	u16string result;
	result.reserve(bufSize/2);
	for (size_t i = 0; i < bufSize; i+=2) {
		const char16_t codePoint = UInt16<>::fromBytes<endianness::LE>(&buf[i]); // a UTF16 code point
		result.push_back(codePoint);
	}
	return result;
}
