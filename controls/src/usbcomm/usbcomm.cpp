#include "usbcomm.h"
#include "sharelog.h"
SHARE_EASYLOGGINGPP(getStorage())
#include "cfileinfo.h"

#include <QTextCodec>
#include <QFile>
#include <QEventLoop>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

qint64 g_nLastTimeMs = 0; /* 全局记录上次发送数据的时间戳 */

UsbComm::UsbComm(QObject *parent) : QObject(parent)
{
    int nFileInfoDelayTime = 1000;  // 这里倒数时间第一次不能少于600ms,后面每一包时间间隔不能少于250ms。
    for ( int i = 0; i < UBX_FILEINFO_RESEND_COUNT; i ++ ) {
        m_listFileInfoDelayTime.append(nFileInfoDelayTime);
        nFileInfoDelayTime -= UBX_FILEINFO_DELAY_TIME;
    }

    int nCmdDelayTime = 1000;
    for ( int i = 0; i < UBX_RESEND_COUNT; i ++ ) {
        m_listCmdDelayTime.append(nCmdDelayTime);
        nCmdDelayTime -= UBX_DELAY_TIME;
    }

    m_bSendCancel = false;
}

UsbComm::~UsbComm()
{
    m_listFileInfoDelayTime.clear();
    m_listCmdDelayTime.clear();
}

void UsbComm::slotSendFileInfo (CFileInfo* fileInfo)
{
    if ( sendCancle() ) {
        return; // 若后续还存在信号，则直接退出
    }

    // 第一包文件信息
    QByteArray data = combineFileInfoPacket (fileInfo);
    sendPacket (UBXPROTOCOL_CMDID_SEND_FILEINFO, data, true);

    //第二包开始文件内容
    int nRandNum = random(0, 255); // 随机数
    QFile file(fileInfo->getZipPath());
    if ( !file.open(QIODevice::ReadOnly) ){
        LOG(ERROR) << trs(QString("open file (%1) failed.").arg(fileInfo->getZipPath()));
        return ; // 打开文件失败
    }

    int nPacketSize = 128;
    bool bEndPacket = false;
    int nPacketCount = fileInfo->m_nFileSize % nPacketSize == 0 ?
                fileInfo->m_nFileSize / nPacketSize : fileInfo->m_nFileSize / nPacketSize + 1;
    for ( int i = 0; i < nPacketCount; i ++ ){
        if ( sendCancle() ){
            break; // 选择取消发送
        }

        QByteArray readData = file.read(nPacketSize);
        int nDataCount = readData.length();
        if ( nDataCount <= 0 ){
            break; // 没有读取数据或者读取出错，退出
        }

        if ( i == nPacketCount - 1 ) {
            bEndPacket = true; // 最后一包数据
        }

        QByteArray packetData = combineFileContentPacket(nRandNum, i, bEndPacket, readData);
        sendPacket (UBXPROTOCOL_CMDID_SEND_FILEPACK, packetData, true);

        emit sigSendBytes(fileInfo->getFilePath(), nDataCount);
    }
    emit sigSendOver(fileInfo->getFilePath(), !m_bSendCancel);
    file.close();
}

void UsbComm::slotSendFileInfo (QList<CFileInfo*> listFileInfo)
{
    int nCount = listFileInfo.count();
    qint64 nTatolSize = 0;
    for ( int i = 0; i < listFileInfo.count(); i ++ ){
        nTatolSize += listFileInfo.at(i)->m_nFileSize;
    }

    int received = 0;
    for ( int i = 0; i < nCount; i ++ ){
        CFileInfo* fileInfo = listFileInfo.at(i);
        if ( sendCancle() ) {
            return; // 若后续还存在信号，则直接退出
        }

        // 第一包文件信息
        QByteArray data = combineFileInfoPacket (fileInfo);
        sendPacket (UBXPROTOCOL_CMDID_SEND_FILEINFO, data, true);

        //第二包开始文件内容
        int nRandNum = random(0, 255); // 随机数
        QFile file(fileInfo->getZipPath());
        if ( !file.open(QIODevice::ReadOnly) ){
            LOG(ERROR) << trs(QString("open file (%1) failed.").arg(fileInfo->getZipPath()));
            break ; // 打开文件失败
        }

        int nPacketSize = 128;
        bool bEndPacket = false;
        int nPacketCount = fileInfo->m_nFileSize % nPacketSize == 0 ?
                    fileInfo->m_nFileSize / nPacketSize : fileInfo->m_nFileSize / nPacketSize + 1;
        for ( int i = 0; i < nPacketCount; i ++ ){
            if ( sendCancle() ){
                break; // 选择取消发送
            }

            QByteArray readData = file.read(nPacketSize);
            int nDataCount = readData.length();
            if ( nDataCount <= 0 ){
                break; // 没有读取数据或者读取出错，退出
            }

            if ( i == nPacketCount - 1 ) {
                bEndPacket = true; // 最后一包数据
            }

            QByteArray packetData = combineFileContentPacket(nRandNum, i, bEndPacket, readData);
            sendPacket (UBXPROTOCOL_CMDID_SEND_FILEPACK, packetData, true);
            received += nDataCount;
            emit downloadProgress(received, nTatolSize);
        }

        file.close();
    }

    emit finished();
}

