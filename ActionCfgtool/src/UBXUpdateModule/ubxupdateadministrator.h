/*
* Copyright (c) 2015, 深圳市优必选科技有限公司
* All rights reserved.
*
* 文件名称：UBXUpdateAdministrator.h
* 创建时间：2016/07/23 16:17
* 文件标识：
* 文件摘要：更新模块管理
*
* 当前版本：1.0.0.0
* 作    者：hwx
* 完成时间：2016/07/23 16:17
* 版本摘要：
*/

#ifndef UBXUPDATEADMINISTRATOR_H
#define UBXUPDATEADMINISTRATOR_H

#include <QObject>

#include "systemembeddedupdatewidget.h"
#include "ubxupdatemodule_global.h"
#include "systempcsoftversionui.h"
#include "videotutorialhandle.h"

class UBXUPDATEMODULESHARED_EXPORT UBXUpdateAdministrator : public QObject,  public zxObserverBase
{
    Q_OBJECT

public:
    /**************************************************************************
    * 函数名: getInstance
    * 功能: 获取更新模块管理
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    static UBXUpdateAdministrator *getInstance();

    /**************************************************************************
    * 函数名: OnNotify
    * 功能: 通知消息
    * 参数:
    *    @[in ] pSubject: 被观察者
    *    @[in ] nMsgID: 消息ID
    *    @[in ] dwContext: 消息上下文
    *    @[in ] wParam: 参数1
    *    @[in ] lParam: 参数2
    * 返回值: void
    * 时间: 2015/08/22 19:09
    * 作者: zdj
    */
    virtual void OnNotify(const zxObserverSubjectBase* pSubject, int nMsgID, DWORD dwContext, WPARAM wParam, LPARAM lParam);

public:
    // 设置、获取版本更新地址
    void setUpdateBasicUrl(const QString &sUrl) { m_sUpdateBasicUrl = sUrl; }
    QString getUpdateBasicUrl() { return m_sUpdateBasicUrl; }

    /**************************************************************************
    * 函数名: checkUpdateSystemsEmbedded
    * 功能: 检查嵌入式版本更新
    * 时间: 2016/07/23 16:39
    * 作者: hwx
    */
    bool checkUpdateSystemsEmbedded(bool bCanShowMBox);
    void showSytemPCSoftVersionUI();

    systemPCSoftVersionUI *getSystemPCSoftVersionUI();
    SystemEmbeddedUpdateWidget *getSysEmbedUpdateWidget();
    CVideoTutorialHandle *getVideoTutorialHandle();

private slots:
    void slotOnTimeout();

signals:
    void sigSystemEmbedVersionUpdateStatus(int nStatus);
    void sigIsSystemEmbedVersionNeedCopyUpdate(bool bMustUpdate); // 嵌入式版本信息
    void sigOnNotifyChangedUdiskResult(bool bOk);  // 是否切换U盘模式成功信号

private:
    explicit UBXUpdateAdministrator(QObject *parent = 0);
    ~UBXUpdateAdministrator();

private:
    QString m_sUpdateBasicUrl;   // 版本更新地址

    SystemEmbeddedUpdateWidget *m_pSysEmbedUpdateWidget;
    systemPCSoftVersionUI *m_pSystemPCSoftVersionUI;
    CVideoTutorialHandle *m_pVideoTutorialHandle;
};

#endif // UBXUPDATEADMINISTRATOR_H
