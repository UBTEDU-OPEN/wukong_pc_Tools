#include "stdafx.h"
#include "UBXSendFileSocketImpl.h"
#include <QtNetwork>
#include <QDebug>

// 发送数据块大小
#define CHUNK_SIZE 10*1024

bool CreateSendFileSocket(ISendFileSession **ppSession)
{
    CUBXSendFileSocketImpl* pProxy = new CUBXSendFileSocketImpl();
    if (pProxy)
    {
        *ppSession = pProxy;
        return true;
    }
    return false;
}

CUBXSendFileSocketImpl::CUBXSendFileSocketImpl(QObject *parent)
    : QObject(parent)
    , m_pFile(NULL)
    , m_nFileLen(0)
    , m_nSendDataLen(0)
    , m_pchSendBuf(NULL)
    , m_bConnected(false)
{

}

CUBXSendFileSocketImpl::~CUBXSendFileSocketImpl()
{
//    delete m_tcpSocket;
    delete[] m_pchSendBuf;
}

bool CUBXSendFileSocketImpl::Initial(QString strIPAddr, int nPort, QString strFilePath, IFileTransferEvent* pEvent)
{
    m_pEvent = pEvent;
    m_strFilePath = strFilePath;

    m_tcpSocket = new QTcpSocket(this);
    m_tcpSocket->connectToHost(QHostAddress(strIPAddr), nPort);
    connect(m_tcpSocket, SIGNAL(connected()), this, SLOT(StartSendFile()));
    connect(m_tcpSocket , SIGNAL(disconnected()) , this , SLOT(slotDisconnect()));

    
    return true;
}

void CUBXSendFileSocketImpl::ReleaseConnection()
{
    m_tcpSocket->close();
//    delete m_tcpSocket;
    delete this;
}

void CUBXSendFileSocketImpl::slotDisconnect()
{
    m_bConnected = false;
}

void CUBXSendFileSocketImpl::StartSendFile()
{
    // Add by jianjie 2017/7/5
    QObject::connect(m_tcpSocket , SIGNAL(bytesWritten(qint64)) , this , SLOT(slotUpdateProgress(qint64)));

    m_bConnected = true;

    m_pFile = new QFile(m_strFilePath);
    m_pFile->open(QIODevice::ReadOnly);

    m_nFileLen = m_pFile->size();

    if (NULL == m_pchSendBuf)
    {
        m_pchSendBuf = new char[CHUNK_SIZE];
    }

    int nTobeWritten = m_nFileLen - m_nSendDataLen;
    if (nTobeWritten > 0)
    {
        int nDataLen = m_pFile->read(m_pchSendBuf , qMin(nTobeWritten , CHUNK_SIZE));
        m_tcpSocket->write(m_pchSendBuf , nDataLen);
    }
    
    // Delete by jianjie 2017/7/3
//    QFile f(m_strFilePath);
//    f.open(QIODevice::ReadOnly);
////    QByteArray block;
//    QByteArray data = f.readAll();
////    QDataStream dts(&block, QIODevice::WriteOnly);
////    dts<<data;
//
//    qint64 nfileBlockLenth = m_tcpSocket->write(data);
//
//    if (m_pEvent)
//    {
//        QString strFileName = m_strFilePath.section('/', -1);
//        m_pEvent->OnFileTransferEvent_TransProgress(1, strFileName, nfileBlockLenth , nfileBlockLenth , 2);
//        m_pEvent->OnFileTransferEvent_SendNextFile();
//    }
}

/***************************************
 * 函数名: slotUpdateProgress
 * 功能:  数据发送槽函数，用于递归发送，并刷新进度条
 * 参数:  nChunkSize 当前发送数据块大小
 * 返回值: 
 * 时间:   2017/7/5 10:39
 * 作者:   jianjie
******************************************/
void CUBXSendFileSocketImpl::slotUpdateProgress(qint64 nChunkSize)
{
    qApp->processEvents();

    qDebug()<<"recv import IP "<< m_strFilePath << endl;
    //AddLog("recv import IP " + m_strFilePath);

    m_nSendDataLen += nChunkSize;

    // 待发送数据
    int nTobeWritten = m_nFileLen - m_nSendDataLen;
    if (nTobeWritten > 0 /*&& m_bConnected*/)
    {
        int nDataLen = m_pFile->read(m_pchSendBuf , qMin(nTobeWritten , CHUNK_SIZE));
        int nTemp = m_tcpSocket->write(m_pchSendBuf , nDataLen);

        sendData.resize(0);
    }

    QString strFileName = m_strFilePath.section('/', -1);

    // 刷新进度条
    if (m_pEvent)
    {
        m_pEvent->OnFileTransferEvent_TransProgress(strFileName, m_nSendDataLen , m_nFileLen , SENDING);
    }

    // 文件数据发送完，触发下一文件
    if (m_nSendDataLen == m_nFileLen)
    {
        m_pFile->close();
        if (m_pEvent)
        {
            m_pEvent->OnFileTransferEvent_TransProgress(strFileName, m_nSendDataLen , m_nFileLen , SENDEND);
            m_pEvent->OnFileTransferEvent_SendNextFile();
        }
    }
}