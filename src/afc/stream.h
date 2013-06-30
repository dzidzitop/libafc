#ifndef STREAM_H_
#define STREAM_H_

#include <cstdio>
#include <zlib.h>

namespace afc
{
	struct Closeable
	{
		virtual ~Closeable() {};

		virtual void close() = 0;
	};

	struct InputStream : public Closeable
	{
		virtual ~InputStream() {};

		virtual size_t read(unsigned char * const data, const size_t n) = 0;
		virtual void reset() = 0;
		virtual size_t skip(const size_t n) = 0;
	};

	struct OutputStream
	{
		virtual ~OutputStream() {};

		virtual void write(const unsigned char * const data, const size_t n) = 0;
	};

	class FileInputStream : public InputStream
	{
	public:
		FileInputStream(const char * const file);
		~FileInputStream();

		virtual size_t read(unsigned char * const data, const size_t n);
		virtual void reset();
		virtual size_t skip(const size_t n);
		virtual void close();
	private:
		std::FILE *m_file;
	};

	class FileOutputStream : public OutputStream
	{
	public:
		FileOutputStream(const char * const file);
		~FileOutputStream();

		virtual void write(const unsigned char * const data, const size_t n);
		virtual void close();
	private:
		std::FILE *m_file;
	};

	class GZipFileInputStream : public InputStream
	{
	public:
		GZipFileInputStream(const char * const file);
		~GZipFileInputStream();

		virtual size_t read(unsigned char * const buf, const size_t n);
		virtual void reset();
		virtual size_t skip(const size_t n);

		virtual void close();
	private:
		gzFile m_file;
	};

	class GZipFileOutputStream : public OutputStream
	{
	public:
		GZipFileOutputStream(const char * const file);
		~GZipFileOutputStream();

		virtual void write(const unsigned char * const data, const size_t n);

		virtual void close();
	private:
		static const int COMPRESS_LEVEL = 9;
		gzFile m_file;
	};
}

#endif /*STREAM_H_*/
