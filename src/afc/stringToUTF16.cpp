#include "utils.h"
#include <iconv.h>
#include <errno.h>
#include <climits>
#include <cstddef>
#include <cstring>
#include <algorithm>

#include "Exception.h"
#include "cpu/primitive.h"

using namespace afc;
using namespace std;

namespace
{
	const endianness LE = endianness::LE;

	// RAII iconv wrapper
	class Iconv
	{
	public:
		Iconv(const char * const destEncoding, const char * const srcEncoding)
		{
			ctx = iconv_open(destEncoding, srcEncoding);
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
					throw IllegalStateException(string("The conversion from ") + srcEncoding +
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

namespace {
	u16string _stringToUTF16LE(const char * const src, const size_t n, const char * const encoding)
	{
		Iconv conv("UTF-16LE", encoding);
		char * srcBuf = const_cast<char *>(src); // for some reason iconv takes non-const source buffer
		size_t srcSize = n;
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
}

u16string afc::stringToUTF16LE(const char * const src, const char * const encoding)
{
	// TODO handle null pointer?
	return _stringToUTF16LE(src, strlen(src), encoding);
}

u16string afc::stringToUTF16LE(const string &src, const char * const encoding)
{
	if (src.empty()) {
		return u16string();
	}
	return _stringToUTF16LE(src.c_str(), src.size(), encoding);
}

string afc::utf16leToString(const u16string &src, const char * const encoding)
{
	Iconv conv(encoding, "UTF-16LE");

	string result;

	char srcBuf[4];
	char *mutableSrcBuf = srcBuf;
	size_t srcCharsLeft;
	char destBuf[8]; // TODO support encodings that can take more than 8 bytes to encode a character
	char *mutableDestBuf = destBuf;
	size_t destCharsLeft;
	for (size_t i = 0, n = src.size(); i < n; ++i, mutableSrcBuf = srcBuf, mutableDestBuf = destBuf) {
		const char16_t c = src[i];
		UInt16<>(c).toBytes<LE>(srcBuf);
		if (c < 0xd800) { // plain character
			srcCharsLeft = 2;
		} else if (c < 0xdc00) { // high surrogate
			if (++i < n) {
				const char16_t lowSurrogate = src[i];
				if (lowSurrogate >= 0xdc00 && lowSurrogate <= 0xdfff) {
					UInt16<>(lowSurrogate).toBytes<LE>(srcBuf+2);
					srcCharsLeft = 4;
				} else {
					goto handleMalformedSequence;
				}
			} else {
				goto handleMalformedSequence;
			}
		} else { // low surrogate
			// TODO support char16_t that contain more than 16 bits?
			goto handleMalformedSequence;
		}
		destCharsLeft = 8;
		conv(&mutableSrcBuf, &srcCharsLeft, &mutableDestBuf, &destCharsLeft);
		for_each(destBuf, mutableDestBuf, [&result](const char byte){result.push_back(byte);});
	}
	return result;
handleMalformedSequence:
	throw MalformedFormatException("Unsupported character sequence");
}
