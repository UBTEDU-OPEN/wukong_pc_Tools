#ifndef CPUBFUN_H
#define CPUBFUN_H

#include "pubdef.h"

QT_BEGIN_NAMESPACE
class QNetworkRequest;
class QNetworkReply;
QT_END_NAMESPACE

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

#define REQUEST_URL_TIME_OUT    (30 * 1000)      /**< 请求超时 */
#define APPID                   "20PC20000"

#ifdef QT_DEBUG
#define HTTP_GET_ACTIONLIST_URL     "https://test79.ubtrobot.com/v1/alpha-centre/mini/action/list"
#define HTTP_POST_GETACTIONLIST_URL "https://test79.ubtrobot.com/v1/alpha-centre/mini/action/getFile"
#else
#define HTTP_GET_ACTIONLIST_URL     "https://cloudapi.ubtrobot.com/v1/alpha-centre/mini/action/list"
#define HTTP_POST_GETACTIONLIST_URL "https://cloudapi.ubtrobot.com/v1/alpha-centre/mini/action/getFile"
#endif

class CPubFun
{
public:
    static QString  getMusicTypeName (int nMusicType);
    static bool     loadPlayList (const QString& strPath, QTypeMusicInfoMap& musicInfoMap);
    static bool     savePlayList (const QString& strPath, const QTypeMusicInfoMap& musicInfoMap);

    static void     addSignRequest (QNetworkRequest* request,
                                    const QString& strUrl,
                                    const QString& strAppId,
                                    const QString& strAppKey,
                                    const QString& strSnNum);                           /**< 初始化HTTP表头 */
    static QString  getURestSign (const QString& strSNNum, const QString& strAppKey);   /**< 获取鉴权 */
    static QString  createVerifyCode (int nBits);                                       /**< 随机码 */
    static int      waitForReply(QNetworkReply *reply);                                 /**< 等待回复 */
};

#endif // CPUBFUN_H
