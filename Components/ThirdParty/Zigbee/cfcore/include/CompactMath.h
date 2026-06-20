// CompactMath.h : header file
//
// Copyright(C) 2023 ubisys technologies GmbH, Duesseldorf, Germany.
// All rights reserved.
//
// www.ubisys.de
// support@ubisys.de
//
// ubisys(R) generic Compact Series framework
//
// This module provides integer arithmetic functions that complement the
// standard C library with implementations that are either smaller or faster

/////////////////////////////////////////////////////////////////////////////
// Integer division with rounding

// Notice: Compilers might generate warnings when T or T2 are unsigned 
// (IAR C++ 7.80.4 does, for example). However, these can be safely ignored,
// as these checks are required for signed types

#ifdef __IAR_SYSTEMS_ICC__
#pragma diag_suppress=Pe186
#endif

template <typename T, typename T2 = T>
inline T __c_div(const T value, const T2 divisor)
{
	const T half = divisor >> 1;

	// If value and divisor have the same sign then add else subtract before
	// dividing
	
	return (value < 0) != (divisor < 0) ?
		(value - half) / divisor :
		(value + half) / divisor;
}


template <typename T, typename T2>
inline T2 __c_muldiv(const T value, const T numerator, const T denominator)
{
	const T2 product = static_cast<T2>(value) * numerator;
	const T half = denominator >> 1;

	// If product and divisor have the same sign then add else subtract before
	// dividing
	
	return (product < 0) != (denominator < 0) ?
		(product - half) / denominator :
		(product + half) / denominator;
}


template <typename T, typename T2 = T>
inline T __c_div2(const T value, const uint_fast8_t shift)
{
	const T half = (1 << shift) - 1;

	// If value is positive then add else subtract before shifting (dividing)
	
	return (value < 0) ?
		(value - half) >> shift :
		(value + half) >> shift;
}

#ifdef __IAR_SYSTEMS_ICC__
#pragma diag_default=Pe186
#endif


/////////////////////////////////////////////////////////////////////////////
// Square root of 32-bit and 64-bit integers

uint_fast16_t __c_sqrt32(const uint_fast32_t value);
uint_fast32_t __c_sqrt64(const uint_fast64_t value);
