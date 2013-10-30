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
#ifndef AFC_STOPWATCH_H_
#define AFC_STOPWATCH_H_

#include <ctime>
#include <iomanip>
#include <iostream>

namespace afc
{
	using std::clock_t;
	using std::clock;
	using std::ostream;
	using std::cout;
	using std::endl;
	using std::setiosflags;
	using std::ios_base;

	class Stopwatch
	{
	public:
		Stopwatch() throw() : m_started(false), m_stopped(false), m_accumulator(0) {}
		~Stopwatch() throw() {}

		Stopwatch &reset() throw()
		{
			m_started = m_stopped = false;
			m_accumulator = 0;
			return *this;
		}

		void start() throw()
		{
			m_stopped = false;
			m_started = true;
			m_accumulator = 0;
			m_start = clock();
		}

		Stopwatch &stop() throw()
		{
			if (m_started) {
				m_accumulator += durationSinceLastResume();
				m_stopped = true;
			}
			return *this;
		}

		void resume() throw()
		{
			m_stopped = false;
			m_start = clock();
		}

		Stopwatch &print(ostream &out = cout)
		{
			if (m_started) {
				const clock_t totalTime = m_stopped ? m_accumulator : m_accumulator + durationSinceLastResume();
				ios_base::fmtflags old = cout.flags(ios_base::fixed);
				cout << static_cast<double>(totalTime)/CLOCKS_PER_SEC << "s" << endl;
				cout.flags(old);
			} else {
				out << "Not started" << endl;
			}
			return *this;
		}
	private:
		inline clock_t durationSinceLastResume() throw() {return clock() - m_start;}

		bool m_started, m_stopped;
		clock_t m_start, m_accumulator;
	};
}

#endif /*AFC_STOPWATCH_H_*/
