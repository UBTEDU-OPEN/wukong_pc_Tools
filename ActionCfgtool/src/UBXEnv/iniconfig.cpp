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
#include "stdafx.h"
#include "iniconfig.h"
#include <QSettings>
#include "configs.h"
#include "singleton.hpp"

IMPL_SINGLETON_CLASS(CIniConfig);

const QString SETTING_SPLIT_CHART = "/";
//Config Node
const QString CFG_NODE_NAME_CONFIG = "Config";  //动作表路径配置节名
const QString CFG_KEY_NAME_ACT_PATH = "ActPath";  //动作表路径配置键名
const QString CFG_KEY_NAME_SERVER = "Server";  //服务器地址配置键名

const QString CFG_KEY_NAME_BIG_FRAME_DELAY = "BigFrameDelay";
const QString CFG_KEY_NAME_SMALL_FRAME_DELAY = "SmallFrameDelay";


//UserInfo Node
const QString CFG_NODE_NAME_USER_INFO = "UserInfo";  //用户信息节名
const QString CFG_KEY_NAME_USER_LOGIN_TYPE_PHONE_LOGIN = "PhoneLogin"; // 手机登录方式
const QString CFG_KEY_NAME_USER_LOGIN_TYPE_MAIL_LOGIN = "MailLogin"; // 邮箱登录方式
const QString CFG_KEY_NAME_COUNTRY_CODE = "CountryCode";  // 国家码
const QString CFG_KEY_NAME_USER_NAME = "UserName";  //户名键名
const QString CFG_KEY_NAME_PASSWORD = "Password"; // 密码
const QString CFG_KEY_NAME_USER_TYPE = "UserType";  //用户类型键名
const QString CFG_KEY_NAME_IS_SAVE = "IsSave";    //是否保存键名

//软件版本号
const QString CFG_KEY_VERSION = "Version";
const QString CFG_KEY_UPGRADE_BASE_URL = "UpgradeBaseUrl";
//邮件配置列表
const QString CFG_KEY_NAME_EMAIL_ADDRESS = "SenderEmail";
const QString CFG_KEY_NAME_MAIL_AUTH = "EmailAuthorization";
const QString CFG_KEY_NAME_RECIPIENTS = "Recipients";
const QString CFG_KEY_NAME_MAIL_SERVER = "MailServer";
const QString CFG_KEY_NAME_PORT = "MailPort";
const QString CFG_KEY_NAME_SSL = "Ssl";

CIniConfig::CIniConfig()
{
    m_strConfigIniFilePath = CConfigs::getIniConfigFile();////动作表配置文件路径
    m_pSettings = new QSettings(m_strConfigIniFilePath, QSettings::IniFormat);
    m_pSettings->beginGroup(CFG_NODE_NAME_CONFIG);
}

CIniConfig::~CIniConfig()
{
    delete m_pSettings;
}

bool CIniConfig::setValue(const QString &strFile, const QString &strKey, const QVariant &varValue)
{
    QSettings settings(strFile, QSettings::IniFormat);
    settings.setValue(strKey, varValue);
    return true;
}

bool CIniConfig::getValue(const QString &strFile, const QString &strKey, int &nValue)
{
    QVariant var = CIniConfig::getValue(strFile, strKey);
    bool bOk = false;
    nValue = var.toInt(&bOk);
    return bOk;
}

bool CIniConfig::getValue(const QString &strFile, const QString &strKey, double &dValue)
{
    QVariant var = CIniConfig::getValue(strFile, strKey);

    bool bOk = false;
    dValue = var.toDouble(&bOk);
    return bOk;
}

bool CIniConfig::getValue(const QString &strFile, const QString &strKey, QString &strValue)
{
    QVariant var = CIniConfig::getValue(strFile, strKey);
    strValue = var.toString();

    return (!strValue.isEmpty());
}

bool CIniConfig::getValue(const QString &strFile, const QString &strKey, bool &bValue)
{
    QVariant var = CIniConfig::getValue(strFile, strKey);

    bValue = var.toBool();
    return bValue;
}

QString CIniConfig::getKey(const QString &strKeyPre, const QString &strKeySuffix)
{
    if(strKeyPre.endsWith(SETTING_SPLIT_CHART))
    {
        return strKeyPre + strKeySuffix;
    }

    return strKeyPre + SETTING_SPLIT_CHART + strKeySuffix;
}

QString CIniConfig::getKey(const QString &strKeyPre, const QString &strKeyMid, const QString &strKeySuffix)
{
    QString strTemp = CIniConfig::getKey(strKeyPre, strKeyMid);
    return CIniConfig::getKey(strTemp, strKeySuffix);
}

QVariant CIniConfig::getValue(const QString &strFile, const QString &strKey)
{
    QSettings settings(strFile, QSettings::IniFormat);
    return settings.value(strKey);
}


