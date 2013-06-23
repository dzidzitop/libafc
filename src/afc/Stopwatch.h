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
