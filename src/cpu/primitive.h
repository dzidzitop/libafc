#ifndef AFC_CPU_PRIMITIVE_H_
#define AFC_CPU_PRIMITIVE_H_

#include "../platform.h"
#include <cstdint>

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

	// TODO remove this class and move everything to templates.
	template<typename T, endianness o> class Int32Base
	{
	public:
		Int32Base(const T i, const endianness byteOrder = PLATFORM_BYTE_ORDER);
		Int32Base(const unsigned char in[], const endianness byteOrder = PLATFORM_BYTE_ORDER);
		template<endianness bo> Int32Base(const Int32Base<T, bo> &i) : Int32Base(i.data.dword, bo) {}

		/**
		 * Preserves the order of bytes.
		 */
		operator T() const {return static_cast<T>(m_data.dword);}

		template<endianness dest> inline void toBytes(unsigned char out[]) const;
		template<endianness src> inline static Int32Base fromBytes(const unsigned char in[]) {return Int32Base(in, src);}
	private:
		static_assert(sizeof(T) == 4, "T is not a dword");
		union data
		{
			T dword;
			char bytes[4];
		};

		data m_data;
	};

	template<endianness o = PLATFORM_BYTE_ORDER> using Int32 = Int32Base<std::int32_t, o>;
	template<endianness o = PLATFORM_BYTE_ORDER> using UInt32 = Int32Base<std::uint32_t, o>;
}

template <typename T, afc::endianness o>
inline afc::Int32Base<T, o>::Int32Base(const unsigned char in[], const afc::endianness byteOrder)
{
	if (byteOrder == o) {
		m_data.bytes[0] = in[0];
		m_data.bytes[1] = in[1];
		m_data.bytes[2] = in[2];
		m_data.bytes[3] = in[3];
	} else {
		m_data.bytes[0] = in[3];
		m_data.bytes[1] = in[2];
		m_data.bytes[2] = in[1];
		m_data.bytes[3] = in[0];
	}
}

template <typename T, afc::endianness o>
inline afc::Int32Base<T, o>::Int32Base(const T i, const endianness byteOrder)
{
	if (byteOrder == o) {
		m_data.dword = i;
	} else {
		data input = {i};
		m_data.bytes[0] = input.bytes[3];
		m_data.bytes[1] = input.bytes[2];
		m_data.bytes[2] = input.bytes[1];
		m_data.bytes[3] = input.bytes[0];
	}
}

template <typename T, afc::endianness src> template <afc::endianness dest>
inline void afc::Int32Base<T, src>::toBytes(unsigned char out[]) const
{
	if (dest == src) {
		out[0] = m_data.bytes[0];
		out[1] = m_data.bytes[1];
		out[2] = m_data.bytes[2];
		out[3] = m_data.bytes[3];
	} else {
		out[0] = m_data.bytes[3];
		out[1] = m_data.bytes[2];
		out[2] = m_data.bytes[1];
		out[3] = m_data.bytes[0];
	}
}

#endif /*AFC_CPU_PRIMITIVE_H_*/
