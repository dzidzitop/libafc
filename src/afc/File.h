#ifndef AFC_FILE_H_
#define AFC_FILE_H_

#include <string>
#include <vector>
#include "utils.h"

// FIXME in UNIX files could contain '/' in their names, thus division by separator is wrong
namespace afc
{
	class File
	{
	public:
		// TODO cut separator
		File(const char * const path) {initPath(path); initName(m_path);}
		// TODO cut separator
		File(const std::string &path) {initPath(path); initName(m_path);}
		File(const File &parent, const std::string &name);

		const std::string &path() const throw() {return m_path;}
		// TODO fixme
		std::string absolutePath() const throw() {return m_path;}
		const std::string &name() const throw() {return m_name;}

		// TODO probably separator could be cut right here (faster)
		File parent() const {return File(m_path.substr(0, m_path.size() - m_name.size()));}

		void listFiles(std::vector<std::string> &list);

		unsigned long long size();
		void setSize(const unsigned long long size);

		static const char separator;
	private:
		void initPath(const std::string &path) {m_path = (path.size() != 0 && path.back() == separator ? path.substr(0, path.size() - 1) : path);}
		void initName(const std::string &path) {const size_t pos = path.find_last_of(separator); m_name = (pos == std::string::npos ? path : path.substr(pos + 1));}
		std::string m_path;
		std::string m_name;
	};
}

#endif /*AFC_FILE_H_*/