bool UsbComm::openHidDev ()
{
    return UsbControl->open();
}

bool UsbComm::closeHidDev ()
{
    return UsbControl->close();
}

bool UsbComm::isOpen ()
{
    return UsbControl->isOpen();
}

void UsbComm::sendOpenFileServiceCmd (const QStringList &list, int nState)
{
    QByteArray data = combineOpenFileServerPacket (list, nState);
    sendPacket (UBXPROTOCOL_CMDID_SEND_FILESERVICE, data);
}

void UsbComm::sendActiveCmd (int nState)
{
    QByteArray data;
    data.append((byte)nState);
    sendPacket (UBXPROTOCOL_CMDID_ACTIVATE_PROGRAMME, data);
}

void UsbComm::sendCmd (int nCmdId, QString strBuf /*= ""*/)
{
    /**
     * @brief strBuf不为空时，报文格式
     *        1、(1b) 数据长度
     *        2、(nb) 数据
     *        strBuf为空时，报文格式
     *        1、(1b) 数据0
     */
    QByteArray data;
    if ( !strBuf.isEmpty() ){
        QByteArray arry = gb2312(strBuf);
        data.append((byte)arry.length());
        data.append(arry);
    }else{
        byte bDefault = 0;
        data.append(bDefault);
    }
    sendPacket (nCmdId, data);
}

void UsbComm::sendPlayCmd(const QString& strDanceName, int nMusicType)
{
    /**
     * @brief 报文格式
     *        1、(1b)数据长度
     *        2、(nb)数据
     *        3、(1b)是否从系统目录查询,0表示不是，1表示是
     */
    QByteArray arry = gb2312(strDanceName);
    QByteArray data;
    data.append((byte)arry.length());
    data.append(arry);
    data.append((byte)nMusicType);
    sendPacket (UBXPROTOCOL_CMDID_PLAY, data);
}

void UsbComm::sendPacket (int nCmdId, const QByteArray &data, bool isFileData /*= false*/)
{
    QList<ushort> listDelayTime;
    if ( isFileData ){
        listDelayTime = m_listFileInfoDelayTime;
    }else{
        listDelayTime = m_listCmdDelayTime;
    }

    int nTimes = listDelayTime.count();
    for (int i = 0; i < nTimes; i++) {
        int nDelayTime = 0;
        if ( i < nTimes - 1 ) {
            nDelayTime = listDelayTime.at(i) - listDelayTime.at(i + 1);
        } else {
            nDelayTime = listDelayTime.at(i);
        }
        QByteArray packetData = combinePacket(nCmdId, listDelayTime.at(i), data);
        sendData(packetData, nDelayTime, isFileData);
    }
}

bool UsbComm::sendData(const QByteArray& data,
                       ushort nDelayTime,
                       bool isFileData /*= false*/)
{
    qint64 nCurTime = QDateTime::currentDateTime().toMSecsSinceEpoch();   //将当前时间转为时间戳(ms)
    qint64 nSleepTime = 0;
    qint64 nTimeLeft = nCurTime - g_nLastTimeMs;
    if ( nTimeLeft < nDelayTime ) {
        nSleepTime = nDelayTime - nTimeLeft;
    }

    sleep(nSleepTime);
    sendData(data);

    g_nLastTimeMs = QDateTime::currentDateTime().toMSecsSinceEpoch();   //将当前时间转为时间戳(ms)
    /**  1字节从PC端发送至主服务需要 2.79 ms
     *   计算方法： 波特率为 9600, 1字节 = 8bit  每一字节需增加1bit stop位，一字节应为 9bit
     *             1/9600 * 9 * 1000 = 0.9375ms
     *   PC -> 发射器 -> 胸口版 -> 主服务 经过三次波特率转发，所以一字节需耗时 0.9375 * 3 = 2.8125 ms
     */
    if ( !isFileData ) // 命令方式考虑发送命令时间，减少同步误差
    {
        qint64 nSendTime = qRound(data.length() * 2.8125); // 四舍五入
        g_nLastTimeMs += nSendTime; // 兼容发送时间
    }
    return true;
}

bool UsbComm::sendData(const QByteArray& data)
{
    QByteArray dataTemp(data);
    byte bStart = 0x0;
    dataTemp.prepend(bStart);  // The first byte is the report number (0x0).
    printf (data);
    return UsbControl->write(dataTemp);
}

