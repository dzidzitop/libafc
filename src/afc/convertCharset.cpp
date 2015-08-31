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
#include "number.h"
#include "StringRef.hpp"

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
					throw Exception("Maximum allowed number of files descriptors are currently open by this process."_s);
				case ENFILE:
					throw Exception("Too many files are currently open in the system."_s);
				case ENOMEM:
					throw Exception("Insufficient storage space is available."_s);
				case EINVAL:
					{
						const std::size_t srcEncodingSize = std::strlen(srcEncoding);
						const std::size_t bufSize = "The conversion from "_s.size() + srcEncodingSize +
								" to UTF-16LE is not supported by the implementation."_s.size();
						afc::FastStringBuffer<char, afc::AllocMode::accurate> buf(bufSize);
						buf.append("The conversion from "_s);
						buf.append(srcEncoding, srcEncodingSize);
						buf.append(" to UTF-16LE is not supported by the implementation."_s);
						throw Exception(String::move(buf));
					}
				default:
					{
						const std::size_t errnoSize = maxPrintedSize<int, 10>();
						const std::size_t bufCapacity = "Unable to initialise encoding context. errno: "_s.size() + errnoSize;
						afc::FastStringBuffer<char, afc::AllocMode::accurate> buf(bufCapacity);
						buf.append("Unable to initialise encoding context. errno: "_s);
						buf.returnTail(afc::printNumber<10>(err, buf.borrowTail()));
						throw Exception(String::move(buf));
					}
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
					throw Exception("There is not sufficient room at *destBuf"_s);
				case EILSEQ:
					throw Exception("An invalid multibyte sequence has been encountered in the input."_s);
				case EINVAL:
					throw Exception("An incomplete multibyte sequence has been encountered in the input."_s);
				default:
					{
						const std::size_t errnoSize = maxPrintedSize<int, 10>();
						const std::size_t bufCapacity = "Unable to convert *srcBuf. errno: "_s.size() + errnoSize;
						afc::FastStringBuffer<char, afc::AllocMode::accurate> buf(bufCapacity);
						buf.append("Unable to convert *srcBuf. errno: "_s);
						buf.returnTail(afc::printNumber<10>(err, buf.borrowTail()));
						throw Exception(String::move(buf));
					}
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
		throw Exception("Unsupported character sequence"_s);
	}

	// converting the char buffer to u16string
	const char * const buf = destBuf.get();
	afc::FastStringBuffer<char16_t, afc::AllocMode::accurate> result(bufSize / 2);
	for (std::size_t i = 0; i < bufSize; i+=2) {
		const char16_t codePoint = UInt16<>::fromBytes<endianness::LE>(&buf[i]); // a UTF16 code point
		result.append(codePoint);
	}
	return afc::U16String::move(result);
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

	return afc::String::move(result);
handleMalformedSequence:
	throw Exception("Unsupported character sequence"_s);
}
