#ifndef URESTSIGNFUN_H
#define URESTSIGNFUN_H

#include <QString>
#include "uhttpdefine.h"

QT_BEGIN_NAMESPACE
class QNetworkRequest;
class QNetworkReply;
QT_END_NAMESPACE

class URestSignFun
{
public:
    static void     addSignRequest (QNetworkRequest* request,
                                    const QString& strUrl,
                                    const QString& strAppId,
                                    const QString& strAppKey,
                                    const QString& strSnNum);                           /**< 初始化HTTP表头 */
    static QString  createVerifyCode (int nBits);                                       /**< 随机码 */
    static int      waitForReply(QNetworkReply *reply);                                 /**< 等待回复 */
};

#endif // URESTSIGNFUN_H
