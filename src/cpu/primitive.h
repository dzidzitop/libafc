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
		template<endianness src> inline static D fromBytes(const unsigned char in[]);
	protected:
		Int32Base(const T i, const endianness interpretation) {set(i, interpretation);}
		template<endianness bo> Int32Base(const Int32Base<T, D, bo> &i) {set(i.data.dword, bo);}

		static_assert(sizeof(T) == 4, "T is not a dword");
		union data
		{
			T dword;
			char bytes[4];
		};
	private:
		data m_data;

		inline void set(const T i, const endianness interpretation);
		template<endianness dest> inline void setBytes(unsigned char out[]) const;
	};

	template<endianness o = PLATFORM_BYTE_ORDER> struct Int32 : public Int32Base<int32_t, Int32<o>, o>
	{
		Int32(const int32_t i, const endianness interpretation = PLATFORM_BYTE_ORDER) : Int32Base<int32_t, Int32<o>, o>(i, interpretation) {}
		template<endianness bo> Int32(const Int32<bo> &i) : Int32Base<int32_t, Int32<o>, o>(i) {}
		template<endianness src> Int32(const unsigned char in[]) : Int32Base<int32_t, Int32<o>, o>(in) {}

		template<endianness dest> operator Int32<dest>() const;
	};

	template<endianness o = PLATFORM_BYTE_ORDER> struct UInt32 : public Int32Base<uint32_t, UInt32<o>, o>
	{
		UInt32(const uint32_t i, const endianness interpretation = PLATFORM_BYTE_ORDER) : Int32Base<uint32_t, UInt32<o>, o>(i, interpretation) {}
		template<endianness bo> UInt32(const UInt32<bo> &i) : Int32Base<uint32_t, UInt32<o>, o>(i) {}
		template<endianness src> UInt32(const unsigned char in[]) : Int32Base<uint32_t, UInt32<o>, o>(in) {};

		template<endianness dest> operator UInt32<dest>() const;
	};
}

template <typename T, typename D, afc::endianness o>
inline void afc::Int32Base<T, D, o>::set(const T i, const afc::endianness interpretation)
{
	if (interpretation == o) {
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

template <typename T, typename D, afc::endianness o> template <afc::endianness src>
inline D afc::Int32Base<T, D, o>::fromBytes(const unsigned char in[])
{
	const uint32_t val = (src==endianness::BE)^(o==endianness::BE)^(PLATFORM_BYTE_ORDER==endianness::BE) ?
			(((((in[0]<<8) + in[1])<<8) + in[2])<<8) + in[3] :
			(((((in[3]<<8) + in[2])<<8) + in[1])<<8) + in[0];
	return D(val, o);
}

#endif /*AFC_CPU_PRIMITIVE_H_*/
