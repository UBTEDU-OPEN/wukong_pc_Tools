
#include <QObject>
#include "IUBXTransFileSession.h"
#include <QTcpSocket>
#include <QFile>


class CUBXSendFileSocketImpl : public QObject, public ISendFileSession
{
    Q_OBJECT
public:
    explicit CUBXSendFileSocketImpl(QObject *parent = 0);
    virtual ~CUBXSendFileSocketImpl();

private slots:

    // 开始发送槽函数(网络连接)
    void StartSendFile();

    // 刷新传输进度
    void slotUpdateProgress(qint64 nChunkSize);

    void slotDisconnect();

public:
    virtual bool Initial(QString strIPAddr, int nPort, QString strFilePath, IFileTransferEvent* pEvent);
    virtual void ReleaseConnection();

private:
    QTcpSocket *m_tcpSocket;
    IFileTransferEvent *m_pEvent;
    QString m_strFilePath;

    QFile*      m_pFile;            // 发送文件
    int         m_nFileLen;         // 文件总长度
    int         m_nSendDataLen;     // 已发送数据长度
    QByteArray  sendData;
    char*       m_pchSendBuf;
    bool        m_bConnected;
};
