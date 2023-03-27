#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <downloadmanager.h>

class UPDATELIBSHARED_EXPORT UpdateManager : public QObject
{
    Q_OBJECT
public:
    explicit UpdateManager(QObject *parent = 0);
    virtual  ~UpdateManager ();

signals:
    void    finished ();

public:
    void    checkUpdate ();
    QString getServerVersion ();
    bool    requestVersion ();

protected:
    void    initRequest (QNetworkRequest& request,
                         const QString& strUrl,
                         const QString& strAppId,
                         const QString& strAppKey,
                         const QString& strDeviceId);
    QString createVerifyCode (int nBits);
    int     waitForReply(QNetworkReply *reply);
    QString printNetReplyError(QNetworkReply* reply);
    QString getHostMacAddress();

public:
    DownloadManager*       getDownloadManager () { return m_pDonwloadManager; }

protected:
    DownloadManager*        m_pDonwloadManager;
    QNetworkAccessManager   m_pManager;
};

#endif // UPDATEMANAGER_H
