#include "updateManager.h"
#include "urldef.h"
#include <QEventLoop>
#include <QTimer>
#include <QDebug>
#include <QNetworkInterface>

// json include
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "ubtlib.h"
// end

UpdateManager::UpdateManager(QObject *parent) : QObject(parent)
{
    m_pDonwloadManager = NULL;
}

UpdateManager::~UpdateManager ()
{
    if ( m_pDonwloadManager ){
        delete m_pDonwloadManager;
        m_pDonwloadManager = NULL;
    }
}

void UpdateManager::checkUpdate ()
{
    if ( requestVersion () ){
        m_pDonwloadManager->startDownLoad();
    }
}

QString UpdateManager::getServerVersion ()
{
    if ( m_pDonwloadManager == NULL ){
        if ( !requestVersion() ){
            return "";
        }
    }
    return m_pDonwloadManager->getServerParam()->getVersionName();
}

bool UpdateManager::requestVersion ()
{
    QString strMacAddr = getHostMacAddress();
    if ( strMacAddr.isEmpty() ){
        qDebug() << "can't find mac addr.";
        return false;
    }

    QNetworkRequest request;
    initRequest(request,
                HTTP_GET_NEWEST_UPGRADE_VERSION,
                APPID,
                "",
                strMacAddr);
    QNetworkReply *reply = m_pManager.get(request);
    int nState = waitForReply (reply);
    bool bRet = false;
    QString strError = "";
    if ( nState == EN_REQUEST_WAIT_RETURN )
    {
        do{
            // 服务器返回码
            if ( reply->error() != QNetworkReply::NoError ){
                strError = printNetReplyError(reply);
                break;
            }else{
                QString strRep = reply->readAll();
                QJsonParseError json_error;
                QJsonDocument jsonDoc(QJsonDocument::fromJson(strRep.toUtf8(), &json_error));
                if ( json_error.error != QJsonParseError::NoError ){
                    strError = QString("Reponse error code : %1").arg(json_error.error);
                    qDebug() << strError;
                    break;
                }

                QJsonArray arry = jsonDoc.array();
                if ( arry.count() != 1 ){
                    qDebug() << "confirm version online or have new version.";
                    bRet = true;
                    break;
                }

                if ( m_pDonwloadManager ){
                    delete m_pDonwloadManager;
                    m_pDonwloadManager = NULL;
                }

                m_pDonwloadManager = new DownloadManager;
                connect(m_pDonwloadManager, &DownloadManager::finished, this, &UpdateManager::finished);

                UpdateParam* param = m_pDonwloadManager->getServerParam();
                QJsonObject obj = arry.at(0).toObject();
                param->setModuleName(obj.value("moduleName").toString());
                param->setVersionName(obj.value("versionName").toString());
                param->setIncremental(obj.value("isIncremental").toBool());
                param->setPackageUrl(obj.value("packageUrl").toString());
                param->setPackageMd5(obj.value("packageMd5").toString());
                param->setPackageSize(obj.value("packageSize").toInt());

                if ( param->getIncremental() ){
                    param->setIncrementUrl(obj.value("incrementUrl").toString());
                    param->setIncrementMd5(obj.value("incrementMd5").toString());
                    param->setIncrementSize(obj.value("incrementSize").toInt());
                }

                param->setForced(obj.value("isForced").toBool());
                param->setReleaseNote(obj.value("releaseNote").toString());
                param->setReleaseTime(obj.value("releaseTime").toInt());
                m_pDonwloadManager->setServerParam(param);
                bRet = true;
            }
        }while(0);

        reply->abort();
        reply->deleteLater();
    }
    else
    {
        if ( nState == EN_REQUEST_WAIT_TIME_OUT ){
            strError =  tr("请求超时");
        }else{
            strError =  tr("网络错误");
        }
    }

    if ( !bRet ){
        emit finished();
        qDebug() << strError;
        return false;
    }

    return true;
}

void UpdateManager::initRequest (QNetworkRequest& request,
                                 const QString& strUrl,
                                 const QString& strAppId,
                                 const QString& strAppKey,
                                 const QString& strDeviceId)
{
    QString strUbtSign = QString::fromStdString(getHeaderXUBTSignV3(strDeviceId.toStdString()));

    request.setUrl(QUrl(strUrl));
    request.setRawHeader(QByteArray("X-UBT-Sign"), strUbtSign.toUtf8());
    request.setRawHeader(QByteArray("X-UBT-AppId"), strAppId.toUtf8());
    request.setRawHeader(QByteArray("X-UBT-DeviceId"), strDeviceId.toUtf8());
    request.setRawHeader(QByteArray("ContentType"), QByteArray("application/json"));

    qDebug() << "URL" << strUrl;
    qDebug() << "X-UBT-Sign" << strUbtSign;
    qDebug() << "X-UBT-AppId" << strAppId;
    qDebug() << "X-UBT-DeviceId" << strDeviceId;
}


QString UpdateManager::createVerifyCode (int nBits)
{
    QString strVerifyCode = "";
    QChar temp;
    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    for ( int i = 0; i < nBits; i++ )
    {
        switch ( qrand() % 3 )
        {
        case 0:
            temp= qrand() % 10 + '0';
            break;

        case 1:
            temp = qrand() % 26 + 'a';
            break;

        case 2:
            temp = qrand() % 26 + 'A';
            break;
        }
        strVerifyCode += temp;
    }

    return strVerifyCode;
}

int UpdateManager::waitForReply(QNetworkReply *reply)
{
    QTimer timer;
    timer.setInterval(REQUEST_TIME_OUT);
    timer.setSingleShot(true);

    /**
     * @brief 分三种情况退出事件循环(任意一个条件触发则退出)
     *        1、计时器超时（30秒）
     *        2、后台回复数据
     *        3、与后台通讯出错
     */
    QEventLoop loop;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    timer.start();
    loop.exec();  // 启动事件循环

    int nState = EN_REQUEST_WAIT_UNKOWN;
    if ( timer.isActive() )
    {
        if ( reply->error() != QNetworkReply::NoError ){
            reply->abort();
            reply->deleteLater();
            nState = EN_REQUEST_WAIT_NETERROR;
        }else{
            nState = EN_REQUEST_WAIT_RETURN;
        }
        timer.stop();
    }
    else
    {
        disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        reply->abort();
        reply->deleteLater();
        nState = EN_REQUEST_WAIT_TIME_OUT;
    }

    return nState;
}

QString UpdateManager::printNetReplyError(QNetworkReply* reply)
{
    QString strError = "";
    switch ( reply->error() ){
    case QNetworkReply::NoError:
        break;
    case QNetworkReply::ContentNotFoundError:{
        QUrl failedUrl = reply->request().url();
        int nStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString strReason = reply->attribute( QNetworkRequest::HttpReasonPhraseAttribute ).toString();
        strError = QString("url: %1, statusCode: %2, reason: %3").arg(failedUrl.toString()).arg(nStatusCode).arg(strReason);
    }
        break;
    default:{
        strError = QString ("Have error : %1, error: %2").arg(reply->errorString()).arg(reply->error());
    }
        break;
    }

    return strError;
}

QString UpdateManager::getHostMacAddress()
{
    QList<QNetworkInterface> listNet = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCount = listNet.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCount; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        QNetworkInterface interface = listNet.at(i);
        if( interface.flags().testFlag(QNetworkInterface::IsUp) &&
            interface.flags().testFlag(QNetworkInterface::IsRunning) &&
            !interface.flags().testFlag(QNetworkInterface::IsLoopBack) )
        {
            strMacAddr = interface.hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}
