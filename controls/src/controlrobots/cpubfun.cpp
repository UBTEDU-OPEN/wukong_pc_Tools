#include "cpubfun.h"
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QTimer>
#include <QNetworkRequest>
#include <QDateTime>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QUrl>
#include "ubtlib.h"

QString CPubFun::getMusicTypeName (int nMusicType)
{
    QString strMusicType = "";
    switch (nMusicType) {
    case EN_SYSTEM_MUSIC_TYPE:
        strMusicType = QObject::tr("系统舞蹈");
        break;
    case EN_CUSTOM_MUSIC_TYPE:
        strMusicType = QObject::tr("自定义舞蹈");
        break;
    case EN_UPDATE_MUSIC_TYPE:
        strMusicType = QObject::tr("后台更新舞蹈");
        break;
    default:
        break;
    }
    return strMusicType;
}

bool CPubFun::loadPlayList (const QString& strPath, QTypeMusicInfoMap& musicInfoMap)
{
    QFile file(strPath);
    if ( !file.exists() )
        return false;

    if( !file.open(QIODevice::ReadOnly) )
        return false;

    QByteArray allData = file.readAll();
    file.close();

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
    if( json_error.error != QJsonParseError::NoError )
        return false;

    QJsonObject root = jsonDoc.object();
    if( !root.contains("list") )
        return false;

    qDeleteAll(musicInfoMap);
    musicInfoMap.clear();

    QJsonObject object = root.value("list").toObject();
    QMusicInfoMap* pMusicInfoMap = NULL;
    if ( object.contains("system") ) {
        QTypeMusicInfoMap::iterator it = musicInfoMap.find(EN_SYSTEM_MUSIC_TYPE);
        if ( it != musicInfoMap.end() ){
            pMusicInfoMap = it.value();
        } else {
            pMusicInfoMap = new QMusicInfoMap;
            musicInfoMap.insert(EN_SYSTEM_MUSIC_TYPE, pMusicInfoMap);
        }

        QJsonArray systemArr = object.value("system").toArray();
        for(int i = 0; i < systemArr.size(); i++) {
            QJsonObject obj = systemArr.at(i).toObject();
            if( obj.contains("fileName") && obj.contains("displayName") && obj.contains("time") ) {
                QString strFileName = obj.value("fileName").toString();
                QString strDisplayName = obj.value("displayName").toString();
                QString strMusicTime = obj.value("time").toString();
                tagMusicInfo* pTagMusicInfo = new tagMusicInfo;
                pTagMusicInfo->m_strDisplayName = strDisplayName;
                pTagMusicInfo->m_nMusicTime = strMusicTime.toInt();
                pMusicInfoMap->insert(strFileName, pTagMusicInfo);
            }
        }
    }

    if ( object.contains("custom") ) {
        QTypeMusicInfoMap::iterator it = musicInfoMap.find(EN_CUSTOM_MUSIC_TYPE);
        if ( it != musicInfoMap.end() ){
            pMusicInfoMap = it.value();
        } else {
            pMusicInfoMap = new QMusicInfoMap;
            musicInfoMap.insert(EN_CUSTOM_MUSIC_TYPE, pMusicInfoMap);
        }

        QJsonArray systemArr = object.value("custom").toArray();
        for(int i = 0; i < systemArr.size(); i++) {
            QJsonObject obj = systemArr.at(i).toObject();
            if( obj.contains("fileName") && obj.contains("displayName") && obj.contains("time") ) {
                QString strFileName = obj.value("fileName").toString();
                QString strDisplayName = obj.value("displayName").toString();
                QString strMusicTime = obj.value("time").toString();
                tagMusicInfo* pTagMusicInfo = new tagMusicInfo;
                pTagMusicInfo->m_strDisplayName = strDisplayName;
                pTagMusicInfo->m_nMusicTime = strMusicTime.toInt();
                pMusicInfoMap->insert(strFileName, pTagMusicInfo);
            }
        }
    }

    if ( object.contains("update") ) {
        QTypeMusicInfoMap::iterator it = musicInfoMap.find(EN_UPDATE_MUSIC_TYPE);
        if ( it != musicInfoMap.end() ){
            pMusicInfoMap = it.value();
        } else {
            pMusicInfoMap = new QMusicInfoMap;
            musicInfoMap.insert(EN_UPDATE_MUSIC_TYPE, pMusicInfoMap);
        }

        QJsonArray systemArr = object.value("update").toArray();
        for(int i = 0; i < systemArr.size(); i++) {
            QJsonObject obj = systemArr.at(i).toObject();
            if( obj.contains("fileName") && obj.contains("displayName") && obj.contains("time") ) {
                QString strFileName = obj.value("fileName").toString();
                QString strDisplayName = obj.value("displayName").toString();
                QString strMusicTime = obj.value("time").toString();
                tagMusicInfo* pTagMusicInfo = new tagMusicInfo;
                pTagMusicInfo->m_strDisplayName = strDisplayName;
                pTagMusicInfo->m_nMusicTime = strMusicTime.toInt();
                pMusicInfoMap->insert(strFileName, pTagMusicInfo);
            }
        }
    }

    return true;
}

bool CPubFun::savePlayList (const QString& strPath, const QTypeMusicInfoMap& musicInfoMap)
{
    QJsonObject childArry;
    QTypeMusicInfoMap::const_iterator it = musicInfoMap.begin();
    for ( ; it != musicInfoMap.end(); it ++ )
    {
        int nType = it.key();
        QJsonArray array;
        QMusicInfoMap* p = it.value();
        QMusicInfoMap::iterator itSub = p->begin();
        for ( ; itSub != p->end(); itSub ++ ) {
            QString strFileName = itSub.key();
            tagMusicInfo* pT = itSub.value();
            QString strDisplayName = pT->m_strDisplayName;
            int nMusicTime = pT->m_nMusicTime;

            QJsonObject object;
            object.insert("fileName", strFileName);
            object.insert("displayName", strDisplayName);
            object.insert("time", QString::number(nMusicTime));
            array.append(object);
        }

        if ( array.count() > 0 ){
            switch (nType){
            case EN_SYSTEM_MUSIC_TYPE:
                childArry.insert("system", array);
                break;
            case EN_CUSTOM_MUSIC_TYPE:
                childArry.insert("custom", array);
                break;
            case EN_UPDATE_MUSIC_TYPE:
                childArry.insert("update", array);
                break;
            default:
                break;
            }
        }
    }

    QJsonDocument jsonDoc;
    QJsonObject object;
    object.insert("list", childArry);
    jsonDoc.setObject(object);
    QByteArray bytes = jsonDoc.toJson(QJsonDocument::Indented);

    QFile file(strPath);
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Truncate) )
        return false;

    file.write(bytes);
    file.close();
    return true;
}

void CPubFun::addSignRequest (QNetworkRequest *request,
                            const QString& strUrl,
                            const QString& strAppId,
                            const QString& strAppKey,
                            const QString& strSnNum )
{
    QString strUbtSign = QString::fromStdString(getHeaderXUBTSignV3(strSnNum.toStdString()));

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

QString CPubFun::createVerifyCode (int nBits)
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

int CPubFun::waitForReply(QNetworkReply *reply)
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
