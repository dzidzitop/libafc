#ifndef AFC_FILE_H_
#define AFC_FILE_H_

#include <string>
#include <vector>
#include "utils.h"
#include <cstddef>

// FIXME in UNIX files could contain '/' in their names, thus division by separator is wrong
namespace afc
{
	class File
	{
	public:
		// TODO cut separator
		File(const char * const path);
		// TODO cut separator
		File(const std::string &path);
		File(const File &parent, const std::string &subPath);

		const std::string &path() const throw() {return m_path;}
		// TODO fixme
		std::string absolutePath() const throw() {return m_path;}
		const std::string &name() const throw() {return m_name;}

		// TODO probably separator could be cut right here (faster)
		// TODO handle case when this file is root
		File parent() const {return File(m_path.substr(0, m_path.size() - m_name.size()));}

		void listFiles(std::vector<std::string> &list);

		unsigned long long size();
		void setSize(const unsigned long long size);

		static const char separator;
	private:
		inline void initName();

		std::string m_path;
		std::string m_name;
	};
}

#endif /*AFC_FILE_H_*/
