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
#ifndef AFC_SEARCH_H_
#define AFC_SEARCH_H_

#include <vector>

namespace afc
{
	template<typename T, typename F> struct SearchSpace
	{
		virtual ~SearchSpace() {};

		virtual unsigned dimensionCount() const throw() = 0;
		virtual T lowerBound(const unsigned dimension) const throw() = 0;
		virtual T upperBound(const unsigned dimension) const throw() = 0;

		virtual F value(const std::vector<T> &state) const throw() = 0;
	};

	template<typename T, typename F> struct SearchAlgorithm
	{
		virtual void solve(const SearchSpace<T, F> &space, std::vector<T> &solution) = 0;
	};

	template<typename T, typename F> struct ExhaustiveSearch
	{
		virtual void solve(const SearchSpace<T, F> &space, std::vector<T> &solution);
	};

	template<typename T, typename F> class RandomStartHillClimbing : public SearchAlgorithm<T, F>
	{
	public:
		RandomStartHillClimbing(const unsigned attemptCount, const unsigned maxSteps)
			: m_attemptCount(attemptCount), m_maxSteps(maxSteps) {}

		void solve(const SearchSpace<T, F> &space, std::vector<T> &solution);
	private:
		const unsigned m_attemptCount;
		const unsigned m_maxSteps;
	};
}

#include "search_algorithm_impl.icpp"

#endif /*AFC_SEARCH_H_*/