QString CIniConfig::getKeyNodeConfig()
{
    return CFG_NODE_NAME_CONFIG;
}

QString CIniConfig::getKeyNodeUserInfo()
{
    return CFG_NODE_NAME_USER_INFO;
}

QString CIniConfig::getKeyActPath()
{
    return CIniConfig::getKey(CIniConfig::getKeyNodeConfig(),
                                        CFG_KEY_NAME_ACT_PATH);
}

QString CIniConfig::getKeyServer()
{
    return CIniConfig::getKey(CIniConfig::getKeyNodeConfig(),
                                        CFG_KEY_NAME_SERVER);
}

QString CIniConfig::getKeyBigFrameDelay()
{
    return CIniConfig::getKey(CIniConfig::getKeyNodeConfig(),
                                        CFG_KEY_NAME_BIG_FRAME_DELAY);
}

QString CIniConfig::getKeySmallFrameDelay()
{
    return CIniConfig::getKey(CIniConfig::getKeyNodeConfig(),
                                        CFG_KEY_NAME_SMALL_FRAME_DELAY);
}

QString CIniConfig::getKeyML()
{
    return CIniConfig::getKey(CIniConfig::getKeyNodeUserInfo(),
                              CFG_KEY_NAME_USER_LOGIN_TYPE_MAIL_LOGIN);
}

QString CIniConfig::getKeyPL()
{
    return CIniConfig::getKey(CIniConfig::getKeyNodeUserInfo(),
                              CFG_KEY_NAME_USER_LOGIN_TYPE_PHONE_LOGIN);
}

QString CIniConfig::getKeyMLUserName()
{
    return CIniConfig::getKey(CIniConfig::getKeyML(),
                              CFG_KEY_NAME_USER_NAME);
}

QString CIniConfig::getKeyMLPassword()
{
    return CIniConfig::getKey(CIniConfig::getKeyML(),
                              CFG_KEY_NAME_PASSWORD);
}

QString CIniConfig::getKeyMLUserType()
{
    return CIniConfig::getKey(CIniConfig::getKeyML(),
                              CFG_KEY_NAME_USER_TYPE);
}

QString CIniConfig::getKeyMLIsSave()
{
    return CIniConfig::getKey(CIniConfig::getKeyML(),
                              CFG_KEY_NAME_IS_SAVE);
}

QString CIniConfig::getKeyPLCountryCode()
{
    return CIniConfig::getKey(CIniConfig::getKeyPL(),
                              CFG_KEY_NAME_COUNTRY_CODE);
}

QString CIniConfig::getKeyPLUserName()
{
    return CIniConfig::getKey(CIniConfig::getKeyPL(),
                              CFG_KEY_NAME_USER_NAME);
}

QString CIniConfig::getKeyPLPassword()
{
    return CIniConfig::getKey(CIniConfig::getKeyPL(),
                              CFG_KEY_NAME_PASSWORD);
}

QString CIniConfig::getKeyPLUserType()
{
    return CIniConfig::getKey(CIniConfig::getKeyPL(),
                              CFG_KEY_NAME_USER_TYPE);
}

QString CIniConfig::getKeyPLIsSave()
{
    return CIniConfig::getKey(CIniConfig::getKeyPL(),
                              CFG_KEY_NAME_IS_SAVE);
}

bool CIniConfig::getValueActPath(QString &strValue)
{
    QString strKey = CIniConfig::getKeyActPath();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, strValue);
}

void CIniConfig::setValueActPath(const QString &strValue)
{
    QString strKey = CIniConfig::getKeyActPath();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, strValue);
}

bool CIniConfig::getValueServer(QString &strValue)
{
    QString strKey = CIniConfig::getKeyServer();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, strValue);
}

void CIniConfig::setValueServer(const QString &strValue)
{
    QString strKey = CIniConfig::getKeyServer();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, strValue);
}

bool CIniConfig::getValueBigFrameDelay(int &nValue)
{
    QString strKey = CIniConfig::getKeyBigFrameDelay();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, nValue);
}

void CIniConfig::setValueBigFrameDelay(const int &nValue)
{
    QString strKey = CIniConfig::getKeyBigFrameDelay();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, nValue);
}

bool CIniConfig::getValueSmallFrameDelay(int &nValue)
{
    QString strKey = CIniConfig::getKeySmallFrameDelay();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, nValue);
}

void CIniConfig::setValueSmallFrameDelay(const int &nValue)
{
    QString strKey = CIniConfig::getKeySmallFrameDelay();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, nValue);
}

bool CIniConfig::getValueMLUserName(QString &strValue)
{
    const QString strKey = CIniConfig::getKeyMLUserName();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, strValue);
}

void CIniConfig::setValueMLUserName(const QString &strValue)
{
    const QString strKey = CIniConfig::getKeyMLUserName();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, strValue);
}

