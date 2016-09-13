/* libafc - utils to facilitate C++ development.
Copyright (C) 2011-2016 Dźmitry Laŭčuk

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
#include "stream.h"

#include <cassert>
#include <cstddef>
#include <cstring>
#include <functional>
#include <utility>

#include "Exception.h"
#include "FastStringBuffer.hpp"
#include "StringRef.hpp"

using namespace afc;
using namespace std;

namespace
{
	// 128 is small enough to not consume too much of the stack and quite large to minimise the amount of invocations
	static const size_t gZipSkipChunkSize = 128;

	void throwException(const char * const message)
	{
		throw Exception(afc::String(message));
	}

	void throwException(ConstStringRef message)
	{
		throw Exception(message);
	}

	void throwCannotOpenFileIOException(const char * const file)
	{
		const std::size_t fileSize = std::strlen(file);
		const std::size_t bufSize = "unable to open file '"_s.size() + fileSize + 1;

		afc::FastStringBuffer<char, afc::AllocMode::accurate> buf(bufSize);
		buf.append("unable to open file '"_s);
		buf.append(file, fileSize);
		buf.append('\'');

		assert(bufSize == buf.size());

		throw Exception(afc::String::move(buf));
	}

	template<typename FileType>
	inline void ensureNotClosed(const FileType file) {
		if (file == nullptr) {
			throwException("Stream is closed"_s);
		}
	}

	template<typename FileType, typename CloseFunction>
	inline void closeFileRef(FileType *&file, CloseFunction close)
	{
		if (file == nullptr) {
			return;
		}
		if (close(file) != 0) {
			throwException("Unable to close file"_s);
		}
		file = nullptr;
	}

	template<typename FileType, typename CloseFunction>
	inline void closeFileNoexcept(FileType * const file, CloseFunction close) noexcept
	{
		if (file == nullptr) {
			return;
		}
		close(file); // ignoring any potential fclose failure
	}
}

afc::FileInputStream::FileInputStream(const char * const file)
{
	m_file = fopen(file, "rb");
	if (m_file == nullptr) {
		throwCannotOpenFileIOException(file);
	}
}

size_t afc::FileInputStream::read(unsigned char * const data, const size_t n)
{
	ensureNotClosed(m_file);
	const size_t count = fread(data, sizeof(unsigned char), n, m_file);
	if (count != n) {
		if (ferror(m_file)) {
			throwException("error encountered while reading from file"_s);
		}
	}
	return count;
}

void afc::FileInputStream::reset()
{
	ensureNotClosed(m_file);
	if (fseek(m_file, 0, SEEK_SET) != 0) {
		throwException("unable to reset stream"_s);
	}
}

size_t afc::FileInputStream::skip(const size_t n)
{
	ensureNotClosed(m_file);
	const long currPos = ftell(m_file);
	if (fseek(m_file, 0, SEEK_END) != 0) {
		throwException("unable to skip data in stream"_s);
	}
	const long endPos = ftell(m_file);
	const size_t tail = endPos - currPos;
	if (n >= tail) {
		return tail;
	}
	if (fseek(m_file, currPos + n, SEEK_SET) != 0) {
		throwException("unable to skip data in stream"_s);
	}
	return n;
}

void afc::FileInputStream::close()
{
	closeFileRef(m_file, function<int (FILE *)>(fclose));
}

afc::FileInputStream::~FileInputStream()
{
	closeFileNoexcept(m_file, function<int (FILE *)>(fclose));
}

afc::FileOutputStream::FileOutputStream(const char * const file)
{
	m_file = fopen(file, "wb");
	if (m_file == nullptr) {
		throwCannotOpenFileIOException(file);
	}
}

void afc::FileOutputStream::write(const unsigned char * const data, const size_t n)
{
	ensureNotClosed(m_file);
	if (fwrite(data, sizeof(unsigned char), n, m_file) != n) {
		throwException("error encountered while writting to file"_s);
	}
}

void afc::FileOutputStream::close()
{
	closeFileRef(m_file, function<int (FILE *)>(fclose));
}

afc::FileOutputStream::~FileOutputStream()
{
	closeFileNoexcept(m_file, function<int (FILE *)>(fclose));
}

afc::GZipFileInputStream::GZipFileInputStream(const char * const file)
{
	m_file = gzopen(file, "rb");
	if (m_file == 0) {
		throwCannotOpenFileIOException(file);
	}
}

// TODO process closed stream correctly
// TODO handle negative n
size_t afc::GZipFileInputStream::read(unsigned char * const buf, const size_t n)
{
	ensureNotClosed(m_file);
	const size_t count = gzread(m_file, buf, n);
	if (count != n) {
		int errorCode;
		const char * const msg = gzerror(m_file, &errorCode);
		switch (errorCode) {
		case Z_OK:
			break;
		case Z_ERRNO:
		default:
			throwException(msg);
		}
	}
	return count;
}

void afc::GZipFileInputStream::close()
{
	closeFileRef(m_file, function<int (gzFile)>(gzclose));
}

afc::GZipFileInputStream::~GZipFileInputStream()
{
	closeFileNoexcept(m_file, function<int (gzFile)>(gzclose));
}

void afc::GZipFileInputStream::reset()
{
	ensureNotClosed(m_file);
	if (gzseek(m_file, 0, SEEK_SET) != 0) {
		throwException("unable to reset stream"_s);
	}
}

size_t afc::GZipFileInputStream::skip(const size_t n)
{
	// reading n bytes since gzseek does not allow for skipping less than n bytes in case of premature end of the file
	unsigned char buf[gZipSkipChunkSize];
	size_t skipped = 0;
	size_t bytesLeft = n;
	for (; bytesLeft > gZipSkipChunkSize; bytesLeft -= gZipSkipChunkSize) {
		const size_t count = read(buf, gZipSkipChunkSize);
		skipped += count;
		if (count != gZipSkipChunkSize) {
			return skipped;
		}
	}
	skipped += read(buf, bytesLeft);
	return skipped;
}

afc::GZipFileOutputStream::GZipFileOutputStream(const char * const file)
{
	// TODO Make compression level configurable. Return level from 9 to default once this is implemented.
	m_file = gzopen(file, "wb9");
	if (m_file == 0) {
		throwCannotOpenFileIOException(file);
	}
}

void afc::GZipFileOutputStream::write(const unsigned char * const data, const size_t n)
{
	ensureNotClosed(m_file);
	if (gzwrite(m_file, data, n) == 0) {
		throwException("error encountered while writing to file"_s);
	}
}

void afc::GZipFileOutputStream::close()
{
	closeFileRef(m_file, function<int (gzFile)>(gzclose));
}

afc::GZipFileOutputStream::~GZipFileOutputStream()
{
	closeFileNoexcept(m_file, function<int (gzFile)>(gzclose));
}
