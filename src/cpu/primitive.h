#ifndef AFC_CPU_PRIMITIVE_H_
#define AFC_CPU_PRIMITIVE_H_

#include "../platform.h"
#include <stdint.h>

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
	template<typename T, typename D, endianness o> class Int32Base
	{
	public:
		/**
		 * Preserves the order of bytes.
		 */
		operator T() const {return static_cast<T>(m_data.dword);}

		template<endianness dest> inline void toBytes(unsigned char out[]) const;
		template<endianness src> inline static D fromBytes(const unsigned char in[]) {return D(in, src);}
	protected:
		Int32Base(const T i, const endianness byteOrder) {set(i, byteOrder);}
		Int32Base(const unsigned char in[], const endianness byteOrder = PLATFORM_BYTE_ORDER);
		template<endianness bo> Int32Base(const Int32Base<T, D, bo> &i) {set(i.data.dword, bo);}

		static_assert(sizeof(T) == 4, "T is not a dword");
		union data
		{
			T dword;
			char bytes[4];
		};
	private:
		data m_data;

		inline void set(const T i, const endianness byteOrder);
	};

	template<endianness o = PLATFORM_BYTE_ORDER> struct Int32 : public Int32Base<int32_t, Int32<o>, o>
	{
		Int32(const int32_t i, const endianness byteOrder = PLATFORM_BYTE_ORDER)
				: Int32Base<int32_t, Int32<o>, o>(i, byteOrder) {}
		Int32(const unsigned char in[], const endianness byteOrder = PLATFORM_BYTE_ORDER)
				: Int32Base<int32_t, Int32<o>, o>(in, byteOrder) {}
		template<endianness bo> Int32(const Int32<bo> &i) : Int32Base<int32_t, Int32<o>, o>(i) {}

		template<endianness dest> operator Int32<dest>() const;
	};

	template<endianness o = PLATFORM_BYTE_ORDER> struct UInt32 : public Int32Base<uint32_t, UInt32<o>, o>
	{
		UInt32(const uint32_t i, const endianness byteOrder = PLATFORM_BYTE_ORDER)
				: Int32Base<uint32_t, UInt32<o>, o>(i, byteOrder) {}
		UInt32(const unsigned char in[], const endianness byteOrder = PLATFORM_BYTE_ORDER)
				: Int32Base<int32_t, Int32<o>, o>(in, byteOrder) {}
		template<endianness bo> UInt32(const UInt32<bo> &i) : Int32Base<uint32_t, UInt32<o>, o>(i) {}

		template<endianness dest> operator UInt32<dest>() const;
	};
}

template <typename T, typename D, afc::endianness o>
inline afc::Int32Base<T, D, o>::Int32Base(const unsigned char in[], const afc::endianness byteOrder)
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

template <typename T, typename D, afc::endianness o>
inline void afc::Int32Base<T, D, o>::set(const T i, const afc::endianness byteOrder)
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

template <typename T, typename D, afc::endianness src> template <afc::endianness dest>
inline void afc::Int32Base<T, D, src>::toBytes(unsigned char out[]) const
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
