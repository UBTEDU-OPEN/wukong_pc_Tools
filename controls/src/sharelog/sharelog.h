#ifndef SHARELOG_H
#define SHARELOG_H

#include <QtCore/qglobal.h>
#ifdef SHARELOG_EXPORTS
#define SHARELOG_EXPORT Q_DECL_EXPORT
#else
#define SHARELOG_EXPORT Q_DECL_IMPORT
#endif

#define ELPP_STL_LOGGING
#include "easylogging++.h"
#undef ELPP_THREADING_ENABLED
#define ELPP_THREADING_ENABLED 1

el::base::type::StoragePointer SHARELOG_EXPORT getStorage();
void SHARELOG_EXPORT initShareLog();

#endif // SHARELOG_H
