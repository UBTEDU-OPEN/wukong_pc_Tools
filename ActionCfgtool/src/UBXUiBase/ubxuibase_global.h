#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(UBXUIBASE_LIB)
#  define UBXUIBASE_EXPORT Q_DECL_EXPORT
# else
#  define UBXUIBASE_EXPORT Q_DECL_IMPORT
# endif
#else
# define UBXUIBASE_EXPORT
#endif
