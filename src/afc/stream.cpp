#include "stream.h"
#include "Exception.h"
#include <string>
#include <sstream>

using namespace afc;
using namespace std;

namespace
{
	// 128 is small enough to not consume too much of the stack and quite large to minimise the amount of invocations
	static const size_t gZipSkipChunkSize = 128;

	void throwIOException(const char * const message = "")
	{
		throw IOException(message);
	}

	void throwCannotOpenFileIOException(const File &file)
	{
		throwIOException(("unable to open file '" + file.absolutePath() + '\'').c_str());
	}

	void throwIllegalStateException(const char * const message = "")
	{
		throw IllegalStateException(message);
	}

	void throwMalformedFormatException(const char * const message = "")
	{
		throw MalformedFormatException(message);
	}

	void throwUnsupportedFormatException(const char * const message = "")
	{
		throw UnsupportedFormatException(message);
	}

	void throwInvalidArgumentException(const char * const message = "")
	{
		throw InvalidArgumentException(message);
	}

	inline void ensureNotClosed(FILE * const file) {
		if (file == 0) {
			throwIOException("stream is closed");
		}
	}

	inline void ensureNotClosed(gzFile file) {
		if (file == 0) {
			throwIOException("stream is closed");
		}
	}
}

afc::FileInputStream::FileInputStream(const File &file)
{
	m_file = fopen(file.path().c_str(), "rb");
	if (m_file == 0) {
		throwCannotOpenFileIOException(file);
	}
}

size_t afc::FileInputStream::read(unsigned char * const data, const size_t n)
{
	ensureNotClosed(m_file);
	const size_t count = fread(data, sizeof(unsigned char), n, m_file);
	if (count != n) {
		if (ferror(m_file)) {
			throwIOException("error encountered while reading from file");
		}
	}
	return count;
}

void afc::FileInputStream::reset()
{
	ensureNotClosed(m_file);
	if (fseek(m_file, 0, SEEK_SET) != 0) {
		throwIOException("unable to reset stream");
	}
}

size_t afc::FileInputStream::skip(const size_t n)
{
	ensureNotClosed(m_file);
	const long currPos = ftell(m_file);
	if (fseek(m_file, 0, SEEK_END) != 0) {
		throwIOException("unable to skip data in stream");
	}
	const long endPos = ftell(m_file);
	const size_t tail = endPos - currPos;
	if (n >= tail) {
		return tail;
	}
	if (fseek(m_file, currPos + n, SEEK_SET) != 0) {
		throwIOException("unable to skip data in stream");
	}
	return n;
}

void afc::FileInputStream::close()
{
	if (m_file == 0) {
		return;
	}
	if (fclose(m_file) != 0) {
		throwIOException("file is not closed");
	}
	m_file = 0;
}

afc::FileOutputStream::FileOutputStream(const File &file)
{
	m_file = fopen(file.path().c_str(), "wb");
	if (m_file == 0) {
		throwCannotOpenFileIOException(file);
	}
}

void afc::FileOutputStream::write(const unsigned char * const data, const size_t n)
{
	ensureNotClosed(m_file);
	if (fwrite(data, sizeof(unsigned char), n, m_file) != n) {
		throwIOException("error encountered while writting to file");
	}
}

void afc::FileOutputStream::close()
{
	if (m_file == 0) {
		return;
	}
	if (fclose(m_file) != 0) {
		throwIOException("file is not closed");
	}
	m_file = 0;
}

afc::GZipFileInputStream::GZipFileInputStream(const File &file)
{
	m_file = gzopen(file.path().c_str(), "rb");
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
			throwIOException(msg);
		default:
			throwMalformedFormatException(msg);
		}
	}
	return count;
}

void afc::GZipFileInputStream::close()
{
	if (m_file == nullptr) {
		return;
	}
	if (gzclose(m_file) != 0) {
		throwIOException("file is not closed");
	}
	m_file = nullptr;
}

void afc::GZipFileInputStream::reset()
{
	ensureNotClosed(m_file);
	if (gzseek(m_file, 0, SEEK_SET) != 0) {
		throwIOException("unable to reset stream");
	}
}

size_t afc::GZipFileInputStream::skip(const size_t n)
{
	if (n < 0) {
		// TODO replace later with string += n;
		stringstream msg;
		msg << "Byte amount to skip must be a non-negative value: " << n;
		throwInvalidArgumentException(msg.str().c_str());
	}

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

afc::GZipFileOutputStream::GZipFileOutputStream(const File &file)
{
	m_file = gzopen(file.path().c_str(), "wb");
	if (m_file == 0) {
		throwCannotOpenFileIOException(file);
	}
}

void afc::GZipFileOutputStream::write(const unsigned char * const data, const size_t n)
{
	ensureNotClosed(m_file);
	if (gzwrite(m_file, data, n) == 0) {
		throwIOException("error encountered while writing to file");
	}
}

void afc::GZipFileOutputStream::close()
{
	if (m_file == 0) {
		return;
	}
	if (gzclose(m_file) != 0) {
		throwIOException("file is not closed");
	}
	m_file = 0;
}
