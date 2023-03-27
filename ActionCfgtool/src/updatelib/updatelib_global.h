#ifndef UPDATELIB_GLOBAL_H
#define UPDATELIB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <urldef.h>

#if defined(UPDATELIB_LIBRARY)
#  define UPDATELIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define UPDATELIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // UPDATELIB_GLOBAL_H
