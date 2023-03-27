#include "smtpclient.h"
#include <QFileInfo>
#include <QByteArray>

SmtpClient::SmtpClient(const QString & host, int port, ConnectionType connectionType) :
    socket(NULL),
    name("localhost"),
    authMethod(AuthPlain),
    connectionTimeout(5000),
    responseTimeout(20000),
    sendMessageTimeout(60000),
    sendBytes (0)
{
    setConnectionType(connectionType);
    this->host = host;
    this->port = port;
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(socketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(()),
            this, SLOT(socketReadyRead()));
    connect(socket, SIGNAL(), this, SLOT());
}

SmtpClient::~SmtpClient() {
    if (socket)
        delete socket;
}


void SmtpClient::setUser(const QString &user)
{
    this->user = user;
}

void SmtpClient::setPassword(const QString &password)
{
    this->password = password;
}

void SmtpClient::setAuthMethod(AuthMethod method)
{
    this->authMethod = method;
}

void SmtpClient::setHost(const QString &host)
{
    this->host = host;
}

void SmtpClient::setPort(int port)
{
    this->port = port;
}

void SmtpClient::setConnectionType(ConnectionType ct)
{
    this->connectionType = ct;

    if (socket)
        delete socket;

    switch (connectionType)
    {
    case TcpConnection:
        socket = new QTcpSocket(this);
        break;
    case SslConnection:
    case TlsConnection:
        socket = new QSslSocket(this);
    }
}

const QString& SmtpClient::getHost() const
{
    return this->host;
}

const QString& SmtpClient::getUser() const
{
    return this->user;
}

const QString& SmtpClient::getPassword() const
{
    return this->password;
}

SmtpClient::AuthMethod SmtpClient::getAuthMethod() const
{
    return this->authMethod;
}

int SmtpClient::getPort() const
{
    return this->port;
}

SmtpClient::ConnectionType SmtpClient::getConnectionType() const
{
    return connectionType;
}

const QString& SmtpClient::getName() const
{
    return this->name;
}

void SmtpClient::setName(const QString &name)
{
    this->name = name;
}

const QString & SmtpClient::getResponseText() const
{
    return responseText;
}

int SmtpClient::getResponseCode() const
{
    return responseCode;
}

QTcpSocket* SmtpClient::getSocket() {
    return socket;
}

int SmtpClient::getConnectionTimeout() const
{
    return connectionTimeout;
}

void SmtpClient::setConnectionTimeout(int msec)
{
    connectionTimeout = msec;
}

int SmtpClient::getResponseTimeout() const
{
    return responseTimeout;
}

void SmtpClient::setResponseTimeout(int msec)
{
    responseTimeout = msec;
}
int SmtpClient::getSendMessageTimeout() const
{
  return sendMessageTimeout;
}
void SmtpClient::setSendMessageTimeout(int msec)
{
  sendMessageTimeout = msec;
}

bool SmtpClient::connectToHost()
{
    switch (connectionType)
    {
    case TlsConnection:
    case TcpConnection:
        socket->connectToHost(host, port);
        break;
    case SslConnection:
        ((QSslSocket*) socket)->connectToHostEncrypted(host, port);
        break;

    }

    // Tries to connect to server
    if (!socket->waitForConnected(connectionTimeout))
    {
        emit smtpError(ConnectionTimeoutError);
        return false;
    }

    try
    {
        // Wait for the server's response
        waitForResponse();

        // If the response code is not 220 (Service ready)
        // means that is something wrong with the server
        if (responseCode != 220)
        {
            emit smtpError(ServerError);
            return false;
        }

        // Send a EHLO/HELO message to the server
        // The client's first command must be EHLO/HELO
        sendMessage("EHLO " + name);

        // Wait for the server's response
        waitForResponse();

        // The response code needs to be 250.
        if (responseCode != 250) {
            emit smtpError(ServerError);
            return false;
        }

        if (connectionType == TlsConnection) {
            // send a request to start TLS handshake
            sendMessage("STARTTLS");

            // Wait for the server's response
            waitForResponse();

            // The response code needs to be 220.
            if (responseCode != 220) {
                emit smtpError(ServerError);
                return false;
            };

            ((QSslSocket*) socket)->startClientEncryption();

            if (!((QSslSocket*) socket)->waitForEncrypted(connectionTimeout)) {
                qDebug() << ((QSslSocket*) socket)->errorString();
                emit smtpError(ConnectionTimeoutError);
                return false;
            }
            sendMessage("EHLO " + name);
            waitForResponse();
            if (responseCode != 250) {
                emit smtpError(ServerError);
                return false;
            }
        }
    }
    catch (ResponseTimeoutException)
    {
        return false;
    }
    catch (SendMessageTimeoutException)
    {
        return false;
    }

    // If no errors occured the function returns true.
    return true;
}

