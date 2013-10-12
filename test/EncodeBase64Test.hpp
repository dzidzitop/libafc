#ifndef AFC_ENCODEBASE64TEST_H_
#define AFC_ENCODEBASE64TEST_H_

#include <cpptest.h>

namespace afc
{
	// TODO Add tests that test all possible values of an octet.
	struct EncodeBase64Test : public Test::Suite
	{
		EncodeBase64Test();

		void testString_EncodeEmptyString();
		void testString_EncodeOctet();
		void testString_EncodeTwoOctets();
		void testString_EncodeTriplet();
		void testString_EncodeTripletAndOctet();
		void testString_EncodeTripletAndTwoOctets();
		void testString_EncodeTwoTriplets();
		void testString_EncodeTwoTripletsAndOctet();
		void testString_EncodeTwoTripletsAndTwoOctets();
	};
}

#endif /* AFC_ENCODEBASE64TEST_H_ */
