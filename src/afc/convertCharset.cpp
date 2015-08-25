/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2015 Dźmitry Laŭčuk

libafc is free software: you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#include "utils.h"
#include <cassert>
#include <iconv.h>
#include <errno.h>
#include <climits>
#include <cstddef>
#include <cstring>

#include "Exception.h"
#include "cpu/primitive.h"
#include "FastStringBuffer.hpp"
#include "math_utils.h"

using namespace afc;

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

		std::size_t operator()(char ** const srcBuf, std::size_t * const srcBytesLeft, char ** const destBuf, std::size_t * const destBytesLeft)
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

afc::String afc::convertToUtf8(const char * const src, const char * const encoding)
{
	assert(src != nullptr);
	assert(encoding != nullptr);

	return convertToUtf8(src, std::strlen(src), encoding);
}

afc::String afc::convertToUtf8(const char * const src, std::size_t n, const char * const encoding)
{
	assert(src != nullptr);
	assert(encoding != nullptr);

	if (n == 0) {
		return afc::String();
	}

	Iconv conv("UTF-8", encoding);
	char *srcBuf = const_cast<char *>(src); // for some reason iconv takes non-const source buffer
	std::size_t srcSize = n;
	const std::size_t destSize = 6 * srcSize; // max length of a UTF-8 character is 6 bytes
	std::size_t destCharsLeft = destSize;
	std::unique_ptr<char[]> destBuf(new char[destSize]);
	char *mutableDestBuf = destBuf.get(); // iconv modifies the pointers to the buffers

	conv(&srcBuf, &srcSize, &mutableDestBuf, &destCharsLeft);

	const std::size_t bufSize = destSize - destCharsLeft;

	return afc::String(destBuf.get(), bufSize);
}

afc::String afc::convertFromUtf8(const char * const src, const char * const encoding)
{
	assert(src != nullptr);
	assert(encoding != nullptr);

	return convertFromUtf8(src, strlen(src), encoding);
}

afc::String afc::convertFromUtf8(const char * const src, const std::size_t n, const char * const encoding)
{
	// TODO remove duplication.
	// TODO optimise memory usage.
	assert(src != nullptr);
	assert(encoding != nullptr);

	if (n == 0) {
		return afc::String();
	}

	Iconv conv(encoding, "UTF-8");
	char *srcBuf = const_cast<char *>(src); // for some reason iconv takes non-const source buffer
	std::size_t srcSize = n;
	const std::size_t destSize = 8 * srcSize; // max length of a character supported is 8 bytes
	std::size_t destCharsLeft = destSize;
	std::unique_ptr<char[]> destBuf(new char[destSize]);
	char *mutableDestBuf = destBuf.get(); // iconv modifies the pointers to the buffers

	conv(&srcBuf, &srcSize, &mutableDestBuf, &destCharsLeft);

	const std::size_t bufSize = destSize - destCharsLeft;

	return afc::String(destBuf.get(), bufSize);
}

afc::U16String afc::stringToUTF16LE(const char * const src, const char * const encoding)
{
	assert(src != nullptr);
	assert(encoding != nullptr);

	return stringToUTF16LE(src, strlen(src), encoding);
}

afc::U16String afc::stringToUTF16LE(const char * const src, std::size_t n, const char * const encoding)
{
	assert(src != nullptr);
	assert(encoding != nullptr);

	if (n == 0) {
		return afc::U16String();
	}

	Iconv conv("UTF-16LE", encoding);
	char *srcBuf = const_cast<char *>(src); // for some reason iconv takes non-const source buffer
	std::size_t srcSize = n;
	const std::size_t destSize = 4 * srcSize; // max length of a UTF16-LE character is 4 bytes
	std::size_t destCharsLeft = destSize;
	std::unique_ptr<char[]> destBuf(new char[destSize]);
	char *mutableDestBuf = destBuf.get(); // iconv modifies the pointers to the buffers

	conv(&srcBuf, &srcSize, &mutableDestBuf, &destCharsLeft);

	const std::size_t bufSize = destSize - destCharsLeft;
	if (isOdd(bufSize)) {
		throw MalformedFormatException("Unsupported character sequence");
	}

	// converting the char buffer to u16string
	const char * const buf = destBuf.get();
	afc::FastStringBuffer<char16_t, afc::AllocMode::accurate> result(bufSize / 2);
	for (std::size_t i = 0; i < bufSize; i+=2) {
		const char16_t codePoint = UInt16<>::fromBytes<endianness::LE>(&buf[i]); // a UTF16 code point
		result.append(codePoint);
	}
	return afc::U16String().attach(result.detach(), bufSize / 2);
}

afc::String afc::utf16leToString(const char16_t * const src, const std::size_t n, const char * const encoding)
{
	Iconv conv(encoding, "UTF-16LE");

	afc::FastStringBuffer<char> result(n); // a reasonable estimate of the result's size.

	char srcBuf[4];
	char *mutableSrcBuf = srcBuf;
	std::size_t srcCharsLeft;
	char destBuf[8]; // TODO support encodings that can take more than 8 bytes to encode a character
	char *mutableDestBuf = destBuf;
	std::size_t destCharsLeft;
	for (std::size_t i = 0; i < n; ++i, mutableSrcBuf = srcBuf, mutableDestBuf = destBuf) {
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
		} else { // either low surrogate or a non-character or > 0xffff
			goto handleMalformedSequence;
		}
		destCharsLeft = 8;
		conv(&mutableSrcBuf, &srcCharsLeft, &mutableDestBuf, &destCharsLeft);
		const std::size_t count = mutableDestBuf - destBuf;
		result.reserve(result.size() + count);
		result.append(destBuf, count);
	}
	{
		const std::size_t bufSize = result.size();
		return afc::String().attach(result.detach(), bufSize);
	}
handleMalformedSequence:
	throw MalformedFormatException("Unsupported character sequence");
}
