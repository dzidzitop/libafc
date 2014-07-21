/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2014 Dźmitry Laŭčuk

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
#ifndef AFCDATEUTIL_HPP_
#define AFCDATEUTIL_HPP_
#include <ctime>
#include <string>
#include <cstdint>

/* getCurrentUTCTimeSeconds() relies upon posix-compatible format of std::time_t.
 * Each POSIX implementation must have unistd.h available.
 */
#include <unistd.h>
#include <time.h>

namespace afc
{
	class Timestamp
	{
	public:
		typedef std::int_fast64_t time_type;

		// Creates a Timestamp in an undefined state.
		explicit Timestamp() {}

		explicit Timestamp(const time_type millis) : m_millis(millis) {}

		Timestamp &operator=(const ::time_t timestamp) noexcept
		{
			m_millis = static_cast<time_type>(timestamp) * 1000;
			return *this;
		}

		explicit operator ::time_t() const noexcept { return static_cast<std::time_t>(m_millis / 1000); }

		void setMillis(const time_type millis) noexcept { m_millis = millis; }
		time_type millis() const noexcept { return m_millis; }
	private:
		time_type m_millis;
	};

	class TimestampTZ : public Timestamp
	{
	public:
		// Creates a Timestamp in an undefined state.
		explicit TimestampTZ() {}

		TimestampTZ &operator=(const ::time_t timestamp) noexcept
		{
			// Resetting the time zone to the system default one.
			// ::timezone is defined for POSIX systems. Non-POSIX systems are not supported.
			::tzset();
			m_gmtOffset = ::timezone;

			Timestamp::operator=(timestamp);
			return *this;
		}

		explicit operator std::tm() const noexcept
		{
			::time_t t = static_cast< ::time_t >(*this);
			::tm dateTime;
			/* Initialises the system time zone data. According to POSIX.1-2004, localtime() is required
			 * to behave as though tzset(3) was called, while localtime_r() does not have this requirement.
			 */
			::gmtime_r(&t, &dateTime);

			/* Flushing time to UTC and normalising it as local time. Setting the correct
			 * time zone afterwards.
			 * It is unclear completely why it works, but it is.
			 */
			dateTime.tm_sec += m_gmtOffset;
			dateTime.tm_gmtoff = 0;
			mktime(&dateTime);

			dateTime.tm_gmtoff = m_gmtOffset;

			return dateTime;
		}

		// In seconds.
		void setGmtOffset(const long gmtOffset) noexcept { m_gmtOffset = gmtOffset; }
		long getGmtOffset() const noexcept { return m_gmtOffset; }
	private:
		long m_gmtOffset;
	};

	class DateTime
	{
	public:
		DateTime() noexcept
		{
			m_tm.tm_year = 0;
			m_tm.tm_mon = 0; // The first month.
			m_tm.tm_mday = 0;
			m_tm.tm_hour = 0;
			m_tm.tm_min = 0;
			m_tm.tm_sec = 0;
			m_tm.tm_isdst = -1; // Let environment to determine if this time is DST.
			m_millisecond = 0;
		}

		DateTime(const std::tm &dateTime) noexcept : m_tm(dateTime) {}

		DateTime &operator=(const std::time_t timestamp) noexcept
		{
			/* Initialises the system time zone data. According to POSIX.1-2004, localtime() is required
			 * to behave as though tzset(3) was called, while localtime_r() does not have this requirement.
			 */
			::tzset();
			::localtime_r(&timestamp, &m_tm);

			return *this;
		}

		DateTime &operator=(const std::tm &dateTime) noexcept
		{
			m_tm = dateTime;
			return *this;
		}

		explicit operator std::tm() const noexcept
		{
			return m_tm;
		}

		Timestamp timestamp() const noexcept
		{
			::tm t = m_tm; // must make a copy of tm since mktime potentially modifies its argument.

			/* This implementation works only for POSIX-compatible systems that store time in
			 * std::time_t as the number of seconds since epoch.
			 */
			return Timestamp(static_cast<Timestamp::time_type>(::mktime(&t)) * 1000);
		}

		void setYear(const long year) noexcept { m_tm.tm_year = year; }
		long getYear() const noexcept { return m_tm.tm_year; }
		// The first month is 1.
		void setMonth(const unsigned month) noexcept { m_tm.tm_mon = month - 1; }
		unsigned getMonth() const noexcept { return m_tm.tm_mon + 1; }
		void setDay(const unsigned day) noexcept { m_tm.tm_mday = day; }
		unsigned getDay() const noexcept { return m_tm.tm_mday; }
		void setHour(const unsigned hour) noexcept { m_tm.tm_hour = hour; }
		unsigned getHour() const noexcept { return m_tm.tm_hour; }
		void setMinute(const unsigned minute) noexcept { m_tm.tm_min = minute; }
		unsigned getMinute() const noexcept { return m_tm.tm_min; }
		void setSecond(const unsigned second) noexcept { m_tm.tm_sec = second; }
		unsigned getSecond() const noexcept { return m_tm.tm_sec; }
		void setMillisecond(const unsigned millisecond) noexcept { m_millisecond = millisecond; }
		unsigned getMillisecond() const noexcept { return m_millisecond; }

		// In seconds.
		void setGmtOffet(const long gmtOffset) noexcept
		{
			// Note: tm_gmtoff is not a part of the standard C++11.
			m_tm.tm_gmtoff = gmtOffset;
		}

		long getGmtOffet() const noexcept
		{
			// Note: tm_gmtoff is not a part of the standard C++11.
			return m_tm.tm_gmtoff;
		}
	private:
		// Note: tm_gmtoff is not a part of the standard C++11.
		::tm m_tm;

		unsigned m_millisecond;
	};

	// a utf-8 string is expected
	bool parseISODateTime(const std::string &str, std::time_t &dest);

	// a utf-8 string is expected
	bool parseISODateTime(const std::string &str, DateTime &dest);

	// a utf-8 string is expected
	bool parseISODateTime(const std::string &str, TimestampTZ &dest);

	inline Timestamp now()
	{
		/* This implementation works only for POSIX-compatible systems that store time in
		 * std::time_t as the number of seconds since epoch.
		 */
		return Timestamp(static_cast<Timestamp::time_type>(::time(nullptr)) * 1000);
	}
}

#endif /* AFCDATEUTIL_HPP_ */

