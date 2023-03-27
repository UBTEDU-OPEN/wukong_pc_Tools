#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(QUTILS_LIB)
#  define QUTILS_EXPORT Q_DECL_EXPORT
# else
#  define QUTILS_EXPORT Q_DECL_IMPORT
# endif
#else
# define QUTILS_EXPORT
#endif
