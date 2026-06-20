// CompactFramework.inl : inline file
//
// Copyright(c) 2005 - 2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) generic Compact series framework
//
// Provides generic inline functions, such as byte-order conversions, etc.


/////////////////////////////////////////////////////////////////////////////
// Increment and saturate

inline void __c_increment_saturate(uint8_t &value)
{
	if (value < UINT8_MAX)
		value++;
}

inline void __c_increment_saturate(uint16_t &value)
{
	if (value < UINT16_MAX)
		value++;
}

inline void __c_increment_saturate(uint32_t &value)
{
	if (value < UINT32_MAX)
		value++;
}

inline void __c_increment_saturate(uint64_t &value)
{
	if (value < UINT64_MAX)
		value++;
}


/////////////////////////////////////////////////////////////////////////////
// CPU endianness abstraction

inline uint16_t __compact_swap16(const uint16_t wValue)
{
	return (wValue >> 8) | ((wValue & 0x00ff) << 8);
}

inline uint32_t __compact_swap32(const uint32_t dwValue)
{
	return (dwValue >> 24) | ((dwValue & 0x00ff0000) >> 8) |
		((dwValue & 0x0000ff00) << 8) | ((dwValue & 0x000000ff) << 24);
}

inline uint64_t __compact_swap64(const uint64_t qwValue)
{
	const uint32_t dwHigher =
		__compact_swap32(static_cast<uint32_t>(qwValue >> 32));

	const uint32_t dwLower =
		__compact_swap32(static_cast<uint32_t>(qwValue));

	return (static_cast<uint64_t>(dwLower) << 32) | dwHigher;
}


/////////////////////////////////////////////////////////////////////////////
// big-endian vs. little-endian

#ifdef __LITTLE_ENDIAN__

inline uint16_t __cfcore_be16(const uint16_t x)
{
	return __compact_swap16(x);
}

inline uint32_t __cfcore_be32(const uint32_t x)
{
	return __compact_swap32(x);
}

inline uint64_t __cfcore_be64(const uint64_t x)
{
	return __compact_swap64(x);
}

#else

inline uint16_t __cfcore_be16(const uint16_t x)
{
	return x;
}

inline uint32_t __cfcore_be32(const uint32_t x)
{
	return x;
}

inline uint64_t __cfcore_be64(const uint64_t x)
{
	return x;
}

#endif


/////////////////////////////////////////////////////////////////////////////
// CPU unaligned read/write

#pragma pack(push, 1)

template<class T>
class __compact_unaligned_t
{
	// Attributes
	public:
		T m_value;

	// Operations
	public:
		// Operator used for unaligned read access
		operator T() const;

		// Operator used for unaligned write access
		__compact_unaligned_t &operator=(const T value);
};

#pragma pack(pop)

template<class T>
inline __compact_unaligned_t<T>::operator T() const
	{ return m_value; }

template<class T>
inline __compact_unaligned_t<T> &__compact_unaligned_t<T>::operator=(const T value)
	{ m_value = value; return *this; }

#define __c_unaligned(T, a) \
	(*reinterpret_cast<__compact_unaligned_t<T> *>(a))

#define __c_unaligned_const(T, a) \
	static_cast<T>(*reinterpret_cast<const __compact_unaligned_t<T> *>(a))


/////////////////////////////////////////////////////////////////////////////
// Enforcing a stricter (larger) alignment on data elements

#if __cplusplus >= 201103L
#define __COMPACT_ALIGN(alignment) alignas(alignment)
#else
#ifdef __GNUC__
#define __COMPACT_ALIGN(alignment) __attribute__ ((aligned(alignment)))
#elif defined(_MSC_VER)
#define __COMPACT_ALIGN(alignment) __declspec(align(alignment))
#elif defined(__IAR_SYSTEMS_ICC__)
#define __COMPACT_ALIGN(alignment) \
	_Pragma("language=extended") \
	_Pragma(__COMPACT_LITERALIZE(data_alignment=alignment)) \
	_Pragma("language=default")
#else
#error Unknown compiler, no support for __COMPACT_ALIGN()
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// Enforcing a stricter (larger) alignment on data elements

#if __cplusplus >= 201103L
#define __COMPACT_ALIGNOF(typeOrExpression) alignof(typeOrExpression)
#else
#ifdef __GNUC__
#define __COMPACT_ALIGNOF(typeOrExpression) __alignof__(typeOrExpression)
#elif defined(_MSC_VER)
#define __COMPACT_ALIGNOF(typeOrExpression) __alignof(typeOrExpression)
#elif defined(__IAR_SYSTEMS_ICC__)
#define __COMPACT_ALIGNOF(typeOrExpression) __ALIGNOF__(typeOrExpression)
#else
#error Unknown compiler, no support for __COMPACT_ALIGNOF()
#endif
#endif


/////////////////////////////////////////////////////////////////////////////
// array size (number of elements in array)

#define __c_countof(array) (sizeof(array)/sizeof(array[0]))


/////////////////////////////////////////////////////////////////////////////
// offset of class member

#ifdef __GNUC__
#define __c_offsetof(cls, member) offsetof(cls, member)
#else
#define __c_offsetof(cls, member) reinterpret_cast<uintptr_t> \
        (&static_cast<const cls *>(0)->member)
#endif
