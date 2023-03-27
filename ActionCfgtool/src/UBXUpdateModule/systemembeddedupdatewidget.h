/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：SystemEmbeddedUpdateWidget.h
* 创建时间：2016/07/23 16:17
* 文件标识：
* 文件摘要：更新嵌入式版本
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/07/23 16:17
* 版本摘要：
*/

#ifndef SYSTEMEMBEDDEDUPDATEWIDGET_H
#define SYSTEMEMBEDDEDUPDATEWIDGET_H

#include <QWidget>
#include <QProgressDialog>
#include <QMouseEvent>
#include <QProcess>
#include "UBXRobotController.h"
#include "global.h"
#include "ubxupdatemodule_global.h"
#include "cthreadcopyfile.h"
#include "cthreadwaitforudiskshowup.h"

class CProgressDialog;
#ifdef Q_OS_MAC
#include "myusbdevice.h"
#endif

namespace Ui {
class SystemEmbeddedUpdateWidget;
}


class UBXUPDATEMODULESHARED_EXPORT SystemEmbeddedUpdateWidget : public QDialog
{
    Q_OBJECT

public:
    explicit SystemEmbeddedUpdateWidget(QWidget *parent = 0);
    ~SystemEmbeddedUpdateWidget();

    /**************************************************************************
    * 函数名: doCheckUpdate
    * 功能: 检查嵌入式版本更新
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    bool doCheckUpdate(bool bCanShowMBox);

    // 判断是是否是需要强制拷贝升级的版本
    bool isForceCopyUpateVersion();

    /**************************************************************************
    * 函数名: getVersionInNumber
    * 功能: 获取仅含有数字的版本号（如果含有V开头，则去掉V）
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    static QString getVersionInNumber(const QString &sVersion);

public slots:
    // 切换到U盘模式结果
    void slotOnChangedToUDiskResult(bool bOk);

    // 切换到U盘模式后，等待U盘出现的结果
    void slotOnRobotUDiskShowUp(bool bShowup);

signals:
    /*************
    * 函数名:sigReadRobotVersionFinished
    * 功能:读取机器人版本完成，在读取嵌入式版本和机器人版本完成时都会触发该信号
    * 实际是否读取完成需要结合两者判断
    * 参数:
    * 返回值:
    * 时间: 2016/0728
    * 作者: hwx
    *****************/
    void sigReadRobotVersionFinished();

    // 嵌入式系统版本是否需要需要切换到U盘模式拷贝升级
    void sigIsSystemEmbedVersionNeedCopyUpdate(bool bMustUpdate);

    // 嵌入式版本状态，当前版本和后台版本比较看是否需要更新
    void sigSystemEmbedVersionUpdateStatus(int nStatus);

protected:

    virtual void timerEvent(QTimerEvent *event) override;
    virtual void customEvent(QEvent * event) override;

    /*************
    * 函数名:paintEvent
    * 功能:重绘事件
    * 参数:
    * 返回值:
    * 时间: 2016/06/01
    * 作者: 周志平
    *****************/
    //If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
    void paintEvent(QPaintEvent *) override;

    /*************
    * 函数名:mouseMoveEvent
    * 功能:鼠标移动事件
    * 参数:
    * 返回值:
    * 时间: 2016/06/07
    * 作者: hwx
    *****************/
    virtual void mouseMoveEvent(QMouseEvent*event) override;

    /*************
    * 函数名:mousePressEvent
    * 功能:鼠标按下事件
    * 参数:
    * 返回值:
    * 时间: 2016/06/07
    * 作者: hwx
    *****************/
    virtual void mousePressEvent(QMouseEvent*event) override;

    /*************
    * 函数名:mouseReleaseEvent
    * 功能:鼠标离开事件
    * 参数:
    * 返回值:
    * 时间: 2016/06/07
    * 作者: hwx
    *****************/
    virtual void mouseReleaseEvent(QMouseEvent*event) override;

private slots:

