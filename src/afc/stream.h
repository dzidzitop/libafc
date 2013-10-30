/* libafc - utils to facilitate C++ development.
Copyright (C) 2011-2013 Dźmitry Laŭčuk

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
		FileInputStream(FileInputStream &) = delete;
		~FileInputStream();

		void operator=(FileInputStream &) = delete;

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
		FileOutputStream(FileOutputStream &) = delete;
		~FileOutputStream();

		void operator=(FileOutputStream &) = delete;

		virtual void write(const unsigned char * const data, const size_t n);
		virtual void close();
	private:
		std::FILE *m_file;
	};

	class GZipFileInputStream : public InputStream
	{
	public:
		GZipFileInputStream(const char * const file);
		GZipFileInputStream(GZipFileInputStream &) = delete;
		~GZipFileInputStream();

		void operator=(GZipFileInputStream &) = delete;

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
		GZipFileOutputStream(GZipFileOutputStream &) = delete;
		~GZipFileOutputStream();

		void operator=(GZipFileOutputStream &) = delete;

		virtual void write(const unsigned char * const data, const size_t n);

		virtual void close();
	private:
		static const int COMPRESS_LEVEL = 9;
		gzFile m_file;
	};
}

#endif /*STREAM_H_*/
