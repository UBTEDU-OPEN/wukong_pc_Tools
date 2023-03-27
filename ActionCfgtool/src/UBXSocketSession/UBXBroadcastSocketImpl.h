#ifndef DEVMONITOR_H
#define DEVMONITOR_H

#include <QObject>
#include "robotdef.h"
#include "Header.h"
#include "UBXBroadcastSocket.h"

#define BROADCAST_PORT   6000

QT_BEGIN_NAMESPACE
class QUdpSocket;
QT_END_NAMESPACE

class CUBXBroadcastSocketImpl : public QObject, public CIUBXBroadcastSocket
{
    Q_OBJECT
public:
    explicit CUBXBroadcastSocketImpl(QObject *parent = 0);
    virtual ~CUBXBroadcastSocketImpl();

signals:
    void showDev(DevInfo &dev);

private slots:
    void processPendingDatagrams();
    void regDev(DevInfo &dev);

public:
    virtual bool Initial(HWND m_hWnd,IUBXBroadcastEvent* pEvent);
    virtual bool Release(void);

private:
    int ParseHeaderData(char * pData, int nLen, CHeader &header);
    void ParseMsg(char* pMsg , int nLen);
    bool SendMsg(int nId , const std::string& msgBody);

private:
    QUdpSocket *udpSocket;
    QList<QString>  devLst;
    IUBXBroadcastEvent *m_pEvent;
};

#endif // QDEVMONITOR_H