QByteArray UsbComm::combineOpenFileServerPacket (const QStringList& list, int nState)
{
    /**
     * @brief 报文内容,若不存在指定序列号,则默认全部开启文件服务,否则开启指定序列号的文件服务
     *        (1)、若指定序列号
     *           1、(1b) 集控状态
     *           2、(1b) 序列号数量,最大不超过255
     *           3、(nb) n * (序列号长度(1b) + 序列号内容(1b))
     *        (2)、若未指定序列号
     *           1、(1b) 集控状态
     */

    QByteArray data;
    data.append((byte)nState);
    int nCount = list.count();
    if ( nCount > 0 ){
        data.append((byte)nCount);
        for (int i = 0; i < nCount; i ++ ){
            QString strSN = list.at(i);
            QByteArray sn = gb2312(strSN);
            int nSerialSum = checkSum (sn);
            data.append((byte)sn.length());
            data.append((byte)nSerialSum);
        }
    }

    return data;
}

QByteArray UsbComm::combineFileInfoPacket (CFileInfo* fileInfo)
{
    /**
     * @brief 文件信息报文
     *        1、(1b)  文件名称长度(0-255)
     *        2、(nb)  文件名称
     *        3、(4b)  文件大小(4GM)
     *        4、(16b) MD5
     *        5、(1b)  类型
     */

    QByteArray arry = gb2312(fileInfo->getZipName());
    QByteArray data;
    data.append((byte)arry.length());
    data.append(arry);

    int nFileSize = fileInfo->m_nFileSize;
    data.append((byte)nFileSize);
    data.append((byte)(nFileSize >> 8));
    data.append((byte)(nFileSize >> 16));
    data.append((byte)(nFileSize >> 24));

    data.append(fileInfo->m_strMd5);
    data.append((byte)fileInfo->m_nRobotPathType);

    return data;
}

QByteArray UsbComm::combineFileContentPacket (int nRandNum, int nPacketIndex, byte bEndPacket, const QByteArray& dataContent)
{
    /**
     * @brief 组合文件内容报文
     *        1、(1b) 随机数
     *        2、(2b) 包序号
     *        3、(1b) 是否为结束包
     *        4、(1b) 文件内容包长度
     *        5、(nb) 文件内容包
     */
    QByteArray data;
    data.append((byte)nRandNum);
    data.append((byte)nPacketIndex);
    data.append((byte)(nPacketIndex >> 8));
    data.append((byte)bEndPacket);
    data.append((byte)dataContent.length());
    data.append(dataContent);
    return data;
}

QByteArray UsbComm::combinePacket (int nCmdId, ushort nDelayTime, const QByteArray& dataContent)
{
    /**
     * @brief 组合报文(报文头 + 报文内容 + 报文尾)
     *        1、(7b)报文头
     *        2、(nb)报文内容
     *        3、(2b)报文尾
     */
    int nPacketLen = sizeof(UBXPACKAGEFRONT) + dataContent.length() + sizeof(UBXPACKAGEBACK);
    byte bState = UBXPROTOCOL_CMDTYPE_SEND;

    QByteArray data;
    // 报文头
    data.append((byte)UBXPROTOCOL_HEAD_1);
    data.append((byte)UBXPROTOCOL_HEAD_2);
    nPacketLen -= 1;
    data.append((byte)(nPacketLen));  // 报文长度指0xED之前的所有的字节长度（不包括0xED）
    data.append((byte)nCmdId);
    data.append(bState);
    data.append((byte)nDelayTime);
    data.append((byte)(nDelayTime >> 8));

    // 报文内容
    if ( !dataContent.isEmpty() ) {
        data.append(dataContent);
    }

    // 报文尾
    QByteArray checkData = data.mid(2); // 校验值不计算头部（0x5A, 0xA5）、尾部（0xED）、校验位长度
    int nCheckSum = checkSum(checkData);
    data.append((byte)nCheckSum);
    data.append((byte)UBXPROTOCOL_TAIL);
    return data;
}

int UsbComm::checkSum(const QByteArray& data)
{
    int nSum = 0;
    for (int i = 0; i < data.length(); i++) {
        nSum += data.at(i);
    }
    return nSum;
}

QByteArray UsbComm::gb2312 (const QString& str)
{
    QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
    return pCodec->fromUnicode(str);
}

int UsbComm::random(int min, int max)
{
    Q_ASSERT(min < max);
    // 加入随机种子。种子是当前时间距离0点0分0秒的秒数。
    // 每次启动程序，只添加一次种子，以做到数字真正随机。
    static bool seedStatus;
    if (!seedStatus) {
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
        seedStatus = true;
    }
    int nRandom = qrand() % (max - min);
    nRandom = min + nRandom;

    return nRandom;
}

void UsbComm::sleep (uint msec)
{
    QEventLoop eventloop;
    QTimer::singleShot(msec, &eventloop, SLOT(quit()));
    eventloop.exec();
}

void UsbComm::printf (const QByteArray& data)
{
    QString str = "send hid dev message : ";
    for(int i = 0; i < data.length(); i++){
        QString strTemp;
        strTemp.sprintf("%02X ", (byte)data.at(i));
        str += strTemp;
    }
    //qDebug() << str;
    LOG(INFO) << trs(str);
}

