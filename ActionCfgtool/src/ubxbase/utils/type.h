#ifndef TYPE_H
#define TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
/// types, modify from proto buffer common.h
//////////////////////////////////////////////////////////////////////////
typedef unsigned int uint;
//#ifdef __MACH__
//typedef unsigned int size_t;
//#endif
typedef wchar_t WCHAR;
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
#include <stddef.h>
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

