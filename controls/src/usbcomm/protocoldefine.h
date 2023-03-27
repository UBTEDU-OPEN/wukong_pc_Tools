#ifndef PROTOCOLDEFINE_H
#define PROTOCOLDEFINE_H
#include <QtCore/qglobal.h>

typedef uchar byte;

#define trs(Var)                                QString(Var).toStdString()

#define UBX_FILEINFO_RESEND_COUNT               2       /**< 文件信息及內容等包重复发送2次 */
#define UBX_FILEINFO_DELAY_TIME                 500     /**< 每一帧文件数据延迟(ms) */
#define UBX_RESEND_COUNT                        5       /**< 命令参数重复发送次数 */
#define UBX_DELAY_TIME                          200     /**< 每次发送命令延迟(ms) */

/**< 发送命令 */
#define UBXPROTOCOL_CMDTYPE_SEND                0x00   /**< 命令类型，发送 */
#define UBXPROTOCOL_CMDID_PLAY                  0x01   /**< 开始表演 */
#define UBXPROTOCOL_CMDID_STOP                  0x02   /**< 结束表演(复位) */
#define UBXPROTOCOL_CMDID_PLAY_TEST             0x03   /**< 播放测试动作 */
#define UBXPROTOCOL_CMDID_PAUSE                 0x04   /**< 暂停播放 */
#define UBXPROTOCOL_CMDID_RESUME                0x05   /**< 继续表演 */
#define UBXPROTOCOL_CMDID_POWEROFF              0x06   /**< 本体关机 */
#define UBXPROTOCOL_CMDID_REST                  0x07   /**< 本体休眠 */
#define UBXPROTOCOL_CMDID_WAKEUP                0x08   /**< 本体唤醒，复位 */
#define UBXPROTOCOL_CMDID_BEGIN_SEND_FILE       0x09   /**< 开始传文件 */
#define UBXPROTOCOL_CMDID_SENDING_FILE          0x0A   /**< 传输文件 */
#define UBXPROTOCOL_CMDID_END_SEND_FILE         0x0B   /**< 结束传文件 */
#define UBXPROTOCOL_CMDID_DELETE_FILE           0x0C   /**< 删除文件 */
#define UBXPROTOCOL_CMDID_SET_FREQUENCY_REC     0x0D   /**< 设置接收频点 */
#define UBXPROTOCOL_CMDID_SET_FREQUENCY_SEND    0x0E   /**< 设置发射频点 */
#define UBXPROTOCOL_CMDID_SHOW_BATTERY          0x0F   /**< 显示电量 */
#define UBXPROTOCOL_CMDID_ACTIVATE_PROGRAMME    0x10   /**< 激活主服务程序 */
#define UBXPROTOCOL_CMDID_DYNAMIC_PLAYER        0x11   /**< 动态播放 */
// 集控传输文件命令类型
#define UBXPROTOCOL_CMDID_SEND_FILESERVICE      0x51   /**< 文件服务 */
#define UBXPROTOCOL_CMDID_SEND_FILEINFO         0x52   /**< 文件信息 */
#define UBXPROTOCOL_CMDID_SEND_FILEPACK         0x53   /**< 文件数据包 */

/**< 释放空间 */
#define SAFE_DELETE(p) {if(p){ delete p; p = NULL;}}
#define SAFE_DELETE_ARRAY(p) {if(p){ delete [] p; p = NULL;}}

/**< 报文定义 */

#define UBXPROTOCOL_HEAD_1                0xA5          /**< 协议头部 */
#define UBXPROTOCOL_HEAD_2                0x5A          /**< 协议头部 */
#define UBXPROTOCOL_TAIL                  0xED          /**< 协议尾部 */

#pragma pack(push, 1)
/**< 数据包前缀结构 */
typedef struct UBXPACKAGEFRONT {
    byte   head[2];         /**< 头部 */
    byte   nPkgLen;         /**< 包长度，即头尾数据总长度, 最大表示255 */
    byte   nCmdId;          /**< 命令ID */
    byte   nState;          /**< 应答状态 */
    ushort nDelayTime;      /**< 倒数时间 */
}UBXPACKAGEFRONT;

/**< 数据包后缀结构 */
typedef struct UBXPACKAGEBACK
{
    byte   nChkSum;  /**< 校验值 */
    byte   tail;     /**< 尾部 */
}UBXPACKAGEBACK;
#pragma pack(pop)

#endif // PROTOCOLDEFINE_H
