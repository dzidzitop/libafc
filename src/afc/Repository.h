/* libafc - utils to facilitate C++ development.
Copyright (C) 2010-2013 Dźmitry Laŭčuk

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
#ifndef AFC_REPOSITORY_H_
#define AFC_REPOSITORY_H_

#include <set>
#include <memory>
#include <functional>

namespace afc
{
	template<typename T, typename Less = std::less<T>> class Repository
	{
		Repository(const Repository &) = delete;
		Repository(Repository &&) = delete;
		Repository &operator=(const Repository &) = delete;
		Repository &operator=(Repository &&) = delete;
	public:
		Repository() : m_values() {}
		~Repository() throw() { clear(); };

		inline const T &get(const T &val);
		inline bool remove(const T &val);

		const size_t size() const throw() { return m_values.size(); }
		const bool empty() const throw() { return m_values.empty(); }
		inline void clear();
	private:
		struct RealLess
		{
			bool operator()(const T * const o1, const T * const o2) throw() { return less(*o1, *o2); }

			Less less;
		};
		typedef std::set<const T *, RealLess> Set;
		Set m_values;
	};
}

template<typename T, typename Less> const T &afc::Repository<T, Less>::get(const T &val)
{
	typename Set::const_iterator p = m_values.find(&val);
	if (p == m_values.end()) {
		std::auto_ptr<T> entry(new T(val));
		m_values.insert(entry.get());
		return *entry.release();
	} else {
		return **p;
	}
}

template<typename T, typename Less> bool afc::Repository<T, Less>::remove(const T &val)
{
	typename Set::const_iterator p = m_values.find(&val);
	if (p == m_values.end()) {
		return false;
	}
	delete *p;
	m_values.erase(p);
	return true;
}

template<typename T, typename Less> void afc::Repository<T, Less>::clear()
{
	for (const T * const val : m_values) {
		delete val;
	}
	m_values.clear();
}

#endif /*AFC_REPOSITORY_H_*/
