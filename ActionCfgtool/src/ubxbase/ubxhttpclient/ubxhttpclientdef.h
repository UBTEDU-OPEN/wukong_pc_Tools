#ifndef _UBX_HTTP_CLIENT_DEF_
#define _UBX_HTTP_CLIENT_DEF_ 

#ifdef BUILD_STATIC
#define UBXHTTPCLIENT_EXPORTS
#else
#ifdef UBXHTTPCLIENT_LIB
#define UBXHTTPCLIENT_EXPORTS __declspec(dllexport)
#else
#define UBXHTTPCLIENT_EXPORTS __declspec(dllimport)
#endif
#endif

//#define BUILDING_CURLPP

#endif
