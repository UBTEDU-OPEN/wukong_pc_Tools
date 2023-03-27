#ifndef _GLOBAL_DEF_UBXSOCKET_
#define _GLOBAL_DEF_UBXSOCKET_

#ifdef UBXSOCKET_LIB_STATIC
#define UBXSOCKET_EXPORTS
#else
    #ifdef UBXSOCKET_LIB
        #define UBXSOCKET_EXPORTS __declspec(dllexport)
    #else
        #define UBXSOCKET_EXPORTS __declspec(dllimport)
    #endif
#endif

#endif