    /**************************************************************************
    * 函数名: slotOnUpdate
    * 功能: 更新版本
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void slotOnUpdate();

    /**************************************************************************
    * 函数名: slotOnCancelUpdate
    * 功能: 取消更新版本
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void slotOnCancelUpdate();

    /**************************************************************************
    * 函数名: slotOnClosed
    * 功能: 关闭界面的槽函数
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void slotOnClosed();

    /**************************************************************************
    * 函数名: slotUpdateUI
    * 功能: 更新UI版本信息
    *
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void slotUpdateUI();

    // U盘格式化完成
    void slotOnFormatUdiskFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void slotOnFormatUdiskStateChanged(QProcess::ProcessState newState);

    // 拷贝文件完成
    void slotOnCopyFileFinished();

private:
    /**************************************************************************
    * 函数名: setOnMenuClickedTrigger
    * 功能: 用户点击菜单栏的固件版本标识
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void setOnMenuClickedTrigger(bool bIsMenuClickedTrigger);

    /**************************************************************************
    * 函数名: downloadUpdateToLocalComputer
    * 功能: 下载更新文件到电脑本地目录
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    bool downloadUpdateToLocalComputer();

    /**************************************************************************
    * 函数名: downloadUpdateToRobot
    * 功能: 从电脑本地目录传输版本更新文件到机器人
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    bool downloadUpdateToRobot();

    /**************************************************************************
    * 函数名: getLocalDownloadFilePath
    * 功能: 获取下载到本地的更新文件
    *
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    QString getLocalDownloadFilePath();

    /**************************************************************************
    * 函数名: reqRobotVersion
    * 功能: 获取当前机器人类型，Alpha1s
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void reqRobotHardwareVersion();

    /**************************************************************************
    * 函数名: reqLocalSystemEmbedVersion
    * 功能: 获取嵌入式版本
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void reqLocalSystemEmbedVersion();

    /**************************************************************************
    * 函数名: reqLatestVersionOnServer
    * 功能: 从服务器请求最新版本
    * 时间: 2016/07/26 16:39
    * 作者: hwx
    */
    void reqLatestSysEmbededVersionOnServer();

    /**************************************************************************
    * 函数名: OnNotify
    * 功能: 处理通知消息
    *     * 参数:
    *    @[in ] pSubject: 主题
    *    @[in ] nMsgID: 消息ID
    *    @[in ] dwContext: 上下文
    *    @[in ] wParam: 参数1
    *    @[in ] lParam: 参数2
    *
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    virtual void OnNotify(const zxObserverSubjectBase* pSubject, int nMsgID, DWORD dwContext, WPARAM wParam, LPARAM lParam) override;

    /**************************************************************************
    * 函数名: processRobotSoftwareVersion
    * 功能: 处理嵌入式版本信息
    *     * 参数:
    *    @[in ] wParam: 参数1，对应参数2的char长度
    *    @[in ] lParam: 参数2
    *
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    bool processRobotSoftwareVersion(WPARAM wParam, LPARAM lParam);

    /**************************************************************************
    * 函数名: processRobotHardwareVersion
    * 功能: 处理机器人版本信息
    *     * 参数:
    *    @[in ] wParam: 参数1，对应参数2的char长度
    *    @[in ] lParam: 参数2
    *
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    bool processRobotHardwareVersion(WPARAM wParam, LPARAM lParam);

    /**************************************************************************
    * 函数名: onDownloadUpdateProgress
    * 功能: 下载到机器人进度
    *
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void onDownloadUpdateProgress(WPARAM wParam, LPARAM lParam);

    /**************************************************************************
    * 函数名: onDownloadUpdateProgressEnd
    * 功能: 下载到机器人完成
    *
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    void onDownloadUpdateProgressEnd(WPARAM wParam, LPARAM lParam);

private:

    // 判断指定路径下是否含有中文名字文件
    bool isActionContainChineseChar(const QString &strFilePath);

    // 判断字符串中是否含有中文字符
    bool isContainsChineseChar(const QString &strText);

    // 格式化移动盘
    void formatUDisk(const QString &strFormat, const QString &strDiskName, const QString strBSDName = "");

    // 判断机器人是否是1P，不是1P即1S
    bool isRobotType1P();

    // 获取bin文件路径
    QString getBinFilePath(bool bIsChinese);

//    CString GetDiskFlag(const CString& sDeviceName, const CString& sDeviceLink);

private:
    Ui::SystemEmbeddedUpdateWidget *ui;

    // 在实现机器人嵌入式版本、机器人类型版本获取、检查更新、弹出更新提示是按顺序依次串行执行，
    // 因此如果在其他多个地方调用获取嵌入式版本和机器人型号的接口，根据MSG_ID都会
    // 接收到对应消息，从而会触发整个串行执行，因此在这里定义标识，只有在用户点击菜单栏的固件
    // 版本时，才会接收消息
    bool m_bOnMenuClickedTrigger;
    bool m_bCanShowMBox; // 是否显示版本信息界面

    bool m_bIsChinese; // 判断中英文

    QPoint   m_dragPosition; // 记录位置信息

    QLabel *m_pProgressLabText;
    QProcess m_formatUdiskProcess;  // 格式化U盘线程
    CProgressDialog* m_pProgressDialog;

    emCheckUpdateType m_eRobotType;  // 版本类型
    QString m_strLocalSysEmbedVersion; // 当前嵌入式版本

    QtHttpOperSync *m_httpOperSync;  // 下载
    FILEINFO m_LatestVersionInfo;  // 最新版本信息
    TRANSFILEINFO  m_TransFileInfo; // 文件信息

    CThreadCopyFile m_oThreadCopyFile;
    CThreadWaitForUdiskShowup m_oThreadWaitUdiskShowUp;

    int m_nUpdateProgressTimerID;

    int m_nTimeCount;
};

#endif // SYSTEMEMBEDDEDUPDATEWIDGET_H
