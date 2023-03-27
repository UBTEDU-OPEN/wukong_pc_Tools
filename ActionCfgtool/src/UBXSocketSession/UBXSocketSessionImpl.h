#pragma once
#include "IUBXSocketSession.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QThreadPool>
#include <QWaitCondition>
#include <QMutex>
#include "type.h"

class QMutex;
class QTimer;
class CDataSend
{
public:
	char* m_pDataToSend;
	int m_nDataLen;

public:
	bool SetData(const char* pData, int nLen);
};

class CUBXSocketSessionImpl;
class CProcessSendData: public QRunnable
{
public:
    CProcessSendData();
    ~CProcessSendData();

    bool Connection(CUBXSocketSessionImpl* pSession);
    bool ReleaseConnection();

private:
    bool					m_bStop;
    CUBXSocketSessionImpl*	m_pSession;

public:
    /**************************************************************************
    * 函数名: run
    * 功能: 向机器人发送iq包的工作者线程
    * 参数: 无
    * 返回值:void
    * 时间:2016/12/26/15:58
    * 作者: wanxf
    **************************************************************************/
    void run();
};

class UBXSOCKETSESSIONSHARED_EXPORT CUBXSocketSessionImpl: public QObject, public CIUBXSocketSession
{
    Q_OBJECT
public:
    explicit CUBXSocketSessionImpl(QObject *parent = 0);
	~CUBXSocketSessionImpl(void);

public:
    void run();
    virtual bool Connect(QString strServerIPAddress, int nTimeOutSec, /*HWND hWnd,*/ IUBXSocketSessionEvent* pEvent);
    virtual bool StopConnect();
    virtual void ReleaseConnection();
    virtual bool SendData(const char* pData, int nLen);
    virtual bool SendObj(int nId, QObject* pObj);

    virtual bool SendMsg(int nId , const std::string& msgBody);

    virtual bool UBXSocketSession_SetUploadFileInfo(QString strFilePath , QString strFilename, int nFileLen , short nType, QString strMD5 = "");
    virtual bool UBXSocketSession_QueryVersion(int nType);

    // 胸板升级
    virtual bool UBXSocketSession_UpgradeChestReq(int nType, int nFileSize);
    virtual bool UBXSocketSession_WriteChestData(int nPageNum, char* pData , int nDataLen);
    virtual bool UBXSocketSession_WriteCheckCode(const char* chMd5 , int nMd5Len);
//    virtual bool UBXSocketSession_EnterFastBootMode();

    virtual bool UBXSocketSession_GetExportFileSource(QString strFilename);
    virtual bool UBXSocketSession_GetDetailFileInfo(QString strFilename);

    virtual void UBXSocketSession_GetActionList();
    virtual void UBXSocketSession_PlayActionFile(QString strFileName);
    virtual void UBXSocketSession_DeleteActionFile(QString strFileName);
    virtual void UBXSocketSession_SetMotorRotation(int nID, int nAngle, int iTime);
    virtual void UBXSocketSession_SetAllAngleToAlpha(MOTOR_DATA* pMotor, int nMotorCount, int nTime, int nCurveType);
    virtual void UBXSocketSession_ReadMotorRotation(int nID, bool bPowerDown);
    virtual void UBXSocketSession_StopPlay();
    virtual void UBXSocketSession_PlayLedData(int type,byte leftLed, byte rightLed, byte bright, byte color, int runTime, int lightUpTime, int lightDownTime, bool bEnd);
    virtual void UBXSocketSession_StopPlayLedData(int type);
    virtual void UBXSocketSession_PlayExpression(QString strFileName, int nCurFrame);
    virtual void UBXSocketSession_ConnectToRobot(int nToRobotId);
    virtual void UBXSocketSession_DownloadApkToRobot(QString downloadUrl);
    virtual void UBXSocketSession_UninstallApkFromRobot(QString apkName);
    virtual void UBXSocketSession_GetInstallApkList();
    virtual void UBXSocketSession_SetChargeAndPlay(bool bOpen);
    //获取窗体
    virtual QObject* GetObject();

public:
	bool ProcessSendData();

public slots:
  void onTimerOut();
  void readForReady();
  void displayError(QAbstractSocket::SocketError socketError);
  void tcpConnected();
  void tcpDisconnected();
  void slotMotionPlay(MOTOR_DATA* pMotor, int nMotorCount, int nRunTime, int nCurveType);

signals:
    void sigRobotConnected();
    void sigRobotDisconnected();

private:
	void ClearDataSendList();
	void AddDataToSendList(CDataSend* pSend);
	// 返回 -1 Socket错误
	int ReadFromSocket();
	void ParseMsg(char* pMsg, int nLen);
	// 发送心跳包
	void SendXT();
	bool SendLoginCmd(QString strPassword);
	
private:
	bool				m_bStop;
    QTcpSocket			*m_Socket;
	int					m_nTimeOutSec;	
	//HWND				m_hNotifyWnd;
    std::string         m_strServerIPAddress;
    QList<CDataSend*>	m_listDataToSend;
	bool				m_bConnectToServer;
//    CProcessSendData	m_ProcessSendData;
	long				m_lLastRcvDataTime;		// 上一次接收到数据的时间，用于超时用
	long				m_lLastSendDataTime;	// 最后一次发送数据的时间
    long                m_nSerialNum;

    int                 m_nReadTotalLen;        // 待接收数据总长度
    int                 m_nRecvDataLen;         // 已接收数据长度
    char*               m_pchRecvBuf;           // 接收数据缓存区

    CProcessSendData*	m_ProcessSendData;
    QMutex              m_mutex;
    QString             m_sendFilePath;
	IUBXSocketSessionEvent*	m_pEvent;
    QTimer *m_MyTimer;
    QMutex m_oMutex;
    QWaitCondition m_oWait;
	
public:
	bool IsConnected(void);
};

