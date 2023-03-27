#include "cthreadwaitforudiskshowup.h"
#include "ubxupdatemoduleconst.h"
#include "systemembeddedupdatewidget.h"
#include "cubxudiskmanager.h"

CThreadWaitForUdiskShowup::CThreadWaitForUdiskShowup(QObject *pParent) : QThread(pParent)
{

}

CThreadWaitForUdiskShowup::~CThreadWaitForUdiskShowup()
{

}

void CThreadWaitForUdiskShowup::run()
{
    int nTryTimes = UpdateModuleConstant::WAIT_FOR_UDISK_SHOWUP_TIMEOUT/UpdateModuleConstant::TIMEOUT_1000MS;
    int nCount = 0;
    bool bShowUp = false;
    while(true)
    {
        QString strDiskName = "";
        QString strBSDPath = "";

#ifdef Q_OS_WIN
        strDiskName = CUBXUdiskManager::GetDiskFlag();
#else
        if (!CUBXUdiskManager::getUBTUSBDeviceByIOLib(strDiskName, strBSDPath))
        {
//            AlphaRobotLog::getInstance()->printError("Failed to get UBT USB device");
//            return;
        }
#endif

        if (strDiskName.isEmpty() == false)
        {
            bShowUp = true;
            break;
        }
		
#ifdef Q_OS_MAC
        if( nTryTimes/2 == nCount)
        {
            if(CUBXUdiskManager::getUBTUSBDeviceByCommand(strDiskName, strBSDPath))
            {
                AlphaRobotLog::getInstance()->printError(QString("Sucess to get UBT disk info in command way: DiskPath: %1; BSDPath: %2").arg(strDiskName).arg(strBSDPath));
                bShowUp = true;
                break;
            }
            else
            {
                AlphaRobotLog::getInstance()->printError("Failed when try to get UBT disk info in command way ");
            }
        }
#endif

        nCount++;
        if (nCount >= nTryTimes)
        {
            AlphaRobotLog::getInstance()->printError("Get robot udisk in IO library way time out");

#ifdef Q_OS_MAC
        if(CUBXUdiskManager::getUBTUSBDeviceByCommand(strDiskName, strBSDPath))
        {
            AlphaRobotLog::getInstance()->printError(QString("Sucess to get UBT disk info in command way: DiskPath: %1; BSDPath: %2").arg(strDiskName).arg(strBSDPath));
            bShowUp = true;
        }
        else
        {
            AlphaRobotLog::getInstance()->printError("Failed to get UBT disk info: getUBTDiskInfo");
        }

        AlphaRobotLog::getInstance()->printError(QString("UBT disk info: getUBTDiskInfo  bsd: %1, disk: %2").arg(strBSDPath).arg(strDiskName));
#endif

            break;
        }

        Sleep(UpdateModuleConstant::TIMEOUT_1000MS);
    }

    emit sigIsUdiskShowUp(bShowUp);
}
