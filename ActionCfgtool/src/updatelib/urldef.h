#ifndef URLDEF_H
#define URLDEF_H

#include <QString>
#if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")
#endif

#ifdef QT_DEBUG
    #define _TEST_ENV  // 定义测试环境
#endif
//#define _TEST_ENV  // 定义测试环境

#define PRODUCT_NAME                "AlphaMiniPC"           // 项目名称

#ifdef Q_OS_WIN
#ifndef Q_OS_WIN64
    #define MODULE_NAME                 "CentraliedControl-x86"     // 模块名称
#else
    #define MODULE_NAME                 "CentraliedControl-x64"     // 模块名称
#endif
#endif

#ifdef QT_DEBUG
    #define PROGRAM_NAME            "CentraliedControld.exe" // 执行程序名称
#else
    #define PROGRAM_NAME            "CentraliedControl.exe" // 执行程序名称
#endif

#define PROGRAM_VERSION             "v2.0.0"                // 当前程序版本
#define APPID                       "100100016"
#define REQUEST_TIME_OUT            (30 * 1000)
#define _FILE_LINE_                 QString("FILE: %1, LINE: %2").arg(__FILE__).arg(__LINE__)
#define trs(Var)                    QString(Var).toStdString()

#ifdef _TEST_ENV
    #define HTTP_GET_NEWEST_UPGRADE_VERSION  \
        QString("https://test79.ubtrobot.com/v1/upgrade-rest/version/upgradable?productName=%1&moduleNames=%2&versionNames=%3") \
        .arg(PRODUCT_NAME).arg(MODULE_NAME).arg(PROGRAM_VERSION)
#else
    #define HTTP_GET_NEWEST_UPGRADE_VERSION  \
        QString("https://apis.ubtrobot.com/v1/upgrade-rest/version/upgradable?productName=%1&moduleNames=%2&versionNames=%3") \
    .arg(PRODUCT_NAME).arg(MODULE_NAME).arg(PROGRAM_VERSION)
#endif

/**
 * @brief 请求状态
 */
enum EN_REQUEST_WAIT_STATE
{
    EN_REQUEST_WAIT_UNKOWN      = 0,     /**< 未知错误 */
    EN_REQUEST_WAIT_TIME_OUT    = 1,     /**< 请求超时 */
    EN_REQUEST_WAIT_RETURN      = 2,     /**< 请求返回 */
    EN_REQUEST_WAIT_NETERROR    = 3      /**< 网络错误 */
};

/**
 * @brief 语言类型
 */
enum EN_LANGUAGE_TYPE
{
    EN_LANGUAGE_EN_US = 0,              /**< 英文    */
    EN_LANGUAGE_ZH_CN = 1,              /**< 中文简体 */
};

#endif // URLDEF_H
