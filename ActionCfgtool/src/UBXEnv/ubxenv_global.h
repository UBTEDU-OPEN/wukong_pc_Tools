#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(UBXENV_LIB)
#  define UBXENV_EXPORT Q_DECL_EXPORT
# else
#  define UBXENV_EXPORT Q_DECL_IMPORT
# endif
#else
# define UBXENV_EXPORT
#endif
