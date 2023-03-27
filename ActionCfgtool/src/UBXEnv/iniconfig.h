/*
* Copyright (c) 2016, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：IniConfig.h
* 创建时间：2016/11/15
* 文件标识：
* 文件摘要：IniConfig参数设置
*
* 当前版本：1.0.0.0
* 作    者：asu
* 完成时间：2016/11/15
* 版本摘要：
*/
#ifndef CINICONFIG_H
#define CINICONFIG_H

#include "ubxenv_global.h"
#include <QString>
#include <QVariant>

#include "singleton.h"
extern const QString SETTING_SPLIT_CHART;
const QString LOG_ENABLE_KEYWORD = "Log";

class QSettings;
class UBXENV_EXPORT CIniConfig : public utils::SingletonT<CIniConfig>
{
public:
    CIniConfig();
    virtual ~CIniConfig();
public:
    static bool setValue(const QString& strFile, const QString& strKey, const QVariant &varValue);
    static bool getValue(const QString& strFile, const QString& strKey, int &nValue);
    static bool getValue(const QString& strFile, const QString& strKey, double &dValue);
    static bool getValue(const QString& strFile, const QString& strKey, QString &strValue);
    static bool getValue(const QString& strFile, const QString& strKey, bool &bValue);

    static QString getKey(const QString& strKeyPre, const QString& strKeySuffix);
    static QString getKey(const QString& strKeyPre, const QString& strKeyMid, const QString& strKeySuffix);
public:
    //for config.ini file
    //for config.ini file node of [Config]
    bool getValueActPath(QString &strValue);
    void setValueActPath(const QString &strValue);
    bool getValueServer(QString &strValue);
    void setValueServer(const QString &strValue);

    bool getValueBigFrameDelay(int &nValue);
    void setValueBigFrameDelay(const int &nValue);
    bool getValueSmallFrameDelay(int &nValue);
    void setValueSmallFrameDelay(const int &nValue);

    //for config.ini file node of [UserInfo]
    //for mail login
    bool getValueMLUserName(QString &strValue);
    void setValueMLUserName(const QString &strValue);
    bool getValueMLPassword(QString &strValue);
    void setValueMLPassword(const QString &strValue);
    bool getValueMLUserType(int &nValue);
    void setValueMLUserType(const int &nValue);
    bool getValueMLIsSave(bool& bSave);
    void setValueMLIsSave(const bool& bSave);

    //for PhoneLogin login
    bool getValuePLCountryCode(QString &strValue);
    void setValuePLCountryCode(const QString &strValue);
    bool getValuePLUserName(QString &strValue);
    void setValuePLUserName(const QString &strValue);
    bool getValuePLPassword(QString &strValue);
    void setValuePLPassword(const QString &strValue);
    bool getValuePLUserType(int &nValue);
    void setValuePLUserType(const int &nValue);
    bool getValuePLIsSave(bool& bSave);
    void setValuePLIsSave(const bool& bSave);

    QString getVersion();
    QString getBaseUpgradeUrl();
    //for Email
    QString getValueSenderEmailAddress();
    QString getValueSenderEmailPassword();
    QStringList getValueRecipients();
    QString getValueSmtpServer();
    int getValueSmtpPort();
    bool getValueSslEnable();
public:
    //for log print
    bool getValueIsLogEnable(bool &bEnable);
private:
    static QVariant getValue(const QString& strFile, const QString& strKey);

private:
    //for config.ini file
    //node of [Config]
    static QString getKeyNodeConfig();
    //node of [UserInfo]
    static QString getKeyNodeUserInfo();

    //
    static QString getKeyActPath();
    static QString getKeyServer();
    static QString getKeyBigFrameDelay();
    static QString getKeySmallFrameDelay();

    //
    static QString getKeyML();
    static QString getKeyPL();

    //mail login key
    static QString getKeyMLUserName();
    static QString getKeyMLPassword();
    static QString getKeyMLUserType();
    static QString getKeyMLIsSave();

    //phone login key
    static QString getKeyPLCountryCode();
    static QString getKeyPLUserName();
    static QString getKeyPLPassword();
    static QString getKeyPLUserType();
    static QString getKeyPLIsSave();
private:
    QString m_strConfigIniFilePath;
    QSettings  *m_pSettings;
};

#endif // CINICONFIG_H
