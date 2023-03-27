#ifndef USBCOMM_GLOBAL_H
#define USBCOMM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(USBCOMM_LIBRARY)
#  define USBCOMMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define USBCOMMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // USBCOMM_GLOBAL_H