bool SmtpClient::login()
{
    return login(user, password, authMethod);
}

bool SmtpClient::login(const QString &user, const QString &password, AuthMethod method)
{
    try {
        if (method == AuthPlain)
        {
            sendMessage("AUTH PLAIN " + QByteArray().append((char) 0).append(user).append((char) 0).append(password).toBase64());
            waitForResponse();

            if (responseCode != 235)
            {
                emit smtpError(AuthenticationFailedError);
                return false;
            }
        }
        else if (method == AuthLogin)
        {
            sendMessage("AUTH LOGIN");
            waitForResponse();
            if (responseCode != 334) { emit smtpError(AuthenticationFailedError); return false; }
            sendMessage(QByteArray().append(user).toBase64());
            waitForResponse();
            if (responseCode != 334) { emit smtpError(AuthenticationFailedError); return false; }
            sendMessage(QByteArray().append(password).toBase64());
            waitForResponse();
            if (responseCode != 235)
            {
                emit smtpError(AuthenticationFailedError);
                return false;
            }
        }
    }
    catch (ResponseTimeoutException)
    {
        emit smtpError(AuthenticationFailedError);
        return false;
    }
    catch (SendMessageTimeoutException)
    {
        emit smtpError(AuthenticationFailedError);
        return false;
    }

    return true;
}

bool SmtpClient::sendMail(MimeMessage& email, std::function<void(int sum, int sended)> func)
{
    try
    {
        sendMessage("MAIL FROM: <" + email.getSender().getAddress() + ">");
        waitForResponse();
        if (responseCode != 250)
            return false;
        QList<EmailAddress*>::const_iterator it, itEnd;
        for (it = email.getRecipients().begin(), itEnd = email.getRecipients().end();
             it != itEnd; ++it)
        {
            sendMessage("RCPT TO:<" + (*it)->getAddress() + ">");
            waitForResponse();
            if (responseCode != 250)
                return false;
        }
        for (it = email.getRecipients(MimeMessage::Cc).begin(), itEnd = email.getRecipients(MimeMessage::Cc).end();
             it != itEnd; ++it)
        {
            sendMessage("RCPT TO:<" + (*it)->getAddress() + ">");
            waitForResponse();

            if (responseCode != 250)
                return false;
        }
        for (it = email.getRecipients(MimeMessage::Bcc).begin(), itEnd = email.getRecipients(MimeMessage::Bcc).end();
             it != itEnd; ++it)
        {
            sendMessage("RCPT TO: <" + (*it)->getAddress() + ">");
            waitForResponse();

            if (responseCode != 250)
                return false;
        }
        sendMessage("DATA");
        waitForResponse();
        if (responseCode != 354)
            return false;
        sendMessage(email.toString());
        sendMessage(".");
        waitForResponse();
        if (responseCode != 250)
            return false;
    }
    catch (ResponseTimeoutException)
    {
        return false;
    }
    catch (SendMessageTimeoutException)
    {
        return false;
    }
    return true;
}

void SmtpClient::quit()
{
    try 
    {
        sendMessage("QUIT");
    }
    catch(SmtpClient::SendMessageTimeoutException) 
    {
        if(socket->state() == QAbstractSocket::ConnectedState || socket->state() == QAbstractSocket::ConnectingState || socket->state() == QAbstractSocket::HostLookupState)
            socket->disconnectFromHost();
    }
}


void SmtpClient::waitForResponse()
{
    do {
        if (!socket->waitForReadyRead(responseTimeout)) {
            emit smtpError(ResponseTimeoutError);
            throw ResponseTimeoutException();
        }
        while (socket->canReadLine()) {
            responseText = socket->readLine();
            responseCode = responseText.left(3).toInt();
            if (responseCode / 100 == 4)
                emit smtpError(ServerError);
            if (responseCode / 100 == 5)
                emit smtpError(ClientError);
            if (responseText[3] == ' ') {
                return;
            }
        }
    } while (true);
}

void SmtpClient::sendMessage(const QString &text)
{
    socket->write(text.toUtf8() + "\r\n");
    if (!socket->waitForBytesWritten(sendMessageTimeout)) {
      emit smtpError(SendDataTimeoutError);
      throw SendMessageTimeoutException();
    }
}
void SmtpClient::socketStateChanged(QAbstractSocket::SocketState /*state*/)
{
}

void SmtpClient::socketError(QAbstractSocket::SocketError /*socketError*/)
{
}

void SmtpClient::socketReadyRead()
{
}




