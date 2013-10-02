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
		inline ~Repository() throw();

		inline const T &get(const T &val);
		inline bool remove(const T &val);

		const size_t size() const throw() {return m_values.size();}
		const bool empty() const throw() {return m_values.empty();}
		inline void clear();
	private:
		struct RealLess
		{
			bool operator()(const T * const o1, const T * const o2) throw() {return less(*o1, *o2);}

			Less less;
		};
		typedef std::set<const T *, RealLess> Set;
		Set m_values;
	};
}

template<typename T, typename Less> afc::Repository<T, Less>::~Repository() throw()
{
	clear();
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
	for (typename Set::const_iterator p = m_values.begin(), q = m_values.end(); p != q; ++p) {
		delete *p;
	}
	m_values.clear();
}

#endif /*AFC_REPOSITORY_H_*/
