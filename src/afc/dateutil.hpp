/* libafc - utils to facilitate C++ development.
Copyright (C) 2013-2015 Dźmitry Laŭčuk

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

#include <cstddef>
#include <ctime>
#include <cstdint>
#include "number.h"

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
			/* Resetting the time zone to the system default one.
			 *
			 * ::timezone contains the negated GMT offset. Note that it is defined
			 * for POSIX systems. Non-POSIX systems are not supported.
			 */
			m_gmtOffset = -::timezone;

			Timestamp::operator=(timestamp);
			return *this;
		}

		explicit operator std::tm() const noexcept
		{
			/* 1) ::mktime() normalises ::tm but is very slow. Normalising it indirectly.
			 * 2) For this, first shift the timestamp towards m_gmtOffset, then obtain
			 *    the UTC decomposition of this time and then compensate the time shift
			 *    by setting the correct time zone (i.e. m_gmtOffset).
			 * 3) After that the time is naturally normalised, since no modifications
			 *    other than setting the valid time zone offset are performed on the components.
			 * 4) It is unclear completely how this works together with leap seconds.
			 *    If this does not support them then we are in trouble.
			 */
			::time_t t = static_cast< ::time_t >(*this);
			t += ::time_t(m_gmtOffset);

			::tm dateTime;
			::gmtime_r(&t, &dateTime);

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

		void setYear(const long year) noexcept { m_tm.tm_year = year - 1900; }
		long getYear() const noexcept { return m_tm.tm_year + 1900; }
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

	// TODO support expanded ISO date format (create another function for this).
	bool parseISODateTime(const char *begin, const char *end, TimestampTZ &dest);
	bool parseISODateTime(const char *str, TimestampTZ &dest);

	bool parseISODateTime(const char *str, std::time_t &dest);
	bool parseISODateTime(const char *str, DateTime &dest);

	constexpr std::size_t maxISODateTimeSize() noexcept
	{
		return sizeof("-XX-XXTXX:XX:XX+XXXX") - 1 + afc::maxPrintedSize<decltype(std::tm::tm_year), 10>();
	}

	template<typename Iterator>
	Iterator formatISODateTime(const TimestampTZ &time, Iterator dest);

	inline Timestamp now()
	{
		/* This implementation works only for POSIX-compatible systems that store time in
		 * std::time_t as the number of seconds since epoch.
		 */
		return Timestamp(static_cast<Timestamp::time_type>(::time(nullptr)) * 1000);
	}

	namespace helper
	{
		struct TimeZoneInit
		{
			static const bool initialised;
		};

		template<typename T, typename Iterator>
		inline Iterator printISOYear(const T year, Iterator dest)
		{
			if (year >= 0) {
				if (year <= 9999) {
					dest = afc::printFourDigits(year, dest);
				} else {
					*dest++ = '+'; // The expanded year representation is used.
					dest = afc::printNumber<10>(year, dest);
				}
			} else {
				if (year <= -9999) {
					*dest++ = '-';
					dest = afc::printFourDigits(-year, dest);
				} else {
					dest = afc::printNumber<10>(year, dest);
				}
			}
			return dest;
		}
	}
}

template<typename Iterator>
Iterator afc::formatISODateTime(const afc::TimestampTZ &time, Iterator dest)
{
	std::tm t = static_cast<std::tm>(time); // Normalised value.
	assert(t.tm_mon >= 0 && t.tm_mon <= 11);
	assert(t.tm_mday >= 1 && t.tm_mday <= 31);
	assert(t.tm_hour >= 0 && t.tm_hour <= 23);
	assert(t.tm_min >= 0 && t.tm_min <= 59);
	assert(t.tm_sec >= 0 && t.tm_sec <= 60); // 60 is for leap seconds.

	dest = afc::helper::printISOYear(t.tm_year + 1900, dest);
	*dest++ = '-';
	dest = afc::printTwoDigits(t.tm_mon + 1, dest);
	*dest++ = '-';
	dest = afc::printTwoDigits(t.tm_mday, dest);
	*dest++ = 'T';
	dest = afc::printTwoDigits(t.tm_hour, dest);
	*dest++ = ':';
	dest = afc::printTwoDigits(t.tm_min, dest);
	*dest++ = ':';
	dest = afc::printTwoDigits(t.tm_sec, dest);

	// The time zone offset format is +hhmm or -hhmm.
	std::int_fast16_t offsetHoursMinutes = t.tm_gmtoff / 60;
	std::int_fast16_t offsetHours = offsetHoursMinutes / 60;
	std::int_fast16_t offsetMinutes = offsetHoursMinutes - offsetHours * 60;
	// TODO think what to do if gmtoff has three-digit hour value.
	offsetHours %= 100;

	*dest++ = offsetHours >= 0 ? '+' : '-';
	dest = afc::printTwoDigits(offsetHours, dest);
	dest = afc::printTwoDigits(offsetMinutes, dest);
	return dest;
}

#endif /* AFCDATEUTIL_HPP_ */

