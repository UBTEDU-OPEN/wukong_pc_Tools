#ifndef __Q_CLIENT_H_
#define __Q_CLIENT_H_

#include <QObject>
#include<QtNetwork/QTcpSocket>
#include<QtNetwork/QTcpServer>
#include <iostream>
#include <string>
#include <sstream>

class QClient : QObject
{
    Q_OBJECT
public:
    enum ClientStatus
    {
        CLIENT_STATUS_IDLE = 0,
        CLIENT_STATUS_OK,
        CLIENT_STATUS_NOK,
        CLIENT_STATUS_DISCONNECTED
    };

    QClient();
    ~QClient();
    bool init();
    void sendData(QByteArray bytes);
public slots:
    void onNewConnection();
    void onReceiveMessage();
    void onConnectionError();
    void sendLogoutMessage();
private:
    std::string getStatusDescription();

    const char* SERVER_IP;
    int SERVER_PORT;
    QTcpSocket m_socket;
    ClientStatus m_clientStatus;
    quint16 m_blockSize;
};

#endif
