//#include "stdafx.h"
#include "systemutils.h"
#include <QProcess>
#include <QMutex>
#include <QTime>
#include <QWaitCondition>
#include <QStringList>
#ifdef _WIN32
#include <windows.h>
#include <TlHelp32.h>
#include <process.h>
#endif
#include <QCoreApplication>
#include <QProcess>

//系统公共函数

QStringList SystemUtils::getAllRunningProcess()
{
	QProcess process;
	process.setReadChannel(QProcess::StandardOutput);
	process.setReadChannelMode(QProcess::MergedChannels);

#ifdef _WIN32
	process.start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption");
#endif

	process.waitForStarted(1000);
	process.waitForFinished(1000);

	QByteArray list = process.readAll();
	QList<QByteArray> _tt = list.split('\n');
	QStringList proList;
	for(int i = 0 ; i< _tt.length(); i++)
	{
		QString tmp = QString::fromLocal8Bit( _tt.at(i).simplified());
		if(tmp.isEmpty()) continue;
		proList<<tmp;
	}
	return proList;
}

/**************************************************************************
* 函数名:   startProcess
* 功能:
* 参数:
*....@[in]  const QString & fullPath
*....@[out] bool
* 返回值:
* 时间:     2017/11/13 17:59
* 作者:   Leo
*/
bool SystemUtils::startProcess(const QString &fullPath)
{
    if (!fullPath.isEmpty()) {
        return false;
    }
    QProcess process;
    process.start(fullPath);
    return true;
}

bool SystemUtils::isProcessIsRunning(const QString& processName)
{
#ifdef _WIN32
	PROCESSENTRY32 pE;
	pE.dwSize = sizeof(pE);
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapshot == INVALID_HANDLE_VALUE) 
	{
		return false;
	}

	bool bNext = Process32First(hSnapshot, &pE);
	while(bNext)
	{
		QString name = QString::fromWCharArray(pE.szExeFile);
		if(!name.compare(processName, Qt::CaseInsensitive)) {
			CloseHandle(hSnapshot);
			return true;
		}
		bNext = Process32Next(hSnapshot, &pE);
	}
	CloseHandle(hSnapshot);
#endif

	return false;
}

bool SystemUtils::killProcess(const QString& processName)
{
	bool bRet = true;

#ifdef _WIN32
	PROCESSENTRY32 pE;
	pE.dwSize = sizeof(pE);
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapshot == INVALID_HANDLE_VALUE) 
	{
		return false;
	}

	bool bNext = Process32First(hSnapshot, &pE);
	while(bNext)
	{
		QString name = QString::fromWCharArray(pE.szExeFile);
		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, pE.th32ProcessID);
		if(!name.compare(processName, Qt::CaseInsensitive) &&
			pE.th32ProcessID != ::GetCurrentProcessId() &&
			hProcess) {
			bool ret =  ::TerminateProcess(hProcess, 0);
			if(bRet){ bRet = ret; }
			if(ret) CloseHandle(hProcess);
		}
		bNext = Process32Next(hSnapshot, &pE);
	}
	CloseHandle(hSnapshot);
#endif

	return bRet;
}

/**************************************************************************
* 函数名:   KillProcess
* 功能:
* 参数:
*....@[in]  const int & id
*....@[out] bool
* 返回值:
* 时间:     2017/11/08 16:32
* 作者:   Leo
*/
int SystemUtils::killProcess(const int &id)
{
    QProcess tasklist;
    int ret = tasklist.execute("taskkill", QStringList() << "/pid" << QString::number(id, 10) << "/f");
    //确认
    return ret;
}

void SystemUtils::sleep(unsigned int msSecond)
{
	QTime t; 
	t.start(); 
	while(t.elapsed() < msSecond) 
	{ 
		QCoreApplication::processEvents(); 
	} 
}

/**************************************************************************
* 函数名:   getPid
* 功能:
* 参数:
*....@[out] int
* 返回值:
* 时间:     2017/11/08 17:48
* 作者:   Leo
*/
int SystemUtils::getPid()
{
    int pid = 0;
#ifdef _WIN32
    pid = getpid();
#else
#endif
    return pid;
}

