/* libafc - utils to facilitate C++ development.
Copyright (C) 2015 Dźmitry Laŭčuk

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
#ifndef AFC_JSONOBJECTPARSERTEST_HPP_
#define AFC_JSONOBJECTPARSERTEST_HPP_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace afc
{
	class JSONObjectParserTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(JSONObjectParserTest);
		CPPUNIT_TEST(testEmptyObject);
		CPPUNIT_TEST(testEmptyObjectWithOuterSpaces);
		CPPUNIT_TEST(testEmptyObjectWithInnerSpaces);
		CPPUNIT_TEST(testObjectWithStringProperty);
		CPPUNIT_TEST(testObjectWithIntProperty);
		CPPUNIT_TEST(testObjectWithBooleanProperty_True);
		CPPUNIT_TEST(testObjectWithBooleanProperty_False);
		CPPUNIT_TEST_SUITE_END();
	public:
		void testEmptyObject();
		void testEmptyObjectWithOuterSpaces();
		void testEmptyObjectWithInnerSpaces();
		void testObjectWithStringProperty();
		void testObjectWithIntProperty();
		void testObjectWithBooleanProperty_True();
		void testObjectWithBooleanProperty_False();
	};
}

#endif /* AFC_JSONOBJECTPARSERTEST_HPP_ */