bool CIniConfig::getValueMLPassword(QString &strValue)
{
    QString strKey = CIniConfig::getKeyMLPassword();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, strValue);
}

void CIniConfig::setValueMLPassword(const QString &strValue)
{
    const QString strKey = CIniConfig::getKeyMLPassword();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, strValue);
}

bool CIniConfig::getValueMLUserType(int &nValue)
{
    QString strKey = CIniConfig::getKeyMLUserType();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, nValue);
}

void CIniConfig::setValueMLUserType(const int &nValue)
{
    const QString strKey = CIniConfig::getKeyMLUserType();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, nValue);
}

bool CIniConfig::getValueMLIsSave(bool &bSave)
{
    QString strKey = CIniConfig::getKeyMLIsSave();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, bSave);
}

void CIniConfig::setValueMLIsSave(const bool &bSave)
{
    const QString strKey = CIniConfig::getKeyMLIsSave();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, bSave);
}

bool CIniConfig::getValuePLCountryCode(QString &strValue)
{
    QString strKey = CIniConfig::getKeyPLCountryCode();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, strValue);
}

void CIniConfig::setValuePLCountryCode(const QString &strValue)
{
    QString strKey = CIniConfig::getKeyPLCountryCode();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, strValue);
}

bool CIniConfig::getValuePLUserName(QString &strValue)
{
    QString strKey = CIniConfig::getKeyPLUserName();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, strValue);
}

void CIniConfig::setValuePLUserName(const QString &strValue)
{
    const QString strKey = CIniConfig::getKeyPLUserName();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, strValue);
}

bool CIniConfig::getValuePLPassword(QString &strValue)
{
    QString strKey = CIniConfig::getKeyPLPassword();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, strValue);
}

void CIniConfig::setValuePLPassword(const QString &strValue)
{
    const QString strKey = CIniConfig::getKeyPLPassword();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, strValue);
}

bool CIniConfig::getValuePLUserType(int &nValue)
{
    QString strKey = CIniConfig::getKeyPLUserType();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, nValue);
}

void CIniConfig::setValuePLUserType(const int &nValue)
{
    const QString strKey = CIniConfig::getKeyPLUserType();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, nValue);
}

bool CIniConfig::getValuePLIsSave(bool &bSave)
{
    QString strKey = CIniConfig::getKeyPLIsSave();
    return CIniConfig::getValue(m_strConfigIniFilePath, strKey, bSave);
}

void CIniConfig::setValuePLIsSave(const bool &bSave)
{
    const QString strKey = CIniConfig::getKeyPLIsSave();
    CIniConfig::setValue(m_strConfigIniFilePath, strKey, bSave);
}

/**************************************************************************
* 函数名:   getVersion
* 功能:
* 参数:
*....@[out] QString
* 返回值:
* 时间:     2017/11/02 19:45
* 作者:   Leo
*/
QString CIniConfig::getVersion()
{
    QString qsValue = m_pSettings->value(CFG_KEY_VERSION).toString();
    return qsValue;
}

/**************************************************************************
* 函数名:   getBaseUpgradeUrl
* 功能:
* 参数:
*....@[out] QString
* 返回值:
* 时间:     2017/11/03 9:51
* 作者:   Leo
*/
QString CIniConfig::getBaseUpgradeUrl()
{
    QString qsValue = m_pSettings->value(CFG_KEY_UPGRADE_BASE_URL).toString();
    return qsValue;
}

QString CIniConfig::getValueSenderEmailAddress()
{
    QString qsValue = m_pSettings->value(CFG_KEY_NAME_EMAIL_ADDRESS).toString();
    return qsValue;
}

QString CIniConfig::getValueSenderEmailPassword()
{
    QString qsValue = m_pSettings->value(CFG_KEY_NAME_MAIL_AUTH).toString();
    return qsValue;
}

QStringList CIniConfig::getValueRecipients()
{
    QString qsAll = m_pSettings->value(CFG_KEY_NAME_RECIPIENTS).toString();
    QStringList qslist = qsAll.split(";", QString::SkipEmptyParts);
    return qslist;
}

QString CIniConfig::getValueSmtpServer()
{
    QString qsValue = m_pSettings->value(CFG_KEY_NAME_MAIL_SERVER).toString();
    return qsValue;
}

int CIniConfig::getValueSmtpPort()
{
    int value = m_pSettings->value(CFG_KEY_NAME_PORT).toInt();
    return value;
}


bool CIniConfig::getValueSslEnable()
{
    return m_pSettings->value(CFG_KEY_NAME_SSL).toBool();
}

bool CIniConfig::getValueIsLogEnable(bool &bEnable)
{
    QString strKey = CIniConfig::getKeyNodeConfig();
    strKey = CIniConfig::getKey(strKey, LOG_ENABLE_KEYWORD);
    CIniConfig::getValue(m_strConfigIniFilePath, strKey, bEnable);
    return bEnable;
}
