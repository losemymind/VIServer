/****************************************************************************
 Copyright (c) 2015 libo All rights reserved.
 
 losemymind.libo@gmail.com
 
 ****************************************************************************/

#ifndef LOSEMYMIND_TYPES_H
#define LOSEMYMIND_TYPES_H
#pragma once

#include <string>
#include "FoundationKit/GenericPlatformMacros.h"

NS_FK_BEGIN

//---------------------------------------------------------------------
// Utility for automatically setting up the pointer-sized integer type
// http://zh.cppreference.com/w/cpp/language/types
//---------------------------------------------------------------------

template<typename T32BITS, typename T64BITS, int PointerSize>
struct SelectIntPointerType
{
    // nothing here are is it an error if the partial specializations fail
};

template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 8>
{
    typedef T64BITS type; // select the 64 bit type
};

template<typename T32BITS, typename T64BITS>
struct SelectIntPointerType<T32BITS, T64BITS, 4>
{
    typedef T32BITS type; // select the 32 bit type
};


// Unsigned base types.
typedef unsigned char 		uint8;		// 8-bit  unsigned.
typedef unsigned short int	uint16;		// 16-bit unsigned.
typedef unsigned int		uint32;		// 32-bit unsigned.
typedef unsigned long long	uint64;		// 64-bit unsigned.

// Signed base types.
typedef	signed char			int8;		// 8-bit  signed.
typedef signed short int	int16;		// 16-bit signed.
typedef signed int	 		int32;		// 32-bit signed.
typedef signed long long	int64;		// 64-bit signed.

typedef float		        float32;    // 32-bit signed.
typedef double		        float64;    // 64-bit signed.
typedef long double         float128;   // 128-bit signed.

// Character types.
typedef char				ANSICHAR;	// An ANSI character       -                  8-bit fixed-width representation of 7-bit characters.
typedef wchar_t				WIDECHAR;	// A wide character        - In-memory only.  ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms.
typedef uint8				CHAR8;		// An 8-bit character type - In-memory only.  8-bit representation.  Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef uint16				CHAR16;		// A 16-bit character type - In-memory only.  16-bit representation.  Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef uint32				CHAR32;		// A 32-bit character type - In-memory only.  32-bit representation.  Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef WIDECHAR			TCHAR;		// A switchable character  - In-memory only.  Either ANSICHAR or WIDECHAR, depending on a licensee's requirements.

typedef SelectIntPointerType<uint32, uint64, sizeof(void*)>::type UPTRINT;	// unsigned int the same size as a pointer
typedef SelectIntPointerType<int32, int64, sizeof(void*)>::type PTRINT;   // signed int the same size as a pointer

typedef UPTRINT address;

typedef std::basic_string<uint8, std::char_traits<uint8>, std::allocator<uint8> > ustring;

template<typename _Ty>
struct allocator_traits 
{
    typedef std::allocator<_Ty> type;
};

template<class _Ty>
using allocator_traits_t = typename allocator_traits<_Ty>::type;

NS_FK_END
#endif // LOSEMYMIND_TYPES_H


