#include "stdafx.h"
#include "UBXUpLoadSocketImpl.h"
#include <QtNetwork>
#include <QDebug>
#include "constant.h"
#include "configs.h"

bool CreateUploadFileSocket(IUploadFileSession **ppSession)
{
    CUBXUpLoadSocketImpl* pProxy = new CUBXUpLoadSocketImpl();
    if (pProxy)
    {
        *ppSession = pProxy;
        return true;
    }
    return false;
}

CUBXUpLoadSocketImpl::CUBXUpLoadSocketImpl(QObject *parent)
    : QObject(parent)
    , m_nFileLen(0)
    , m_nRecvDataLen(0)
    , m_pFile(NULL)
{
    
}

CUBXUpLoadSocketImpl::~CUBXUpLoadSocketImpl()
{
}

bool CUBXUpLoadSocketImpl::Initial(QString strIPAddr, int nPort, QString strFileName, int nFileLen, IFileTransferEvent* pEvent)
{
    m_pEvent = pEvent;
    m_nFileLen = nFileLen;
    m_strFileName = strFileName;

    if (strFileName.contains("/"))
    {
        QString actionName = USERDIR_ACTIONLIB;
        actionName += QDir::separator() + strFileName.left(strFileName.indexOf("/"));
        QDir musicDir = CConfigs::getUserDirPath(actionName);
        if(!musicDir.exists())
            musicDir.mkpath(musicDir.absolutePath());
    }

    // 保存文件路径
    QString strFilePath = CConfigs::getUserDirPath(USERDIR_ACTIONLIB) + QDir::separator() + strFileName;
    m_pFile = new QFile(strFilePath);
    m_pFile->open(QIODevice::WriteOnly);

    m_receiveSocket = new QTcpSocket(this);
    connect(m_receiveSocket, SIGNAL(readyRead()), this, SLOT(StartReadData()));
    connect(m_receiveSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));

    m_receiveSocket->connectToHost(QHostAddress(strIPAddr), nPort);

    return true;
}

void CUBXUpLoadSocketImpl::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        break;
    case QAbstractSocket::ConnectionRefusedError:
        break;
    default:
        break;
    }
    QString strErr = m_receiveSocket->errorString();
    qDebug()<<"The following error occurred:"<<strErr<<endl;
}

void CUBXUpLoadSocketImpl::ReleaseConnection()
{
    m_receiveSocket->close();
    delete this;
}

void CUBXUpLoadSocketImpl::StartReadData()
{

    QByteArray array = m_receiveSocket->readAll();
    m_nRecvDataLen += array.count();

    qDebug()<<"recv export IP "<<m_strFileName<< QString::number(m_nRecvDataLen) <<"--" + QString::number(m_nFileLen)<<endl;
    //AddLog("recv export IP " + m_strFileName + QString::number(m_nRecvDataLen) + "--" + QString::number(m_nFileLen));

    // 刷新进度条
    if (m_pEvent)
    {
        m_pEvent->OnFileTransferEvent_TransProgress(m_strFileName, m_nRecvDataLen , m_nFileLen , SENDING);
    }

    m_pFile->write(array);

    if (m_nRecvDataLen == m_nFileLen)
    {
        m_pFile->close();
        if (m_pEvent)
        {
            m_pEvent->OnFileTransferEvent_TransProgress(m_strFileName, m_nRecvDataLen , m_nFileLen , SENDEND);
            m_pEvent->OnFileTransferEvent_UploadNextFile();
        }
    }
    


    //if ((m_receiveSocket->bytesAvailable()) >= m_nFileLen)
    //{
    //    QByteArray datagram;
    //    datagram.resize(m_receiveSocket->bytesAvailable());
    //    m_receiveSocket->read(datagram.data(), datagram.size());
    //    m_file = new QFile(strDirPath);
    //    m_file->open(QIODevice::WriteOnly);
    //    m_file->write(datagram);
    //    m_file->close();

    //    m_pEvent->OnFileTransferEvent_TransProgress(0, m_filename, 0 , 0 , 2);
    //    m_pEvent->OnFileTransferEvent_UploadNextFile();
    //}
}
