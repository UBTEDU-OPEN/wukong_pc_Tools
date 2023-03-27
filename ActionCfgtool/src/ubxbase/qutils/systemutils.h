#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include "qutils_global.h"
class QString;
class QStringList;
//系统公共函数
class QUTILS_EXPORT SystemUtils
{
public:
    static bool startProcess(const QString &fullPath);
    static bool isProcessIsRunning(const QString &processName);
    static QStringList getAllRunningProcess();
    static bool killProcess(const QString &processName);
    static int killProcess(const int &id);
    static void sleep(unsigned int msSecond);
    static int getPid();
    //static DWORD getProcessFromName(QString name)
};

#endif