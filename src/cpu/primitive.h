#ifndef AFC_CPU_PRIMITIVE_H_
#define AFC_CPU_PRIMITIVE_H_

#include "../platform.h"
#include <cstdint>
#include <climits>
#include <cstddef>

static_assert(CHAR_BIT == 8, "only 8-bit bytes (chars) are supported");

namespace afc
{
	enum class endianness {LE = 0, BE = 1};

	const endianness PLATFORM_BYTE_ORDER =
#ifdef AFC_LE
		endianness::LE;
#elif defined AFC_BE
		endianness::BE;
#else
	#error "unsupported platform"
#endif

	template<typename T, endianness o> class IntegerBase
	{
		static const size_t bytesCount = sizeof(T);
	public:
		IntegerBase(const T val, const endianness byteOrder = PLATFORM_BYTE_ORDER);
		IntegerBase(const unsigned char in[], const endianness byteOrder = PLATFORM_BYTE_ORDER) {init(in, byteOrder);}
		IntegerBase(const char in[], const endianness byteOrder = PLATFORM_BYTE_ORDER) {init(in, byteOrder);}
		template<endianness bo> IntegerBase(const IntegerBase<T, bo> &val) : IntegerBase(val.data.value, bo) {}

		/**
		 * Preserves the order of bytes.
		 */
		operator T() const {return m_data.value;}

		template<endianness dest> inline void toBytes(unsigned char out[]) const;
		template<endianness src> inline static IntegerBase fromBytes(const unsigned char in[]) {return IntegerBase(in, src);}
		template<endianness src> inline static IntegerBase fromBytes(const char in[]) {return IntegerBase(in, src);}

		typedef T type;
	private:
		template<typename CharType> inline void init(const CharType in[], const endianness byteOrder = PLATFORM_BYTE_ORDER);

		union data
		{
			T value;
			unsigned char bytes[bytesCount];
		};

		data m_data;
	};

	template<endianness o = PLATFORM_BYTE_ORDER> using Int16 = IntegerBase<std::int16_t, o>;
	template<endianness o = PLATFORM_BYTE_ORDER> using UInt16 = IntegerBase<std::uint16_t, o>;
	template<endianness o = PLATFORM_BYTE_ORDER> using Int32 = IntegerBase<std::int32_t, o>;
	template<endianness o = PLATFORM_BYTE_ORDER> using UInt32 = IntegerBase<std::uint32_t, o>;
	template<endianness o = PLATFORM_BYTE_ORDER> using Int64 = IntegerBase<std::int64_t, o>;
	template<endianness o = PLATFORM_BYTE_ORDER> using UInt64 = IntegerBase<std::uint64_t, o>;
}

template<typename T, afc::endianness o> template<typename CharType>
inline void afc::IntegerBase<T, o>::init(const CharType in[], const afc::endianness byteOrder)
{
	static_assert(sizeof(CharType) == 1, "only 8-bit bytes (chars) are supported");
	if (byteOrder == o) {
		for (size_t i = 0; i < bytesCount; ++i) {
			m_data.bytes[i] = in[i];
		}
	} else {
		for (size_t i = 0; i < bytesCount; ++i) {
			m_data.bytes[i] = in[bytesCount-1 - i];
		}
	}
}

template<typename T, afc::endianness o>
inline afc::IntegerBase<T, o>::IntegerBase(const T val, const endianness byteOrder)
{
	if (byteOrder == o) {
		m_data.value = val;
	} else {
		data input = {val};
		for (size_t i = 0; i < bytesCount; ++i) {
			m_data.bytes[i] = input.bytes[bytesCount-1 - i];
		}
	}
}

template<typename T, afc::endianness src> template <afc::endianness dest>
inline void afc::IntegerBase<T, src>::toBytes(unsigned char out[]) const
{
	if (dest == src) {
		for (size_t i = 0; i < bytesCount; ++i) {
			out[i] = m_data.bytes[i];
		}
	} else {
		for (size_t i = 0; i < bytesCount; ++i) {
			out[i] = m_data.bytes[bytesCount-1 - i];
		}
	}
}

#endif /*AFC_CPU_PRIMITIVE_H_*/
