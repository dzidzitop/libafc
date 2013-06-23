#include "platform.h"
#include "File.h"
#include <dirent.h>
#include "Exception.h"
#include <cerrno>

using std::vector;
using std::string;
using std::size_t;

#ifdef AFC_WIN32
	const string afc::File::separator = "\\";
#else
	const string afc::File::separator = "/";
#endif

// TODO cut separator
afc::File::File(const File &parent, const std::string &name)
{
	const size_t nameSize = endsWith(name, separator) ? name.size() - separator.size() : name.size();
	m_path.reserve(parent.m_path.size() + separator.size() + nameSize);
	m_path.append(parent.m_path).append(separator).append(name, 0, nameSize);
	initName(m_path);
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
