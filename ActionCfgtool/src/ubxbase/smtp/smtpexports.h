#pragma once
#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(SMTP_LIB)
#  define SMTP_EXPORT Q_DECL_EXPORT
# else
#  define SMTP_EXPORT Q_DECL_IMPORT
# endif
#else
# define SMTP_EXPORT
#endif