//#pragma once
#ifndef IUBXSOCKETSESSIONEVENT_H
#define IUBXSOCKETSESSIONEVENT_H

#include "ubxsocketsession_global.h"
#include <QObject>
#include <QString>
#include "type.h"
#include "robotdef.h"

typedef unsigned char byte;
//typedef void*    HWND;

#define WM_ALPHA_SOCKET_SESSION_NOTIFY	WM_USER + 100

#define ALPHA_SESSION_CONNECTTING_SERVER	1

#define ALPHA_SESSION_CONNECTED_SERVER		2

#define ALPHA_SESSION_CONNECTED_FAILD		3

#define ALPHA_SESSION_AUTH_ERROR			4

#define ALPHA_SESSION_CONNECTION_REQUEST    5

#define ALPHA_SESSION_SERVERINFO_ISCOMING   6

#define ALPHA_SESSION_UNINSTALL_APK_RESULT   7

#define ALPHA_SESSION_INSTALL_APK_RESULT  8

typedef struct __WIFI_CONNECT_INFO
{
    QString ssid;
    QString	psw;
    QString encryty;
	bool knowWifi;
} WIFI_CONNECT_INFO;


class IUBXSocketSessionEvent
{
public:
    virtual void OnUBXSocketSessionEvent_ActionFileList(QStringList fileList) = 0;
    virtual void OnUBXSocketSessionEvent_ExportFileSource(QString strFileName , int nFileSize , bool bGetNextFile) = 0;
    virtual void OnUBXSocketSessionEvent_ImportFile(int nPort , QString strIp , QString strFileName) = 0;
    virtual void OnUBXSocketSessionEvent_ExportFile(int nPort , QString strIp , QString strFileName) = 0;
//	virtual void OnUBXSocketSessionEvent_ServerInfo_Update(FileServerInfo_DL* pServInfo) = 0;
//	virtual void OnUBXSocketSessionEvent_GetWifiList(vector<ShowWifi> showWifi) = 0;
	virtual void OnUBXSocketSessionEvent_ReadMotorAngle(int nId, int nAngle) = 0;
    virtual void OnUBXSocketSessionEvent_XmppPresenceChange(int isOnLine, QString deviceName) = 0;
    virtual void OnUBXSocketSessionEvent_SetConnectRobotState(bool bState) = 0;
//	virtual void OnUBXSocketSessionEvent_AccountOfflined() = 0;
    virtual void OnUBXSocketSessionEvent_UploadNextFile() = 0;
    virtual void OnUBXSocketSessionEvent_ExportNextFile() = 0;
//  virtual void OnUBXSocketSessionEvent_XmppUpdateInfo_DL(QString strFilePath,int nDataSent,float fSpeed,int nFileLen ,int nStatus) = 0;
//  virtual void OnUBXSocketSessionEvent_XmppUpdateInfo_UL(QString strFilePath,int nDataRecv,float fSpeed,int nFileLen ,int nStatus) = 0;
//	virtual void OnUBXSocketSessionEvent_UpdateApkList() = 0;

    virtual void OnUBXSocketSessionEvent_SetInstalledAppList(QString appName) = 0;
    virtual void OnUBXSocketSessionEvent_SetTransProgress(int transType, QString strFileName, int transSize , int nStatus) = 0;
};


class CIUBXSocketSession
{
public:
    virtual bool Connect(QString strServerIPAddress, int nTimeOutSec,/* HWND hWnd, */IUBXSocketSessionEvent* pEvent) = 0;

	virtual bool StopConnect() = 0;
    virtual void ReleaseConnection() = 0;
    virtual QObject* GetObject() = 0;

	virtual bool SendData(const char* pData, int nLen) = 0;
    virtual bool SendObj(int nId, QObject* pObj) = 0;	
    //virtual bool SendFileInfo(const char* strFilename, int nFileLen) = 0;
//	virtual void UBXSocketSession_GetDeviceList() = 0;

	virtual void UBXSocketSession_GetActionList() = 0;
    virtual bool UBXSocketSession_SetUploadFileInfo(QString strFilePath,QString strFilename, int nFileLen , short nType, QString strMD5 = "") = 0;

	virtual bool UBXSocketSession_QueryVersion(int nType) = 0;

    virtual bool UBXSocketSession_UpgradeChestReq(int nType, int nFileSize) = 0;
    virtual bool UBXSocketSession_WriteChestData(int nPageNum, char* pData , int nDataLen) = 0;
    virtual bool UBXSocketSession_WriteCheckCode(const char* chMd5, int nMd5Len) = 0;

//	virtual bool UBXSocketSession_QueryChestVer() = 0;
//	virtual bool UBXSocketSession_EnterFastBootMode() = 0;
    virtual bool UBXSocketSession_GetExportFileSource(QString strFilename) = 0;
    virtual bool UBXSocketSession_GetDetailFileInfo(QString strFilename) = 0;

    virtual void UBXSocketSession_PlayActionFile(QString strFileName) = 0;
    virtual void UBXSocketSession_DeleteActionFile(QString strFileName) = 0;

	virtual void UBXSocketSession_SetMotorRotation(int nID, int nAngle, int iTime) = 0;
    virtual void UBXSocketSession_SetAllAngleToAlpha(MOTOR_DATA* pMotor, int nMotorCount, int nTime, int nCurveType) = 0;
	virtual void UBXSocketSession_ReadMotorRotation(int nID, bool bPowerDown) = 0;
	virtual void UBXSocketSession_StopPlay() = 0;

	virtual void UBXSocketSession_PlayLedData(int type,byte leftLed, byte rightLed, byte bright, byte color, int runTime, int lightUpTime, int lightDownTime, bool bEnd) = 0;
	virtual void UBXSocketSession_StopPlayLedData(int type) = 0;
    virtual void UBXSocketSession_PlayExpression(QString strFileName, int nCurFrame) = 0;

    virtual void UBXSocketSession_ConnectToRobot(int nToRobotId) = 0;
    virtual void UBXSocketSession_DownloadApkToRobot(QString downloadUrl) = 0;
    virtual void UBXSocketSession_UninstallApkFromRobot(QString apkName) = 0;
	virtual void UBXSocketSession_GetInstallApkList() = 0;

    virtual void UBXSocketSession_SetChargeAndPlay(bool bOpen) = 0;
};

//XMPPSESSIONSHARED_EXPORT bool CreateUBXSocketSession(CIUBXSocketSession** ppSession);
//XMPPSESSIONSHARED_EXPORT bool CreateUBXXmppSession(CIUBXSocketSession** ppSession);

UBXSOCKETSESSIONSHARED_EXPORT bool CreateUBXSocketSession(CIUBXSocketSession** ppSession);
UBXSOCKETSESSIONSHARED_EXPORT bool CreateUBXXmppSession(CIUBXSocketSession** ppSession);
UBXSOCKETSESSIONSHARED_EXPORT  bool PreCreateSocketSession(int nFromUserId, QString UserPassword, IUBXSocketSessionEvent* pEvent);

#endif //IUBXSOCKETSESSIONEVENT_H
