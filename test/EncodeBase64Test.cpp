#include "EncodeBase64Test.hpp"
#include <afc/base64.hpp>

using namespace std;

afc::EncodeBase64Test::EncodeBase64Test()
{
	TEST_ADD(EncodeBase64Test::testString_EncodeEmptyString);
	TEST_ADD(EncodeBase64Test::testString_EncodeOctet);
	TEST_ADD(EncodeBase64Test::testString_EncodeTwoOctets);
	TEST_ADD(EncodeBase64Test::testString_EncodeTriplet);
	TEST_ADD(EncodeBase64Test::testString_EncodeTripletAndOctet);
	TEST_ADD(EncodeBase64Test::testString_EncodeTripletAndTwoOctets);
	TEST_ADD(EncodeBase64Test::testString_EncodeTwoTriplets);
	TEST_ADD(EncodeBase64Test::testString_EncodeTwoTripletsAndOctet);
	TEST_ADD(EncodeBase64Test::testString_EncodeTwoTripletsAndTwoOctets);
}

void afc::EncodeBase64Test::testString_EncodeEmptyString()
{
	TEST_ASSERT(encodeBase64(string()) == "");
}

void afc::EncodeBase64Test::testString_EncodeOctet()
{
	TEST_ASSERT(encodeBase64(string("M")) == "TQ==");
}

void afc::EncodeBase64Test::testString_EncodeTwoOctets()
{
	TEST_ASSERT(encodeBase64(string("Ma")) == "TWE=");
}

void afc::EncodeBase64Test::testString_EncodeTriplet()
{
	TEST_ASSERT(encodeBase64(string("Man")) == "TWFu");
}

void afc::EncodeBase64Test::testString_EncodeTripletAndOctet()
{
	TEST_ASSERT(encodeBase64(string("ManM")) == "TWFuTQ==");
}

void afc::EncodeBase64Test::testString_EncodeTripletAndTwoOctets()
{
	TEST_ASSERT(encodeBase64(string("ManMa")) == "TWFuTWE=");
}

void afc::EncodeBase64Test::testString_EncodeTwoTriplets()
{
	TEST_ASSERT(encodeBase64(string("Triple")) == "VHJpcGxl");
}

void afc::EncodeBase64Test::testString_EncodeTwoTripletsAndOctet()
{
	TEST_ASSERT(encodeBase64(string("TripleM")) == "VHJpcGxlTQ==");
}

void afc::EncodeBase64Test::testString_EncodeTwoTripletsAndTwoOctets()
{
	TEST_ASSERT(encodeBase64(string("TripleMa")) == "VHJpcGxlTWE=");
}
