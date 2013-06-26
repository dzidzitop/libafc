#include "platform.h"
#include "File.h"
#include <dirent.h>
#include "Exception.h"
#include <cerrno>

using std::vector;
using std::string;
using std::size_t;

#ifdef AFC_WIN32
	const char afc::File::separator = '\\';
#else
	const char afc::File::separator = '/';
#endif

// TODO cut separator
afc::File::File(const File &parent, const std::string &name)
{
	const size_t nameSize = name.size() != 0 || name.back() == separator ? name.size() - 1 : name.size();
	m_path.reserve(parent.m_path.size() + 1 + nameSize);
	m_path += parent.m_path;
	m_path += separator;
	m_path.append(name, 0, nameSize);
	initName(m_path);
}

afc::File::File(const char * const path)
{
	size_t nameSeparatorPos = static_cast<size_t>(-1);
	size_t lastSeparatorPos = static_cast<size_t>(-1);
	const char * p = path;
	size_t size = 0;
	for (char c; (c = *p); ++p, ++size) {
		if (c == separator) {
			nameSeparatorPos = lastSeparatorPos;
			lastSeparatorPos = size;
		}
	}
	if (size != 0) {
		if (lastSeparatorPos == size - 1) {
			m_path.assign(path, size - 1);
		} else {
			m_path.assign(path, size);
			nameSeparatorPos = lastSeparatorPos; // in foo/bar/baz last separator is before file name
		}
	}
	if (nameSeparatorPos != static_cast<size_t>(-1)) {
		m_name = m_path.substr(nameSeparatorPos + 1);
	} else {
		m_name = m_path;
	}
}

void afc::File::listFiles(vector<string> &list)
{
	DIR * const dir = opendir(m_path.c_str());

	if (dir == 0) {
		/* TODO support this
		 * errno Errors: EACCES means the search permission is denied for the component of the path prefix of dirname or read permission is denied for dirname.
		 * ELOOP means too many symbolic links were encountered in resolving path. ENAMETOOLONG means the length of the dirname argument exceeds PATH_MAX, or a pathname component is longer than NAME_MAX.
		 * ENOENT means a component of dirname does not name an existing directory or dirname is an empty string. ENOTDIR means a component of dirname is not a directory.
		 * EMFILE means OPEN_MAX file descriptors are currently open in the calling process. ENAMETOOLONG means the pathname resolution of a symbolic link produced an intermediate result whose length exceeds PATH_MAX.
		 * ENFILE means there are too many files are currently open in the system.
		 */
		throwException<IOException>();
	}

	try {
		list.clear();

		dirent * entry;

		while ((entry = readdir(dir)) != 0) {
			const string name(&entry->d_name[0]);
			if (name.compare(".") != 0 && name.compare("..") != 0) {
				list.push_back(name);
			}
		}

		if (errno != 0) {
			/* TODO support this
			 * errno Errors: EOVERFLOW means one of the values in the structure to be returned cannot be represented correctly. EBADF means dirp does not refer to an open directory stream.
			 * ENOENT means the current position of the directory stream is invalid.
			 */
			throwException<IOException>();
		}
	}
	catch (...) {
		closedir(dir);
		throw;
	}
	closedir(dir);
}
