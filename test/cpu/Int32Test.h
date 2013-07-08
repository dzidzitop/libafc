#include <cpptest.h>

namespace afc
{
	struct Int32Test : public Test::Suite
	{
		Int32Test();

		void testInt32Conversions_PlatformEndianness();
		void testInt32Conversions_LittleEndian();
		void testInt32Conversions_BigEndian();
		void testConversionToBytes_PlatformEndianness();
		void testConversionToBytes_LittleEndian();
		void testConversionToBytes_BigEndian();
		void testInt32LE();
		void testInt32BE();
	};
}
