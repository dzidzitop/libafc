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
#include "DateUtilTest.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(afc::DateUtilTest);

#include <afc/dateutil.hpp>
#include <time.h>

using namespace std;
using namespace afc;

void afc::DateUtilTest::setUp()
{
	m_timeZoneBackup = getenv("TZ");
	// This time zone is set to ensure that conversion is performed via UTC.
	setenv("TZ", "ABC-12:30", true);
}

void afc::DateUtilTest::tearDown()
{
	if (m_timeZoneBackup == nullptr) {
		setenv("TZ", m_timeZoneBackup, true);
	} else {
		unsetenv("TZ");
	}
}

void afc::DateUtilTest::testParseValidISODateTime_PositiveUTCTimeZone()
{
	string input("2013-10-16T20:02:26+0000");
	time_t dest;

	const bool result = parseISODateTime(input, dest);

	CPPUNIT_ASSERT(result);

	tm dateTime;
	gmtime_r(&dest, &dateTime);

	char buf[100];
	const size_t count = std::strftime(buf, 100, "%Y-%m-%dT%H:%M:%S%z", &dateTime);

	CPPUNIT_ASSERT(count != 0);
	CPPUNIT_ASSERT_EQUAL(string("2013-10-16T20:02:26+0000"), string(buf));
}

void afc::DateUtilTest::testParseValidISODateTime_NegativeUTCTimeZone()
{
	string input("2013-10-16T20:02:26-0000");
	time_t dest;

	const bool result = parseISODateTime(input, dest);

	CPPUNIT_ASSERT(result);

	tm dateTime;
	gmtime_r(&dest, &dateTime);

	char buf[100];
	const size_t count = std::strftime(buf, 100, "%Y-%m-%dT%H:%M:%S%z", &dateTime);

	CPPUNIT_ASSERT(count != 0);
	CPPUNIT_ASSERT_EQUAL(string("2013-10-16T20:02:26+0000"), string(buf));
}

void afc::DateUtilTest::testParseValidISODateTime_PositiveNonUTCTimeZone()
{
	string input("2013-10-16T20:02:26+0300");
	time_t dest;

	const bool result = parseISODateTime(input, dest);

	CPPUNIT_ASSERT(result);

	tm dateTime;
	gmtime_r(&dest, &dateTime);

	char buf[100];
	const size_t count = std::strftime(buf, 100, "%Y-%m-%dT%H:%M:%S%z", &dateTime);

	CPPUNIT_ASSERT(count != 0);
	CPPUNIT_ASSERT_EQUAL(string("2013-10-16T17:02:26+0000"), string(buf));
}

void afc::DateUtilTest::testParseValidISODateTime_NegativeNonUTCTimeZone()
{
	string input("2013-10-16T20:02:26-0130");
	time_t dest;

	const bool result = parseISODateTime(input, dest);

	CPPUNIT_ASSERT(result);

	tm dateTime;
	gmtime_r(&dest, &dateTime);

	char buf[100];
	const size_t count = std::strftime(buf, 100, "%Y-%m-%dT%H:%M:%S%z", &dateTime);

	CPPUNIT_ASSERT(count != 0);
	CPPUNIT_ASSERT_EQUAL(string("2013-10-16T21:32:26+0000"), string(buf));
}

void DateUtilTest::testParseValidISODateTime_DateTime_PositiveUTCTimeZone()
{
	string input("2013-10-16T20:02:26+0000");
	DateTime dest;

	const bool result = parseISODateTime(input, dest);

	CPPUNIT_ASSERT(result);

	tm dateTime = static_cast<tm>(dest);

	char buf[100];
	const size_t count = std::strftime(buf, 100, "%Y-%m-%dT%H:%M:%S%z", &dateTime);

	CPPUNIT_ASSERT(count != 0);
	CPPUNIT_ASSERT_EQUAL(string("2013-10-16T20:02:26+0000"), string(buf));
}

void DateUtilTest::testParseValidISODateTime_DateTime_NegativeUTCTimeZone()
{
	string input("2013-10-16T20:02:26-0000");
	DateTime dest;

	const bool result = parseISODateTime(input, dest);

	CPPUNIT_ASSERT(result);

	tm dateTime = static_cast<tm>(dest);

	char buf[100];
	const size_t count = std::strftime(buf, 100, "%Y-%m-%dT%H:%M:%S%z", &dateTime);

	CPPUNIT_ASSERT(count != 0);
	CPPUNIT_ASSERT_EQUAL(string("2013-10-16T20:02:26+0000"), string(buf));
}

void DateUtilTest::testParseValidISODateTime_DateTime_PositiveNonUTCTimeZone()
{
	string input("2013-10-16T20:02:26+0300");
	DateTime dest;

	const bool result = parseISODateTime(input, dest);

	CPPUNIT_ASSERT(result);

	tm dateTime = static_cast<tm>(dest);

	char buf[100];
	const size_t count = std::strftime(buf, 100, "%Y-%m-%dT%H:%M:%S%z", &dateTime);

	CPPUNIT_ASSERT(count != 0);
	CPPUNIT_ASSERT_EQUAL(string("2013-10-16T20:02:26+0300"), string(buf));
}

void DateUtilTest::testParseValidISODateTime_DateTime_NegativeNonUTCTimeZone()
{
	string input("2013-10-16T20:02:26-0130");
	DateTime dest;

	const bool result = parseISODateTime(input, dest);

	CPPUNIT_ASSERT(result);

	tm dateTime = static_cast<tm>(dest);

	char buf[100];
	const size_t count = std::strftime(buf, 100, "%Y-%m-%dT%H:%M:%S%z", &dateTime);

	CPPUNIT_ASSERT(count != 0);
	CPPUNIT_ASSERT_EQUAL(string("2013-10-16T20:02:26-0130"), string(buf));
}
