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

#include "dateutil.hpp"

#include <time.h>

#include "builtin.hpp"
#include "ensure_ascii.hpp"

using namespace std;

const bool afc::helper::TimeZoneInit::initialised = []() { ::tzset(); return true; }();

namespace
{
	bool parseDateTime(const char * const str, tm &dateTime)
	{
		/* The ASCII-compatible encodings are supported only. The pattern used depends only
		 * on the ASCII subset of UTF-8 so no additional conversion is needed to parse the date.
		 */
		const char * const parseResult = strptime(str, "%FT%T%z", &dateTime);

		if (unlikely(parseResult == nullptr || *parseResult != '\0')) {
			// Either parse error or not the whole string is processed.
			return false;
		}

		/* strptime() does not assign the field tm_isdst. If it appears to be negative
		 * then strftime with %z discards the time zone as non-determined. To avoid
		 * this effect, assigning it explicitly.
		 */
		dateTime.tm_isdst = 0;

		return true;
	}
}

bool afc::parseISODateTime(const char * const str, time_t &dest)
{
	tm dateTime;

	if (!parseDateTime(str, dateTime)) {
		return false;
	}

	/* 1. Flush time zone offset into seconds to make a UTC time.
	 * 2. Make a local value out of the tm structure with the UTC time. It is shifted towards future by
	 *    amount of negated ::timezone.
	 * 3. Make a time_t value out of the tm structure with the local time.
	 *
	 * Unfortunately, this code is not portable. It compiles in Debian Wheezy with GCC 4.7.
	 * In addition, this code cannot tolerate the system time zone changed in the middle of processing.
	 */
	dateTime.tm_sec -= dateTime.tm_gmtoff;
	dateTime.tm_sec -= timezone;
	dateTime.tm_gmtoff = -timezone;

	dest = mktime(&dateTime);
	return true;
}

bool afc::parseISODateTime(const char * const str, afc::DateTime &dest)
{
	tm dateTime;

	if (!parseDateTime(str, dateTime)) {
		return false;
	}

	dest = dateTime;

	return true;
}

bool afc::parseISODateTime(const char * const str, TimestampTZ &dest)
{
	tm dateTime;

	if (unlikely(!parseDateTime(str, dateTime))) {
		return false;
	}

	/* 1. Flush time zone offset into seconds to make a UTC time.
	 * 2. Make a local value out of the tm structure with the UTC time. It is shifted towards future by
	 *    amount of negated ::timezone.
	 * 3. Make a time_t value out of the tm structure with the local time.
	 *
	 * Unfortunately, this code is not portable. It compiles in Debian Wheezy with GCC 4.7.
	 * In addition, this code cannot tolerate the system time zone changed in the middle of processing.
	 */
	const int gmtOffset = dateTime.tm_gmtoff;
	dateTime.tm_sec -= gmtOffset;
	dateTime.tm_sec -= timezone;
	dateTime.tm_gmtoff = -timezone;

	dest.setMillis(static_cast<Timestamp::time_type>(mktime(&dateTime)) * 1000);
	dest.setGmtOffset(gmtOffset);
	return true;
}
