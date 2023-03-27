#ifndef _GLOBAL_ACTIONDATA_DEFS_
#define _GLOBAL_ACTIONDATA_DEFS_

#ifdef ACTIONDATA_LIB_STATIC
#define UBXACTIONDATA_EXPORTS
#else
#ifdef UBX_ACTIONDATA_LIB
#define UBXACTIONDATA_EXPORTS __declspec(dllexport)
#else
#define UBXACTIONDATA_EXPORTS __declspec(dllimport)
#endif
#endif

#include "type.h"
#endif

