#ifndef __Q_SERVER_H
#define __Q_SERVER_H

#include <QTcpServer>
#include <iostream>
#include <string>
#include <sstream>

class QServerThread;

class QServer : public QTcpServer
{
    Q_OBJECT

public:
    friend class QServerThread;

    enum ServerStatus
    {
        SERVER_STATUS_IDLE = 0,
        SERVER_STATUS_OK = 1,
        SERVER_STATUS_NOK = 2
    };

    QServer();
    bool init();

public slots:
    void onNewConnection();
    void printClients();

private:
    std::string getStatusDescription();

    const char* SERVER_IP;
    int SERVER_PORT;
    ServerStatus m_serverStatus;
    QMap<int, QServerThread*> m_clients;
    int m_numMaxClients;
};

#endif
