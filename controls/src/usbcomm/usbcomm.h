#ifndef USBCOMM_H
#define USBCOMM_H

#include "usbcomm_global.h"
#include <QObject>
#include "protocoldefine.h"
#include "qusbcontrol.h"

QT_BEGIN_NAMESPACE
class CFileInfo;
QT_BEGIN_NAMESPACE

class USBCOMMSHARED_EXPORT UsbComm : public QObject
{
    Q_OBJECT
public:
    explicit UsbComm(QObject* parent = NULL);
    virtual~ UsbComm();

signals:
    // 文件传输
    void    sigSendBytes (QString strFilePath, int nSize);
    void    sigSendOver (QString strFilePath, bool bState);
    // 批量传输
    void    downloadProgress (qint64 received, qint64 total);
    void    finished ();

public slots:
    void    slotSendFileInfo (CFileInfo* fileInfo);
    void    slotSendFileInfo (QList<CFileInfo*> listFileInfo);

public:
    /**
     * @brief 打开hid dev
     * @return
     */
    bool    openHidDev ();

    /**
     * @brief 关闭hid dev
     * @return
     */
    bool    closeHidDev ();

    /**
     * @brief hid dev 是否打开
     * @return
     */
    bool    isOpen ();

    /**
     * @brief 激活指定序列号的文件服务命令
     * @param list 序列号列表
     * @param nState 0 表示关闭服务，1表示打开服务
     */
    void sendOpenFileServiceCmd (const QStringList& list, int nState);

    /**
     * @brief 发送激活集控命令
     * @param nState 0 表示退出集控 1表示激活集控
     */
    void sendActiveCmd (int nState);

    /**
     * @brief 发送命令
     * @param nCmdId 命令ID, 例如站立、蹲下、电量显示、关机等
     * @param strBuf 附带参数，目前仅动态播放使用
     */
    void sendCmd (int nCmdId, QString strBuf = "");

    /**
     * @brief 发送播放舞蹈命令
     * @param strDanceName 舞蹈名称
     * @param nMusicType   舞动类型  详情见 EM_MUSIC_TYPE
     */
    void sendPlayCmd (const QString& strDanceName, int nMusicType);

protected:
    /**
     * @brief 发送报文
     * @param nCmdId  命令ID
     * @param data    发送数据
     * @param isFileData true表示文件数据，false表示命令数据
     */
    void sendPacket (int nCmdId, const QByteArray& data, bool isFileData = false);

    /**
     * @brief 多次发送数据至Hid Dev，多次发送目的是保证数据同步
     * @param data 数据
     * @param nDelayTime 每一次发送后，延迟时间(ms)
     * @param isFileData true表示发送的数据时来源于文件内容，false表示发送的数据为命令
     * @return
     */
    bool    sendData(const QByteArray &data, ushort nDelayTime, bool isFileData = false);

    /**
     * @brief sendData 发送数据至Hid Dev
     * @param data  数据
     * @return
     */
    bool    sendData(const QByteArray &data);

    /**
     * @brief 组合打开/关闭文件服务报文
     * @param list 待集控的序列号列表
     * @param nState 集控状态
     * @return 文件服务报文
     */
    QByteArray combineOpenFileServerPacket (const QStringList &list, int nState);

    /**
     * @brief  文件信息报文
     * @param  fileInfo 文件信息
     * @return 文件信息报文
     */
    QByteArray combineFileInfoPacket (CFileInfo* fileInfo);

    /**
     * @brief 组合文件内容包报文
     * @param nRandNum  随机数
     * @param nPacketIndex 包序号
     * @param bEndPacket   是否为结束包
     * @param dataContent  文件包内容
     * @return  组合文件内容报文
     */
    QByteArray combineFileContentPacket (int nRandNum, int nPacketIndex, byte bEndPacket, const QByteArray& dataContent);

    /**
     * @brief 组合报文，添加报文头和报文尾
     * @param nCmdId  命令ID
     * @param nDelayTime 延迟时间
     * @param dataContent 报文内容
     * @return 组合之后的报文
     */
    QByteArray combinePacket (int nCmdId, ushort nDelayTime, const QByteArray& dataContent);

protected:
    /**
     * @brief 校验取和
     * @param data 字符串
     * @return 字符串相加和
     */
    int         checkSum(const QByteArray& data);
    QByteArray  gb2312 (const QString& str);
    int         random(int min, int max);
    void        sleep(uint msec);                          /**< 延迟 */
    void        printf (const QByteArray& data);

public:
    void setSendCancel (bool bCancel) { m_bSendCancel = bCancel; }
    bool sendCancle () const          { return m_bSendCancel; }

protected:
    QList<ushort>      m_listFileInfoDelayTime;
    QList<ushort>      m_listCmdDelayTime;
    bool               m_bSendCancel;
};

#endif // USBCOMM_H
