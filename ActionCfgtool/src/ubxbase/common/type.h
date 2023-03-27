#ifndef __TYPE_H
#define __TYPE_H

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
/// types, modify from proto buffer common.h
//////////////////////////////////////////////////////////////////////////
typedef unsigned int uint;
#ifdef __MACH__
typedef int BOOL;
#endif
typedef unsigned long long  ULONGLONG;
typedef unsigned long       DWORD;
typedef void*           PVOID;
typedef PVOID          LPVOID;
typedef LPVOID         HANDLE;
typedef unsigned int    UINT;
typedef unsigned char byte;

#ifdef _WIN32
#include <windef.h>
#else
typedef void*         HWND;
#endif

#ifndef _WIN32
//typedef ULONGLONG   LRESULT;
typedef unsigned short   WCHAR;
#endif

#ifndef _MAC
//typedef wchar_t WCHAR;    // wc,   16-bit UNICODE character
#else
// some Macintosh compilers don't define wchar_t in a convenient location, or define it as a char
typedef unsigned short WCHAR;    // wc,   16-bit UNICODE character
#endif

typedef unsigned char byte;

#ifdef _MSC_VER
typedef __int8  r_int8;
typedef __int16 r_int16;
typedef __int32 r_int32;
typedef __int64 r_int64;

typedef unsigned __int8  r_uint8;
typedef unsigned __int16 r_uint16;
typedef unsigned __int32 r_uint32;
typedef unsigned __int64 r_uint64;
#else
#include <stdint.h>
typedef int8_t  r_int8;
typedef int16_t r_int16;
typedef int32_t r_int32;
typedef long long int r_int64;

typedef uint8_t  r_uint8;
typedef uint16_t r_uint16;
typedef uint32_t r_uint32;
typedef unsigned long long int r_uint64;
#endif

typedef r_uint8 BYTE, *PBYTE;  /* 8 bits */

#define TRUE  1
#define FALSE 0
#define NULL  0

// long long macros to be used because gcc and vc++ use different suffixes,
// and different size specifiers in format strings
#undef TT_LONGLONG
#undef TT_ULONGLONG
#undef TT_LL_FORMAT

#ifdef _MSC_VER
#define TT_LONGLONG(x) x##I64
#define TT_ULONGLONG(x) x##UI64
#define TT_LL_FORMAT "I64"  // As in //printf("%I64d", ...)
#else
#define TT_LONGLONG(x) x##LL
#define TT_ULONGLONG(x) x##ULL
#define TT_LL_FORMAT "ll"  // As in "%lld". Note that "q" is poor form also.
#endif

#define INT32_MAX 0x7FFFFFFF
#define INT32_MIN (-INT32_MAX - 1)
#define INT64_MAX ( TT_LONGLONG(0x7FFFFFFFFFFFFFFF) )
#define INT64_MIN (-INT64_MAX - 1)
#define UINT32_MAX 0xFFFFFFFFu
#define UNIT64_MAX TT_ULONGLONG(0xFFFFFFFFFFFFFFFF);

#ifdef __cplusplus
}
#endif
#endif

