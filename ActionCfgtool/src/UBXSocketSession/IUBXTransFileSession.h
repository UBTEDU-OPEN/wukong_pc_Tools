#include "ubxsocketsession_global.h"

class IFileTransferEvent
{
public:
    virtual void OnFileTransferEvent_SendNextFile() = 0;
    virtual void OnFileTransferEvent_UploadNextFile() = 0;
    virtual void OnFileTransferEvent_TransProgress(QString strFileName, int nSendData , int nFileLen , int nStatus) = 0;
};

class ISendFileSession
{
public:
    virtual bool Initial(QString strIPAddr, int nPort, QString strFilePath, IFileTransferEvent* pEvent) = 0;
    virtual void ReleaseConnection() = 0;
};
bool UBXSOCKETSESSIONSHARED_EXPORT CreateSendFileSocket(ISendFileSession** ppSession);

class IUploadFileSession
{
public:
    virtual bool Initial(QString strIPAddr, int nPort, QString strFilePath, int nFileLen, IFileTransferEvent* pEvent) = 0;
    virtual void ReleaseConnection() = 0;
};
bool UBXSOCKETSESSIONSHARED_EXPORT CreateUploadFileSocket(IUploadFileSession** ppSession);
