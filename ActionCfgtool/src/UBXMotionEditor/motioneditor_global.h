#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(MOTIONEDITOR_LIB)
#  define MOTIONEDITOR_EXPORT Q_DECL_EXPORT
# else
#  define MOTIONEDITOR_EXPORT Q_DECL_IMPORT
# endif
#else
# define MOTIONEDITOR_EXPORT
#endif
