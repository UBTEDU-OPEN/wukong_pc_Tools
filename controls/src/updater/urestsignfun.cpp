#include "urestsignfun.h"
#include <QTimer>
#include <QNetworkRequest>
#include <QDateTime>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QUrl>
#include "ubtlib.h"

void URestSignFun::addSignRequest (QNetworkRequest *request,
                            const QString& strUrl,
                            const QString& strAppId,
                            const QString& strAppKey,
                            const QString& strSnNum )
{
    QString strUbtSign = QString::fromStdString(getUpdateHeaderXUBTSignV3(strSnNum.toStdString()));
    request->setUrl(QUrl(strUrl));
    request->setRawHeader(QByteArray("Content-Type"), QByteArray("application/json"));
    request->setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    request->setRawHeader(QByteArray("X-UBT-AppId"), strAppId.toUtf8());
    request->setRawHeader(QByteArray("X-UBT-Sign"), strUbtSign.toUtf8());
    request->setRawHeader(QByteArray("X-UBT-DeviceId"), strSnNum.toUtf8());
    request->setRawHeader(QByteArray("Access-Version"), QByteArray("v2"));
    request->setRawHeader(QByteArray("common-info"), QByteArray("{}"));
    request->setRawHeader(QByteArray("device-info"), QByteArray("{}"));
}


QString URestSignFun::createVerifyCode (int nBits)
{
    QString strVerifyCode = "";
    QChar temp;
    qsrand(QTime(0,0,0).msecsTo(QTime::currentTime()));
    for ( int i = 0; i < nBits; i++ ){
        switch ( qrand() % 3 ){
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

int URestSignFun::waitForReply(QNetworkReply *reply)
{
    QTimer timer;
    timer.setInterval(REQUEST_URL_TIME_OUT);
    timer.setSingleShot(true);

    /**
     * @brief 分三种情况退出事件循环(任意一个条件触发则退出)
     *        1、计时器超时（30秒）
     *        2、后台回复数据
     *        3、与后台通讯出错
     */
    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), &loop, SLOT(quit()));
    timer.start();
    loop.exec();  // 启动事件循环

    int nState = EN_REQUEST_WAIT_UNKOWN;
    if ( timer.isActive() ) {
        timer.stop();
        if ( reply->error() != QNetworkReply::NoError ){
            nState = EN_REQUEST_WAIT_NETERROR;
        }else{
            nState = EN_REQUEST_WAIT_RETURN;
        }
    } else {
        QObject::disconnect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        nState = EN_REQUEST_WAIT_TIME_OUT;
    }

    return nState;
}
