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
#ifndef AFC_DATEUTILTEST_HPP_
#define AFC_DATEUTILTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <memory>
#include <string>

namespace afc
{
	class DateUtilTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(DateUtilTest);
		CPPUNIT_TEST(testParseValidISODateTime_PositiveUTCTimeZone);
		CPPUNIT_TEST(testParseValidISODateTime_NegativeUTCTimeZone);
		CPPUNIT_TEST(testParseValidISODateTime_PositiveNonUTCTimeZone);
		CPPUNIT_TEST(testParseValidISODateTime_NegativeNonUTCTimeZone);

		CPPUNIT_TEST(testParseValidISODateTime_DateTime_PositiveUTCTimeZone);
		CPPUNIT_TEST(testParseValidISODateTime_DateTime_NegativeUTCTimeZone);
		CPPUNIT_TEST(testParseValidISODateTime_DateTime_PositiveNonUTCTimeZone);
		CPPUNIT_TEST(testParseValidISODateTime_DateTime_NegativeNonUTCTimeZone);
		CPPUNIT_TEST_SUITE_END();

		std::unique_ptr<std::string> m_timeZoneBackup;
	public:
		void setUp() override;
		void tearDown() override;

		void testParseValidISODateTime_PositiveUTCTimeZone();
		void testParseValidISODateTime_NegativeUTCTimeZone();
		void testParseValidISODateTime_PositiveNonUTCTimeZone();
		void testParseValidISODateTime_NegativeNonUTCTimeZone();

		void testParseValidISODateTime_DateTime_PositiveUTCTimeZone();
		void testParseValidISODateTime_DateTime_NegativeUTCTimeZone();
		void testParseValidISODateTime_DateTime_PositiveNonUTCTimeZone();
		void testParseValidISODateTime_DateTime_NegativeNonUTCTimeZone();
	};
}

#endif /* AFC_DATEUTILTEST_HPP_ */
