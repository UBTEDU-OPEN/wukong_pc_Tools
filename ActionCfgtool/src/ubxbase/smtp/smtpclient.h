#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QtNetwork/QSslSocket>
#include <functional>
#include "mimemessage.h"
#include "smtpexports.h"

class SMTP_EXPORT SmtpClient : public QObject
{
    Q_OBJECT
public:

    /* [0] Enumerations */

    enum AuthMethod
    {
        AuthPlain,
        AuthLogin
    };

    enum SmtpError
    {
        ConnectionTimeoutError,
        ResponseTimeoutError,
        SendDataTimeoutError,
        AuthenticationFailedError,
        ServerError,    // 4xx smtp error
        ClientError     // 5xx smtp error
    };

    enum ConnectionType
    {
        TcpConnection,
        SslConnection,
        TlsConnection       // STARTTLS
    };

    /* [0] --- */


    /* [1] Constructors and Destructors */

    SmtpClient(const QString & host = "localhost", int port = 25, ConnectionType ct = TcpConnection);

    ~SmtpClient();

    /* [1] --- */


    /* [2] Getters and Setters */

    const QString& getHost() const;
    void setHost(const QString &host);

    int getPort() const;
    void setPort(int port);

    const QString& getName() const;
    void setName(const QString &name);

    ConnectionType getConnectionType() const;
    void setConnectionType(ConnectionType ct);

    const QString & getUser() const;
    void setUser(const QString &user);

    const QString & getPassword() const;
    void setPassword(const QString &password);

    SmtpClient::AuthMethod getAuthMethod() const;
    void setAuthMethod(AuthMethod method);

    const QString & getResponseText() const;
    int getResponseCode() const;

    int getConnectionTimeout() const;
    void setConnectionTimeout(int msec);

    int getResponseTimeout() const;
    void setResponseTimeout(int msec);
    
    int getSendMessageTimeout() const;
    void setSendMessageTimeout(int msec);

    QTcpSocket* getSocket();


    /* [2] --- */


    /* [3] Public methods */

    bool connectToHost();

    bool login();
    bool login(const QString &user, const QString &password, AuthMethod method = AuthLogin);

    bool sendMail(MimeMessage& email, std::function<void (int sum, int sended)> func = NULL);
    void quit();


    /* [3] --- */

protected:

    /* [4] Protected members */

    QTcpSocket *socket;

    QString host;
    int port;
    ConnectionType connectionType;
    QString name;

    QString user;
    QString password;
    AuthMethod authMethod;

    int connectionTimeout;
    int responseTimeout;
    int sendMessageTimeout;
    
    
    QString responseText;
    int responseCode;

    int sendBytes;
    class ResponseTimeoutException {};
    class SendMessageTimeoutException {};

    /* [4] --- */


    /* [5] Protected methods */

    void waitForResponse();

    void sendMessage(const QString &text);

    /* [5] --- */

protected slots:

    /* [6] Protected slots */

    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketError(QAbstractSocket::SocketError error);
    void socketReadyRead();

    /* [6] --- */


signals:

    /* [7] Signals */

    void smtpError(SmtpClient::SmtpError e);

    /* [7] --- */

};

#endif // SMTPCLIENT_H
