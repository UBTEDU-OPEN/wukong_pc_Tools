/**
 *	utils总体控制
 */

#ifndef _BASE_UTILS_UTILSDEF_H
#define _BASE_UTILS_UTILSDEF_H

#ifdef UTILS_LIB_STATIC
#define UTILS_EXPORTS
#else
    #ifdef UTILS_LIB
#define UTILS_EXPORTS __declspec(dllexport)
    #else
#define UTILS_EXPORTS __declspec(dllimport)
    #endif
#endif

#endif
