#ifndef UHTTPDEFINE_H
#define UHTTPDEFINE_H

#include <QString>
#if _MSC_VER >= 1600
    #pragma execution_character_set("utf-8")
#endif

#define REQUEST_URL_TIME_OUT    (30 * 1000)      /**< 请求超时 */
#define UPDATER_APPID           "100100016"

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

#endif // UHTTPDEFINE_H
