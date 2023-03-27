#include "stdafx.h"
#include "UBXBroadcastSocketImpl.h"
#include <QtNetwork>
#include <QDebug>

//#include "../LibPackClass/UBXDevInfo.h"
//#include "../LibPackClass/HeaderCmd.h"

#include "protocoldef.h"

#include "AlphaMessage.pb.h"
#include "AlphaMessageHeader.pb.h"
#include "AlPcWifiAccessPoint.pb.h"

bool CreateUBXBroadcastSocket(CIUBXBroadcastSocket **ppSession)
{
    CUBXBroadcastSocketImpl* pProxy = new CUBXBroadcastSocketImpl();
    if (pProxy)
    {
        *ppSession = pProxy;
        return true;
    }
    return false;
}

CUBXBroadcastSocketImpl::CUBXBroadcastSocketImpl(QObject *parent)
    : QObject(parent)
{
}

CUBXBroadcastSocketImpl::~CUBXBroadcastSocketImpl()
{
    delete udpSocket;
}

bool CUBXBroadcastSocketImpl::Initial(HWND m_hWnd, IUBXBroadcastEvent* pEvent)
{
    if (pEvent)
    {
        m_pEvent = pEvent;
    }

    udpSocket = new QUdpSocket(this);
    udpSocket->bind(BROADCAST_PORT, QUdpSocket::ShareAddress);

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    SendMsg(REQ_GET_WIFI_ACCESS_POINTS , "");

//    if (!InitialBCSocket(m_nPort,m_pBroadcastAddr))
//    {
//        return false;
//    }

//	m_bMainThread = true;
//	m_hNotifyWnd = m_hWnd;
//	return StartThread();
    return true;
}

bool CUBXBroadcastSocketImpl::Release(void)
{
//	bool bStatus =true;
//	if (m_BCRecvSock)
//	{
//		closesocket(m_BCRecvSock);
//	}
//	if (m_BCSendSock)
//	{
//		closesocket(m_BCSendSock);
//	}
//	WaitForStop();
//	ClearDataList();   //清除new出来的数据
//	ClearNetAdaptersList();
//	delete this;  //删除new出来的指

    return false;
}

// protobuf发送消息
bool CUBXBroadcastSocketImpl::SendMsg(int nId , const std::string& msgBody)
{
    AlphaMessage sendMsg;

    AlphaMessageHeader* msgHeader = sendMsg.mutable_header();
    msgHeader->set_commandid(nId);
    msgHeader->set_versioncode("1");
    msgHeader->set_sendserial(1);
    msgHeader->set_responseserial(0);

    sendMsg.set_bodydata(msgBody);

    std::string outdata = sendMsg.SerializeAsString();

    CHeader header;
    header.SetMsg(outdata.c_str(), outdata.size());

    char* pData = NULL;
    int nLen = 0;
    nLen = header.GetHeaderData(&pData);
    if (pData && nLen)
    {
        udpSocket->writeDatagram(pData , nLen , QHostAddress::Broadcast , BROADCAST_PORT);
        delete [] pData;
    }

    sendMsg.clear_header();

    return true;
}

void CUBXBroadcastSocketImpl::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        udpSocket->readDatagram(datagram.data(), datagram.size());

        AlphaMessage recvMsg;
        recvMsg.ParseFromArray(datagram.data() , datagram.size());

        AlphaMessageHeader msgHeader = recvMsg.header();
        std::string msgBody = recvMsg.bodydata();

        int nCommandId = msgHeader.commandid();
        switch (nCommandId)
        {
        case RSP_GET_WIFI_LINK_POINTS:
            {
                AlPcWifiAccessPointResponse wifiRsp;
                wifiRsp.ParseFromString(msgBody);
                QString strIpAddr = QString::fromStdString(wifiRsp.stripaddress());

                if (m_pEvent)
                {
                    m_pEvent->OnBroadcastEvent_DevInfo(strIpAddr);
                }
            }
        	break;
        default:
            break;
        }
    }
}

void CUBXBroadcastSocketImpl::regDev(DevInfo &dev)
{
    if( devLst.contains(dev.strMacAddress) )
    {
        qDebug()<<"old dev";
        return ;
    }

    devLst.append(dev.strMacAddress);
    emit showDev(dev);
}

void CUBXBroadcastSocketImpl::ParseMsg(char* pMsg , int nLen)
{
    if (pMsg == NULL)
        return;
    /*
    msgpack::unpacked sesult;
    msgpack::unpack(&sesult, pMsg, nLen);
    msgpack::object obj = sesult.get();
    CHeaderCmd s;
    obj.convert(&s);
    msgpack::unpacked sesult1;
    msgpack::object obj2;
    if (s.GetMsg())
    {
        msgpack::unpack(&sesult1, s.GetMsg(), s.GetMsgLen());
        obj2 = sesult1.get();
    }

    switch(s.GetMsgID())
    {
    case ALPHA_MSG_RSP_FILESERVERINFO:
        {
            CUBXDevInfo ubxDevInfo;
            obj2.convert(&ubxDevInfo);

            DevInfo devInfo;
            std::string sTemp="";
            ubxDevInfo.getDevName(sTemp);
            devInfo.strDevname =  QString::fromStdString(sTemp);

            ubxDevInfo.getIpAddress(sTemp);
            devInfo.strIpAddress = QString::fromStdString(sTemp);

            if (m_pEvent)
            {
                //m_pEvent->OnBroadcastEvent_DevInfo(devInfo);
            }
 //           std::string sTemp="";
 //           ubxDevInfo.getDevName(sTemp);
 //           devInfo.strDevname =  CString(sTemp.c_str());

 //           ubxDevInfo.getOsVersion(sTemp);
 //           devInfo.strOsVersion = CString(sTemp.c_str());

 //           ubxDevInfo.getIpAddress(sTemp);
 //           devInfo.strIpAddress = CString(sTemp.c_str());

 //           ubxDevInfo.getMacAddress(sTemp);
 //           devInfo.strMacAddress = CString(sTemp.c_str());

 //           emit regDev(devInfo);
        }
        break;

    default:
        break;
    }*/
}

int CUBXBroadcastSocketImpl::ParseHeaderData(char * pData, int nLen, CHeader &header)
{
    /*
    short	m_nFlag;
    // 包长度
    int		m_nLength;
    // 协议版本
    short	m_nVersion;
    // 辅助信息
    short	m_nExtInfo;
    // 消息具体内容
    char*	m_pMsg;
    // 消息内容长度
    int		m_nMsgLen;*/
    //pData += Offset;
    int offset = 0;
    while ( ( nLen - offset) >10 )
    {
        if (-1 == header.SetHeaderData(pData , nLen, offset))
        {
            break;
        }
        ParseMsg(header.m_pMsg ,header.m_nMsgLen);
    }

    //header.SetHeaderData(pData, &nLen);
    return 0;
}
