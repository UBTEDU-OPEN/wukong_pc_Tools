//
//  QServerThread.h
//  QTNetworkDemo
//
//  Created by mugx on 04/03/12.
//  Copyright (c) 2015 mugx. All rights reserved.

#ifndef QServerThread_h
#define QServerThread_h

#include "QServer.h"
#include <QRunnable>
#include <QTcpSocket>

class QServerThread : public QObject, public QRunnable
{
    Q_OBJECT
public:
    friend class QServer;

    enum ServerThreadStatus
    {
        SERVER_THREAD_STATUS_IDLE = 0,
        SERVER_THREAD_STATUS_OK,
        SERVER_THREAD_STATUS_NOK,
        SERVER_THREAD_STATUS_DISCONNECTED
    };

    QServerThread(int socketDescriptor, QServer* _server);
    ~QServerThread();
    std::string getClientConnectedIp();
signals:
    void error(int socketError, const QString &message);

protected:
    void run();

private:
    std::string getStatusDescription();
    void initSocket();
    void sendWelcomeMessage();
    void receiveLogoutMessage();
    void sendGoodbyeMessage();

    std::string m_ipAddress;
    QServer* m_server;
    ServerThreadStatus m_serverThreadStatus;
    QTcpSocket* m_socket;
    int m_socketDescriptor;
};

#endif
